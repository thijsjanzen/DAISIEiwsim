#pragma once
#include <cmath>
#include <random>
#include <array>

#include "island_spec.h"
#include "island.h"

struct rnd {
  std::mt19937_64 rndgen_;
  rnd() {
    std::random_device rd;
    rndgen_ = std::mt19937_64(rd());
  }
  
  explicit rnd(unsigned int seed) {
    rndgen_ = std::mt19937_64(seed);
  }
  
  void set_seed(unsigned int s) {
    rndgen_ = std::mt19937_64(s);
  }
  
  double exp(double lambda) {
   // std::exponential_distribution<double> d(lambda);
    //return d(rndgen_);
    double out =  R::rexp(1.0 / lambda);
    //std::cerr << "exp: " << out << "\n";
    return out;
  }
  
  int random_number(unsigned int n) {
    //return std::uniform_int_distribution<> (0, n-1)(rndgen_);
    auto r = R::runif(0, n - 1);
   // std::cerr << "sample: " << n << " " << r << "\n";
    return static_cast<int>(r);
  }
};


enum event {immigration, extinction, anagenesis, cladogenesis};
using sttdata = std::vector< std::array< double, 4 >>;


class DAISIE_sim {
public:
  virtual void run() = 0;
  virtual ~DAISIE_sim() {}
  island_spec island_spec_;
  sttdata stt_table_;
};



template<typename ISLAND>
struct DAISIE_sim_impl : public DAISIE_sim {
  const double hyper_pars_d;
  const double hyper_pars_x;
  double time_;
  double total_time;
  ISLAND A;
 
  
  double K;
  double lac;
  double laa;
  double mu;
  double gam;
  
  double max_ext_rate = 1000.0;
  const double mainland_n;
  size_t max_spec_id;
  
  int num_species;
  int num_immigrants;
  
  std::array<double, 4> rates;
  
  rnd rnd_;
  
  // island_spec island_spec_;
  // sttdata stt_table_;
  
  
  DAISIE_sim_impl(const std::vector<double>& pars,
             double h_pars_d,
             double h_pars_x,
             double num_mainland,
             double max_time,
             int seed,
             ISLAND a_in) : 
    hyper_pars_d(h_pars_d), 
    hyper_pars_x(h_pars_x),
    total_time(max_time),
    A(a_in),
    mainland_n(num_mainland) {
    lac = pars[0];
    mu  = pars[1];
    K   = pars[2];
    gam = pars[3];
    laa = pars[4];
    if (seed < 0) {
      rnd_ = rnd();
    } else {
      rnd_ = rnd(seed);
    }
  }
  
  
  void run() override {
    time_ = 0.0;
    island_spec_.clear();
    stt_table_.clear();
    stt_table_.push_back({total_time, 0, 0, 0});
    num_species    = island_spec_.size();
    num_immigrants = island_spec_.num_immigrants();
    max_spec_id = 0;
    
    while(time_ < total_time) {
      update_rates();
      calc_next_timeval(&time_);
      
      if (time_ < total_time) {
        update_rates();
        
        event chosen_event = DAISIE_sample_event_cr();
        
        /*if (chosen_event == event::immigration) {
          std::cerr << time_ << " " << "immigration\n";
        }
        
        if (chosen_event == event::extinction) {
          std::cerr << time_ << " " << "extinction\n";
        }
        if (chosen_event == event::anagenesis) {
          std::cerr << time_ << " " << "anagenesis\n";
        }
        if (chosen_event == event::cladogenesis) {
          std::cerr << time_ << " " << "cladogenesis\n";
        }*/
        
        
        DAISIE_sim_update_state_cr(chosen_event);
        
        update_stt_table();
        num_species = island_spec_.size();
        num_immigrants = stt_table_.back()[1];
      }
    }
  }
  
  void update_rates() {
    A.update_area(time_);
    
    rates[event::immigration]  = get_immigration_rate();
    rates[event::extinction]   = get_extinction_rate();
    rates[event::anagenesis]   = get_anagenesis_rate();
    rates[event::cladogenesis] = get_cladogenesis_rate();

    // std::cerr << "rates: ";
    // for (const auto& i : rates) {
    //   std::cerr << i << " ";
    // } std::cerr << " " << num_species << "\n";

  }
  
  void update_stt_table() {
    std::array<double, 4> add = island_spec_.get_stt(total_time - time_);
    stt_table_.push_back(add);
  }
  
  void DAISIE_sim_update_state_cr(const event chosen_event) {
    switch(chosen_event) {
    case immigration: 
      do_immigration(); break;
    case extinction:
      do_extinction(); break;
    case anagenesis:
      do_anagenesis(); break;
    case cladogenesis:
      do_cladogenesis(); break;
    }
  }
  
  void do_immigration() {
    auto colonist = rnd_.random_number(mainland_n); // +1 because of R counting
 //   std::cerr << "immi: " << colonist << "\n";
    int index = island_spec_.find_species(colonist);
    if (index >= island_spec_.size()) {
      // could not find colonist
      island_spec_.push_back(island_spec_row(colonist, time_, species_type::I));
    } else {
      island_spec_[index].colonisation_time = time_;
    }
  }
  
