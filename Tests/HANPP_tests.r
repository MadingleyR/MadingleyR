# Load the remotes package
# library('remotes') # or use library('devtools')

# Install the MadingleyR package
install_github('MadingleyR/MadingleyR', subdir='Package', build_vignettes = TRUE, force = TRUE)

library(MadingleyR)

# Set model params
spatial_window = c(35, 37, 50, 52) # region of interest, somewhere in europe
plot_spatialwindow(spatial_window)
sptl_inp = madingley_inputs('spatial inputs') # load default inputs

# Initialise model
mdata = madingley_init(spatial_window = spatial_window, spatial_inputs = sptl_inp)

# Run spin-up of 10 years 
sptl_inp$hanpp[] = 1
mdata2 = madingley_run(madingley_data = mdata, spatial_inputs = sptl_inp, years = 10, )

# Run spin-up of 10 years 
mdl_params[86,]$values = 1
sptl_inp$hanpp[] = 0.5
mdata3 = madingley_run(madingley_data = mdata, spatial_inputs = sptl_inp, years = 10, model_parameters = mdl_params)

# Run spin-up of 10 years 
mdl_params[86,]$values = 1
sptl_inp$hanpp[] = 10
mdata4 = madingley_run(madingley_data = mdata, spatial_inputs = sptl_inp, years = 10, model_parameters = mdl_params)

# check veg mass
veg1.00 = mean(mdata2$time_line_stocks$TotalStockBiomass[mdata2$time_line_stocks$Year==10])
veg0.50 = mean(mdata3$time_line_stocks$TotalStockBiomass[mdata3$time_line_stocks$Year==10])
veg0.25 = mean(mdata4$time_line_stocks$TotalStockBiomass[mdata4$time_line_stocks$Year==10])


# spatial HANPP test
plot(sptl_inp$hanpp)
mdata = madingley_init(spatial_window = spatial_window, spatial_inputs = sptl_inp)
mdata5 = madingley_run(madingley_data = mdata, spatial_inputs = sptl_inp, years = 1, model_parameters = mdl_params, parallel = FALSE)

