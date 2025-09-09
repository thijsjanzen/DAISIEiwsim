#include <Rcpp.h>
using namespace Rcpp;




// You can include R code blocks in C++ files processed with sourceCpp
// (useful for testing and development). The R code will be automatically 
// run after the compilation.
//

Rcpp::List execute_time_loop_rcpp(double timeval,
                                  double total_time,
                                  std::vector<double>& pars,
                                  double hyuper_pars,
                                  double area_pars,
                                  double K,
                                  int num_spec,
                                  int num_immigrants,
                                  int mainland_n,
                                  int island_ontogeny,
                                  int sea_level) {
  /*
  double t = 0.0;
  while(t < timeval) {
    update_rates();
    calc_next_timeval();
    if (timeval < total_time) {
      rates <- update_rates();
      event = DAISIE_sample_event_cr();
      updated_state <- DAISIE_sim_update_state_cr();
      
      island_spec <- updated_state$island_spec
      maxspecID <- updated_state$maxspecID
      stt_table <- updated_state$stt_table
      num_spec <- length(island_spec[, 1])
        num_immigrants <- length(which(island_spec[, 4] == "I"))
    }

  }
  
 */
  
  
}
