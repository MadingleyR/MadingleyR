write_simulation_parameters = function(temp_dir) {

  input_name = paste0(temp_dir,"/input/SimulationControlParameters.csv")
  fileConn<-file(input_name)
  writeLines(get_input_header(3), fileConn)
  close(fileConn)
  data.table::fwrite(get_simulation_parameters(), input_name, row.names=FALSE, col.names=FALSE, append = TRUE, scipen = 999)
  
}