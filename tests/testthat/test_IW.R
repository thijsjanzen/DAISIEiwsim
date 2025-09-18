test_that("test IW", {
  n_mainland_species <- 100
  island_age <- 0.4
  clado_rate <- 5.0
  ext_rate <- 1.0
  clade_carr_cap <- 20.0
  imm_rate <- 0.01
  ana_rate <- 1.0
  
  set.seed(42)
  
  res1 <-  DAISIE_sim_cr(
    time = island_age,
    M = n_mainland_species,
    pars = c(clado_rate, ext_rate, clade_carr_cap, imm_rate, ana_rate),
    replicates = 1,
    divdepmodel = "IW",
    plot_sims = FALSE,
    verbose = FALSE,
    rcpp = FALSE
  )
  
  set.seed(42)
  res2 <-  DAISIE_sim_cr(
    time = island_age,
    M = n_mainland_species,
    pars = c(clado_rate, ext_rate, clade_carr_cap, imm_rate, ana_rate),
    replicates = 1,
    divdepmodel = "IW",
    plot_sims = FALSE,
    verbose = FALSE,
    rcpp = TRUE
  )
  
  testthat::expect_true(all.equal(res1, res2))
  
  for (r in 1:100) {
    set.seed(r)
    clado_rate <- runif(1, 0, 5)
    ext_rate <- runif(1, 0, 1)
    imm_rate <- runif(1, 0, 0.1)
    ana_rate <- runif(1, 0, 2)
    set.seed(r)
    
    res1 <-  DAISIE_sim_cr(
      time = island_age,
      M = n_mainland_species,
      pars = c(clado_rate, ext_rate, clade_carr_cap, imm_rate, ana_rate),
      replicates = 1,
      divdepmodel = "IW",
      plot_sims = FALSE,
      verbose = FALSE,
      rcpp = FALSE
    )
    
    set.seed(r)
    res2 <-  DAISIE_sim_cr(
      time = island_age,
      M = n_mainland_species,
      pars = c(clado_rate, ext_rate, clade_carr_cap, imm_rate, ana_rate),
      replicates = 1,
      divdepmodel = "IW",
      plot_sims = FALSE,
      verbose = FALSE,
      rcpp = TRUE
    )
    testthat::expect_true(all.equal(res1, res2))
  }
})
