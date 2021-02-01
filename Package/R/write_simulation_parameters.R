write_simulation_parameters = function(temp_dir) {
  df = get_simulation_parameters()
  input_dir = paste0(temp_dir,"/input/")
  sink(paste0(input_dir,"SimulationControlParameters.csv"))
  cat(get_input_header(3)); cat("\n")
  for(r in 1:nrow(df)){
    for(c in 1:ncol(df)) {
      if(!is.na(df[r,c])) cat(paste0(df[r,c]))
      if(c!=ncol(df)) cat(",")
    }
    cat("\n")
  }
  sink()
}