check_and_rewrite_spatial_inputs = function(spatial_inputs,tempdir=tempdir()){

  return_val = F

  # check if spatial inputs have changed from last write to temp folder
  check = sort(as.vector(unlist(lapply(spatial_inputs,function(x) {c(cellStats(x,stat='sd'),cellStats(x,stat='mean'))}))))
  check_check = F
  if(file.exists(paste0(tempdir,"/spatial_inputs/spatial_inputs.csv"))){
    check_check <- try(all.equal(check,read.csv(paste0(tempdir,"/spatial_inputs/spatial_inputs.csv"),row.names = NULL)$check), silent=T)
    if(class(check_check)=="try-error") check_check = F # write data
  }else{
    check_check = F
  }
  dir.create(paste0(tempdir,"/spatial_inputs/"), showWarnings = FALSE)
  write.csv(data.frame(check=check),paste0(tempdir,"/spatial_inputs/spatial_inputs.csv"))

  # write spatial model inputs to temp dir (only if they have changed from last ini or run)
  if(class(check_check)!="logical") check_check = F
  if(!check_check) return_val = T

  write_spatial = T
  if(file.exists(paste0(tempdir,"/spatial_inputs/1deg/available_water_capacity.csv"))) write_spatial = F
  if(write_spatial) return_val = T

  return(return_val)
}
