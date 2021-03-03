write_madingley_data_cohorts_stocks_to_temp_dir_fast = 
function (input_dir, madingley_data) 
{
  cRdir = paste0(input_dir, "C.csv")
  data.table::fwrite(madingley_data$cohorts,cRdir,row.names=F,col.names=F)

  sRdir = paste0(input_dir, "S.csv")
  data.table::fwrite(madingley_data$stocks,sRdir,row.names=F,col.names=F)

}
