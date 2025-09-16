#' alternative method to allow comparison rcpp
#' @param v v
#' @param x x
#' @export
sample3 <- function(v, x) {
  num_entries <- length(v) 
  r <- runif(1, 0, num_entries - 1)
  cat("sample: ", num_entries, r, "\n")
  r <- 1 + floor(r)
  return(r)
}