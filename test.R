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
cat("RCPP START\n")
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

#cat(all.equal(res1, res2), "\n")
cat("done\n")