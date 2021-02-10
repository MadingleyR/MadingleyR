write_madingley_data_cohorts_stocks_to_temp_dir_fast = 
function (input_dir, madingley_data) 
{
  cRdir = paste0(input_dir, "C.csv")
  data.table::fwrite(madingley_data$cohorts,cRdir,row.names=F,col.names=F)
  
  options(scipen = 100, digits = 4)
  sRdir = paste0(input_dir, "S.csv")
  sHEADER = get_stock_restart_header()
  sink(sRdir)
  for (r in 1:nrow(madingley_data$stocks)) {
    cat(paste(madingley_data$stocks[r, ], collapse = ","))
    cat("\n")
  }
  sink()
  options(scipen = 0, digits = 7)
}
