madingley_plot = function(madingley_data){
  
  # store default par
  def_par = par(no.readonly = TRUE)
  
  # try to make plots
  try(plot_timelines(madingley_data, select = 'feeding guilds', main="Time series plot"))
  cat('\n')
  try(plot_densities(madingley_data, weighted = 'biomass', xlim = c(-4, 5), multipanel = TRUE))
  cat('\n')
  try(plot_foodweb(madingley_data, max_flows = 5, sample_data = 30, main="Food-web plot"))
  cat('\n')
  try(plot_trophicpyramid(madingley_data, main="Trophic Pyramid"))
  cat('\n')
  try(plot_spatialbiomass(madingley_data, functional_filter = TRUE, box=FALSE, axes=FALSE))
  
  # return to default par
  par(def_par)
  
}

