return_output_list_run = function(cohort_def,stock_def,out_dir,out_dir_name) {
  output_list = list()
  
  output_list$cohorts = read.csv(paste0(out_dir,"cohort_properties/","FullCohortProperties_99999.csv"))
  output_list$stocks = read.csv(paste0(out_dir,"stock_properties/","StockProperties_99999.csv"))
  
  output_list$cohort_def = cohort_def
  output_list$stock_def = stock_def
  
  output_list$time_line_cohorts = read.csv(paste0(out_dir,"timelines/MontlyCohortBiomass.csv"))
  
  tls = read.csv(paste0(out_dir,"timelines/MonthlyStockBiomass.csv"),header = FALSE)
  tls = as.vector(tls[1:nrow(output_list$time_line_cohorts),])
  output_list$time_line_stocks = data.frame(Month=output_list$time_line_cohorts$Month,
                                            Year=output_list$time_line_cohorts$Year,
                                            TotalStockBiomass=tls)
  
  output_list$out_dir_name = out_dir_name
  
  return(output_list)
}