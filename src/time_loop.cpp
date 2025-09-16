#include <Rcpp.h>

#include "util.h"
#include "DAISIE_sim.h"
#include "island.h"


std::unique_ptr<DAISIE_sim> create_daisie_sim(double total_time,
                                              std::vector<double>& pars,
                                              Rcpp::List hyper_pars,
                                              Rcpp::List area_pars,
                                              int seed,
                                              int mainland_n,
                                              int island_ontogeny,
                                              int sea_level) {
  if (island_ontogeny == 0 && sea_level == 0) {
    auto is = island_static();
    return std::unique_ptr<DAISIE_sim>(new DAISIE_sim_impl<island_static>(pars,
                                                                          hyper_pars["d"],
                                                                          hyper_pars["x"],
                                                                          mainland_n,
                                                                          total_time,
                                                                          seed,
                                                                          is));
  } else if (island_ontogeny == 1 && sea_level == 0) {
    auto is_b = island_beta(area_pars["max_area"],
                            area_pars["total_island_age"],
                            area_pars["proportional_peak_t"]);
    
    return std::unique_ptr<DAISIE_sim>(new DAISIE_sim_impl<island_beta>(pars,
                                                                        hyper_pars["d"],
                                                                        hyper_pars["x"],
                                                                        mainland_n,
                                                                        total_time,
                                                                        seed,
                                                                        is_b));
  } else if (island_ontogeny == 0 && sea_level == 1) {
    auto is_a = island_angular(area_pars["sea_level_frequency"],
                               area_pars["total_island_age"],
                               total_time,
                               area_pars["amplitude"],
                               area_pars["current_area"],
                               area_pars["island_gradient_angle"]);
    
    return std::unique_ptr<DAISIE_sim>(new DAISIE_sim_impl<island_angular>(pars,
                                                                           hyper_pars["d"],
                                                                           hyper_pars["x"],
                                                                           mainland_n,
                                                                           total_time,
                                                                           seed,
                                                                           is_a));
  } else if (island_ontogeny == 1 && sea_level == 1) {
    auto is_b_a = island_beta_angular(area_pars["sea_level_frequency"],
                                      area_pars["total_island_age"],
                                      total_time,
                                      area_pars["amplitude"],
                                      area_pars["current_area"],
                                      area_pars["island_gradient_angle"],
                                      area_pars["max_area"],
                                      area_pars["proportional_peak_t"]);
    
    return std::unique_ptr<DAISIE_sim>(new DAISIE_sim_impl<island_beta_angular>(pars,
                                                                                hyper_pars["d"],
                                                                                hyper_pars["x"],
                                                                                mainland_n,
                                                                                total_time,
                                                                                seed,
                                                                                is_b_a));
  } else {
    throw "not viable island ontogeny selected";
  }
}

// [[Rcpp::export]]
Rcpp::List execute_time_loop_rcpp(double total_time,
                                  std::vector<double>& pars,
                                  Rcpp::List hyper_pars,
                                  Rcpp::List  area_pars,
                                  int seed,
                                  int mainland_n,
                                  int island_ontogeny,
                                  int sea_level) {
  
  std::unique_ptr<DAISIE_sim> sim = create_daisie_sim(total_time,
                                                      pars,
                                                      hyper_pars,
                                                      area_pars,
                                                      seed,
                                                      mainland_n,
                                                      island_ontogeny,
                                                      sea_level);
  
  sim->run();
  
  auto island_spec_for_R = make_island_spec_for_R(sim->island_spec_);
  auto stt_table_for_R = make_stt_table_for_R(sim->stt_table_);
  // place holder code to get it to compile for now.
  Rcpp::List output =
    Rcpp::List::create(Rcpp::Named("island_spec") = island_spec_for_R,
                       Rcpp::Named("stt_table") = stt_table_for_R);
  return output;
}



