plot_spatialwindow = function(spatial_window,raster=0){

  if(class(raster)=="numeric") raster = madingley_inputs('spatial inputs')[[2]]
  plot(raster)
  lines(x=c(spatial_window[[1]],spatial_window[[1]]),y=c(spatial_window[[3]],spatial_window[[4]])) # vert left
  lines(x=c(spatial_window[[2]],spatial_window[[2]]),y=c(spatial_window[[3]],spatial_window[[4]])) # vert right
  lines(x=c(spatial_window[[1]],spatial_window[[2]]),y=c(spatial_window[[4]],spatial_window[[4]])) # vert right
  lines(x=c(spatial_window[[1]],spatial_window[[2]]),y=c(spatial_window[[3]],spatial_window[[3]])) # vert right
}
