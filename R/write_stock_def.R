write_stock_def = function(temp_dir,stock_def) {
  input_dir = paste0(temp_dir,"/input/")
  sink(paste0(input_dir,"StockFunctionalGroupDefinitions.csv"))
  cat(get_input_header(4)); cat("\n")
  for(r in 1:nrow(stock_def)){
    for(c in 1:ncol(stock_def)) {
      if(!is.na(stock_def[r,c])) cat(paste0(stock_def[r,c]))
      if(c!=ncol(stock_def)) cat(",")
    }
    cat("\n")
  }
  sink()
}