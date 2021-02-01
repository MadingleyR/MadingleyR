plot_window = function(XY_window,backgroud="mean temp"){
  if(backgroud=="mean temp") {
    plot(mean(spatial_inputs$`near-surface_temperature`))
  }
  lines(x=XY_window[1:2],y=XY_window[c(3,3)])
  lines(x=XY_window[1:2],y=XY_window[c(4,4)])
  lines(x=XY_window[c(1,1)],y=XY_window[c(3,4)])
  lines(x=XY_window[c(2,2)],y=XY_window[c(3,4)])
}