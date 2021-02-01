return_output_list_ini = function(cohort_def,stock_def,out_dir) {
  output_list = list()
  output_list$cohorts = read.csv(paste0(out_dir,"cohort_properties/","FullCohortProperties_99999.csv"))
  output_list$stocks = read.csv(paste0(out_dir,"stock_properties/","StockProperties_99999.csv"))
  output_list$cohort_def = cohort_def
  output_list$stock_def = stock_def
  return(output_list)
}