  void do_anagenesis() {
    std::vector<size_t> potential_immigrants;
    for (size_t i = 0; i < island_spec_.size(); ++i) {
      if (island_spec_[i].type_species == species_type::I)  {
        potential_immigrants.push_back(i);
      }
    }
    
    size_t index = potential_immigrants[0];
    if (potential_immigrants.size() > 1) {
      index = potential_immigrants[ rnd_.random_number(potential_immigrants.size())];
    }
    // std::cerr << "ana: " << index << "\n";
    max_spec_id++;
    island_spec_.anagenesis(index, max_spec_id);
  }
  
  void do_cladogenesis() {
    auto index = rnd_.random_number(island_spec_.size());
  //  std::cerr << "clado: " << index << "\n";
    island_spec_.cladogenesis(index, &max_spec_id, time_);
  }
  
  void do_extinction() {
    auto index = rnd_.random_number(island_spec_.size());
 //   std::cerr << "extinct: " << index << "\n";
    auto type_of_species = island_spec_[index].type_species;
    if (type_of_species == species_type::C) {
      remove_cladogenetic(index);
    } else {
      // if I or A
      island_spec_.remove(index);
    }
  }
  
  event DAISIE_sample_event_cr() {
   
   double sum_rates = rates[0] + rates[1] + rates[2] + rates[3];
    auto r = R::runif(0, sum_rates);    //rnd_.uniform(sum_rates);
  //  std::cerr << "sum_rates: " << r << "\n";
    if (r < rates[immigration]) return immigration;
    if (r < rates[extinction])  return extinction;
    if (r < rates[anagenesis])  return anagenesis;
    
    return cladogenesis;
  }
  
  void calc_next_timeval(double* t) {
    double sum_rates = rates[0] + rates[1] + rates[2] + rates[3];

  //  std::cerr << "next_timeval: " << *t << " " << sum_rates << " ";
       //       rates[0] << " " << rates[1] << " " << rates[2] << " " << rates[3] << " ";
    if (sum_rates > 0) {
      double dt = rnd_.exp(sum_rates);
 //     std::cerr << dt << " ";
      *t += dt;
    } else {
      *t = total_time;
    }
//    std::cerr << "\n";
    return;
  }
  
  double get_immigration_rate() {
    double per_capita = gam * (1.0 - (num_species / (K * A.area)));
    if (per_capita < 0) per_capita = 0.0;
    return mainland_n * per_capita;
  }
  
  double get_extinction_rate() {
    double per_capita = mu;
    if (A.area != 1) {
      per_capita = mu * std::pow(A.area, -hyper_pars_x);
    }
    auto ext_rate = per_capita * num_species;
    if (ext_rate > max_ext_rate) ext_rate = max_ext_rate;
    return ext_rate;
  }
  
  double get_anagenesis_rate() {
    return laa * num_immigrants;
  }
  
  double get_cladogenesis_rate() {
    
    double island_effect = 1;
    if (A.area != 1.0) {
      island_effect = std::pow(A.area, hyper_pars_d);
    }
    
    double per_capita_rate = lac * island_effect *
      (1 - num_species / (K * A.area));
    if (per_capita_rate < 0.0) per_capita_rate = 0.0;
    
    return per_capita_rate * num_species;
  }
  
  void remove_cladogenetic(size_t index_extinct) {
    
    std::vector<size_t> sisters;
    int num_sisters = 0;
    for (size_t i = 0; i < island_spec_.size(); ++i) {
      if (island_spec_[i].parent == island_spec_[index_extinct].parent &&
          island_spec_[i].colonisation_time == island_spec_[index_extinct].colonisation_time) {
        if (i != index_extinct) sisters.push_back(i);
        num_sisters++;
      }
    }
    
    if (num_sisters == 2) {
      auto survivors = sisters.front(); // using survivors to match R code, but this is only a single survivor at a time
      
      island_spec_[survivors].type_species = species_type::A;
      island_spec_[survivors].ext_type = extinction_type::clado_extinct;
      island_spec_[survivors].anc_type.clear();
      
      island_spec_.remove(index_extinct);
      return;
    }
    
    if (num_sisters > 2) {
      std::sort(sisters.begin(), sisters.end());
      
      int number_of_splits = static_cast<int>(island_spec_[index_extinct].anc_type.size());
      auto most_recent_split = island_spec_[index_extinct].anc_type.back();
      
      auto sister_most_recent_split = species::B;
      
      if (most_recent_split == species::B) {
        sister_most_recent_split = species::A;
      }
      if (most_recent_split == species::A) { // for completeness and matching R.
        sister_most_recent_split = species::B;
      }
      
      
      std::vector< species > motif_to_find = island_spec_[index_extinct].anc_type;
      
      
      if (!motif_to_find.empty()) motif_to_find.pop_back();
      
      motif_to_find.push_back(sister_most_recent_split);
      
      std::vector<int> possible_sister;
      
      for (size_t i = 0; i < sisters.size(); ++i) {
        size_t survivor = sisters[i];
        if (match_motif(island_spec_[survivor].anc_type, motif_to_find)) {
          possible_sister.push_back(survivor);
        }
      }
      
      if (most_recent_split == species::A) {
        size_t first_sister = possible_sister.front();
        island_spec_[first_sister].extinction_time = island_spec_[index_extinct].extinction_time;
      }
      // remove the offending A/B
      for (const auto ps : possible_sister) {
        island_spec_[ps].anc_type.erase(island_spec_[ps].anc_type.begin() + number_of_splits - 1);
      }
      island_spec_.remove(index_extinct);
      return;
    }
    return;
  }
};

