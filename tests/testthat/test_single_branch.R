test_that("compare runs", {
  set.seed(1)
  
  n_mainland_species <- 1
  sim_time <- 10
  clado_rate <- 1.0
  ext_rate <- 0.1
  carr_cap <- 4
  imm_rate <- 1.0
  ana_rate <- 1.0
  area_pars <- create_area_pars(
    max_area = 1,
    current_area = 1,
    proportional_peak_t = 0,
    total_island_age = 0,
    sea_level_amplitude = 0,
    sea_level_frequency = 0,
    island_gradient_angle = 0)
  hyper_pars <- create_hyper_pars(d = 0, x = 0)
  nonoceanic_pars <- c(0, 0)

    DAISIEiwsim::DAISIE_sim_core_cr(
      time = sim_time,
      mainland_n = n_mainland_species,
      pars = c(clado_rate, ext_rate, carr_cap, imm_rate, ana_rate),
      area_pars = area_pars,
      hyper_pars = hyper_pars,
      nonoceanic_pars = nonoceanic_pars
    )
  
  
})