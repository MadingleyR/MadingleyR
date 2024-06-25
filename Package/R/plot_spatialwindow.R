plot_spatialwindow = function(spatial_window, input_raster=0, window_color="red",...){

  # by default do not add the legend
  add_legend = FALSE
  
  if(length(spatial_window)!=4) {
    stop("Please provide 4 coordinates c(min_long, max_long, min_lat, max_lat)")
  }
  
  if(!spatial_window[[1]]<spatial_window[[2]]) {
    stop("Order of coordinates not defined correctly c(min_long, max_long, min_lat, max_lat)")
  }
  if(!spatial_window[[3]]<spatial_window[[4]]) {
    stop("Order of coordinates not defined correctly c(min_long, max_long, min_lat, max_lat)")
  }
  
  if(class(input_raster)=="numeric") {
    raster_path = paste0(get_lib_path(),"/spatial_input_rasters/realm_classification.tif")
    # old code: input_raster = raster::raster(raster_path)
    input_raster = terra::rast(raster_path)
    input_raster[input_raster[]==1] = NA
  }else{
    add_legend = TRUE
  }
  
  # old code: raster::plot(input_raster,colNA="grey",legend=add_legend,...)
  terra::plot(input_raster,colNA="grey",legend=add_legend,...)
  lines(x=c(spatial_window[[1]],spatial_window[[1]]),y=c(spatial_window[[3]],spatial_window[[4]]), col = window_color) # vert left
  lines(x=c(spatial_window[[2]],spatial_window[[2]]),y=c(spatial_window[[3]],spatial_window[[4]]), col = window_color) # vert right
  lines(x=c(spatial_window[[1]],spatial_window[[2]]),y=c(spatial_window[[4]],spatial_window[[4]]), col = window_color) # vert right
  lines(x=c(spatial_window[[1]],spatial_window[[2]]),y=c(spatial_window[[3]],spatial_window[[3]]), col = window_color) # vert right
}
