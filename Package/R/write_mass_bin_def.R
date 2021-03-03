write_mass_bin_def = function(temp_dir) {
  input_name = paste0(temp_dir,"/input/MassBinDefinitions.csv")
  mass_bins = data.frame(c(10^6, c(9:1 %o% 10^(5:-2)),0.001,0.0001, 0.00001, 0.000001,0))
  
  fileConn<-file(input_name)
  writeLines("Mass bin lower bound", fileConn)
  close(fileConn)
  data.table::fwrite(mass_bins, input_name, row.names=FALSE, col.names=FALSE, append = TRUE, scipen = 999)

}