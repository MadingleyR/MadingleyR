madingey_plot = function(madingley_data){

  def.par = par(no.readonly = TRUE)

  layout(matrix(c(7, 7, 7, 7, 7, 7, 1, 2, 3, 4, 5, 6), nrow=4, byrow=TRUE))

  plot_densities(madingley_data, weighted = 'biomass', xlim = c(-4, 5), multipanel = F)
  cat('\n')
  plot_timelines(madingley_data, select = 'feeding guilds', main="Time series plot")

  par(def.par)

  try(plot_foodweb(madingley_data, max_flows = 5, sample_data = 30, main="Food-web plot"))
  cat('\n')
  try(plot_trophicpyramid(madingley_data, main="Trophic Pyramid"))
  cat('\n')
  try(plot_spatialbiomass(madingley_data, functional_filter = T, box=F, axes=F))

}

### Test code
# library(MadingleyR)
# sptl_inp = madingley_inputs('spatial inputs')
# chrt_def = madingley_inputs('cohort definition')
# stck_def = madingley_inputs('stock definition')
# mdl_prms = madingley_inputs('model parameters')
#
# mdata = madingley_init(spatial_window = c(31, 35, -5, -1),
#                        cohort_def = chrt_def,
#                        stock_def = stck_def,
#                        spatial_inputs = sptl_inp)
#
# mdata2 = madingley_run(madingley_data = mdata,
#                        years = 100,
#                        cohort_def = chrt_def,
#                        stock_def = stck_def,
#                        spatial_inputs = sptl_inp,
#                        model_parameters = mdl_prms)
#
# madingey_plot(mdata2)
### Test code
