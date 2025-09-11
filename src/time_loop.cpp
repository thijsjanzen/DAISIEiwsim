#include <Rcpp.h>
using namespace Rcpp;

#include "util.h"
#include "DAISIE_sim.h"
#include "island.h"

Rcpp::List execute_time_loop_rcpp(double total_time,
                                  std::vector<double>& pars,
                                  double hyper_par_d,
                                  double hyper_par_x,
                                  double area_pars,
                                  double K,
                                  int seed,
                                  int mainland_n,
                                  int island_ontogeny,
                                  int sea_level,
                                  double max_area,
                                  double total_island_age,
                                  double proportional_peak_t,
                                  double sea_level_frequency,
                                  double total_island_age,
                                  double total_time,
                                  double amplitude,
                                  double current_area,
                                  double island_gradient_angle) {
  
  if (island_ontogeny == 0 && sea_level == 0) {
    auto is = island_static();
    DAISIE_sim<island_static> sim(pars,
                                  hyper_par_d,
                                  hyper_par_x,
                                  mainland_n,
                                  total_time,
                                  seed,
                                  is);
    sim.run();
  }
  
  if (island_ontogeny == 1 && sea_level == 0) {
    auto is_b = island_beta(max_area,
                          total_island_age,
                          proportional_peak_t);
    
    DAISIE_sim<island_beta> sim(pars,
                                hyper_par_d,
                                hyper_par_x,
                                mainland_n,
                                total_time,
                                seed,
                                is_b);
    sim.run();
  }
  
  if (island_ontogeny == 0 && sea_level == 1) {
    auto is_a = island_angular(sea_level_frequency,
                               total_island_age,
                               total_time,
                               amplitude,
                               current_area,
                               island_gradient_angle);
    
    DAISIE_sim<island_angular> sim(pars,
                                hyper_par_d,
                                hyper_par_x,
                                mainland_n,
                                total_time,
                                seed,
                                is_a);
    sim.run();
  }
  
}
