#' Internal function of the DAISIE simulation
#'
#' @inheritParams default_params_doc
#' @keywords internal
DAISIE_sim_core_cr_rcpp <- function(
    time,
    mainland_n,
    pars,
    nonoceanic_pars,
    hyper_pars,
    area_pars
) {
  
  #### Initialization ####
  timeval <- 0
  total_time <- time
  
  testit::assert(length(pars) == 5 || length(pars) == 10)
  
  testit::assert(is.null(area_pars) || are_area_pars(area_pars))
  if (pars[4] == 0 && nonoceanic_pars[1] == 0) {
    stop("Island has no species and the rate of
    colonisation is zero. Island cannot be colonised.")
  }
  
  nonoceanic_sample <- DAISIE_nonoceanic_spec(
        prob_samp = nonoceanic_pars[1],
        prob_nonend = nonoceanic_pars[2],
        mainland_n = mainland_n)
  
  maxspecID <- mainland_n
  island_spec <- c()
  stt_table <- matrix(ncol = 4)
  colnames(stt_table) <- c("Time", "nI", "nA", "nC")
  
  spec_tables <- DAISIE_spec_tables(stt_table,
                                    total_time,
                                    timeval,
                                    nonoceanic_sample,
                                    island_spec,
                                    maxspecID)
  
  maxspecID <- spec_tables$maxspecID
  stt_table <- spec_tables$stt_table
  mainland_spec <- spec_tables$mainland_spec
  island_spec <- spec_tables$island_spec
  lac <- pars[1]
  mu <- pars[2]
  K <- pars[3]
  gam <- pars[4]
  laa <- pars[5]
  
  num_spec <- length(island_spec[, 1])
  num_immigrants <- length(which(island_spec[, 4] == "I"))
  
  results <- execute_time_loop_rcpp(total_time,
                                    pars,
                                    hyper_pars,
                                    area_pars,
                                    seed,
                                    mainland_n,
                                    island_ontogeny,
                                    sea_level)
  
  stt_table = results$stt_table
  colnames(stt_table) <- c("Time","nI","nA","nC")
  island_spec = results$island_spec
  
  
  #### Finalize STT ####
  stt_table <- rbind(
    stt_table,
    c(
      0,
      stt_table[nrow(stt_table), 2],
      stt_table[nrow(stt_table), 3],
      stt_table[nrow(stt_table), 4]
    )
  )
  island <- DAISIE_create_island(
    stt_table = stt_table,
    total_time = total_time,
    island_spec = island_spec,
    mainland_n = mainland_n)
  
  ordered_stt_times <- sort(island$stt_table[, 1], decreasing = TRUE)
  testit::assert(all(ordered_stt_times == island$stt_table[, 1]))
  return(island)
}
