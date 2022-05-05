plot_window = function(XY_window,spatial_raster=NA){
  if(class(spatial_raster)!="logical") {
    plot(spatial_raster)
  }else{
    spatial_path=paste0(get_lib_path(),"/spatial_input_rasters/land_mask.tif")
    raster::plot(raster::raster(spatial_path))
  }
  lines(x=XY_window[1:2],y=XY_window[c(3,3)])
  lines(x=XY_window[1:2],y=XY_window[c(4,4)])
  lines(x=XY_window[c(1,1)],y=XY_window[c(3,4)])
  lines(x=XY_window[c(2,2)],y=XY_window[c(3,4)])
}