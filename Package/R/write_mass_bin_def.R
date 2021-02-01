write_mass_bin_def = function(temp_dir) {
  input_dir = paste0(temp_dir,"/input/")
  mass_bins = c(10^6, c(9:1 %o% 10^(5:-2)),0.001,0.0001, 0.00001, 0.000001,0)
  sink(paste0(input_dir,"MassBinDefinitions.csv"))
  cat("Mass bin lower bound"); cat("\n")
  for(i in mass_bins){
    cat(i); cat("\n")
  }
  sink()
}