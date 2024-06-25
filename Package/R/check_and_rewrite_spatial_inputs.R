check_and_rewrite_spatial_inputs = function(spatial_inputs,tempdir=tempdir()){

  # default val
  return_val = FALSE

  #-- check if spatial inputs have changed from last write to temp folder
  # raster_version
  # check_prev = unlist(lapply(spatial_inputs,function(x) {c(cellStats(x,stat='sd'),cellStats(x,stat='mean'))}))
  # check_prev = sort(as.vector(check_prev))
  # terra update
  check = unlist(lapply(spatial_inputs2,function(x) {c(terra::global(x,fun='sd',na.rm=TRUE),terra::global(x,fun='mean',na.rm=TRUE))}))
  check = sort(as.vector(check)) 
  
  check_check = FALSE
  if(file.exists(paste0(tempdir,"/spatial_inputs/spatial_inputs.csv"))){
    check_check <- try(all.equal(check,read.csv(paste0(tempdir,"/spatial_inputs/spatial_inputs.csv"),row.names = NULL)$check), silent=TRUE)
    if(class(check_check)=="try-error") check_check = FALSE # write data
  }else{
    check_check = FALSE
  }
  dir.create(paste0(tempdir,"/spatial_inputs/"), showWarnings = FALSE)
  write.csv(data.frame(check=check),paste0(tempdir,"/spatial_inputs/spatial_inputs.csv"))

  # write spatial model inputs to temp dir (only if they have changed from last ini or run)
  if(class(check_check)!="logical") check_check = FALSE
  if(!check_check) return_val = TRUE

  write_spatial = TRUE
  if(file.exists(paste0(tempdir,"/spatial_inputs/1deg/available_water_capacity.csv"))) write_spatial = FALSE
  if(write_spatial) return_val = TRUE

  return(return_val)
}
