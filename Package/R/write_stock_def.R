write_stock_def = function(temp_dir,stock_def) {

  input_name = paste0(temp_dir,"/input/StockFunctionalGroupDefinitions.csv")
  fileConn<-file(input_name)
  writeLines(get_input_header(4), fileConn)
  close(fileConn)
  data.table::fwrite(stock_def, input_name, row.names=FALSE, col.names=FALSE, append = TRUE, , scipen = 999)
}