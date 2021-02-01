write_cohort_def = function(temp_dir,cohort_def) {
  cohort_def$PROPERTY_Maximum.mass = cohort_def$PROPERTY_Maximum.mass + 1
  input_dir = paste0(temp_dir,"/input/")
  sink(paste0(input_dir,"CohortFunctionalGroupDefinitions.csv"))
  cat(get_input_header(1)); cat("\n")
  for(r in 1:nrow(cohort_def)){
    for(c in 1:ncol(cohort_def)) {
      if(!is.na(cohort_def[r,c])) cat(paste0(cohort_def[r,c]))
      if(c!=ncol(cohort_def)) cat(",")
    }
    cat("\n")
  }
  sink()
}