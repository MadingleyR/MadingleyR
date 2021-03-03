madingley_plot = function(madingley_data){

  # store default par
  def_par = par(no.readonly = TRUE)

  layout(matrix(c(7, 7, 7, 7, 7, 7, 1, 2, 3, 4, 5, 6), nrow=4, byrow=TRUE))

  plot_densities(madingley_data, weighted = 'biomass', xlim = c(-4, 5), multipanel = F)
  cat('\n')
  plot_timelines(madingley_data, select = 'feeding guilds', main="Time series plot")

  # return to default par
  par(def_par)

  try(plot_foodweb(madingley_data, max_flows = 5, sample_data = 30, main="Food-web plot"))
  cat('\n')
  try(plot_trophicpyramid(madingley_data, main="Trophic Pyramid"))
  cat('\n')
  try(plot_spatialbiomass(madingley_data, functional_filter = T, box=F, axes=F))

}
