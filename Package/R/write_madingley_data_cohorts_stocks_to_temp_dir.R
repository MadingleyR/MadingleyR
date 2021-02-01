write_madingley_data_cohorts_stocks_to_temp_dir = function(input_dir,madingley_data){

	# write madingley_data$cohorts
	options("scipen"=100, "digits"=4)
	cRdir = paste0(input_dir,"C.csv")
	cHEADER = get_cohort_restart_header()
	sink(cRdir)
	for(r in 1:nrow(madingley_data$cohorts)){
	  cat(paste(madingley_data$cohorts[r,],collapse = ",")); cat("\n")
	}
	sink()
	options("scipen"=0, "digits"=7)

	# write madingley_data$stocks
	options("scipen"=100, "digits"=4)
	sRdir = paste0(input_dir,"S.csv")
	sHEADER = get_stock_restart_header()
	sink(sRdir)
	for(r in 1:nrow(madingley_data$stocks)){
	  cat(paste(madingley_data$stocks[r,],collapse = ",")); cat("\n")
	}
	sink()
	options("scipen"=0, "digits"=7)

}
