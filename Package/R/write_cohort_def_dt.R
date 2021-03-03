write_cohort_def_dt = function(temp_dir,cohort_def) {
  cohort_def$PROPERTY_Maximum.mass = cohort_def$PROPERTY_Maximum.mass + 1
  input_name = paste0(temp_dir,"/input/CohortFunctionalGroupDefinitions.csv")
  fileConn<-file(input_name)
  writeLines(get_input_header(1), fileConn)
  close(fileConn)
  data.table::fwrite(cohort_def, input_name, row.names=FALSE, col.names=FALSE, append = TRUE)
}