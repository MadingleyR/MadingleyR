load_spatial_inputs_from_package_dir = function(){
  
  # for now testing in the vegan package directory
  path = grep('vegan/data',list.dirs(.libPaths()),value=TRUE)
  #list.files(path)
  #saveRDS(spatial_inputs, paste0(path,"/spatial_inputs.rds"))
  y <- readRDS(paste0(path,"/spatial_inputs.rds"))
  #all.equal(spatial_inputs, y)
  return(y)
}
