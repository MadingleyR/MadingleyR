# Load the remotes package
# library('remotes') # or use library('devtools')

# Install the MadingleyR package
#install_github('MadingleyR/MadingleyR', subdir='Package', force = TRUE)
library(MadingleyR)

# Set model params
spatial_window = c(35, 37, 50, 52) # region of interest, somewhere in europe
plot_spatialwindow(spatial_window)
sptl_inp = madingley_inputs('spatial inputs') # load default inputs

# Initialise model
mdata = madingley_init(spatial_window = spatial_window, spatial_inputs = sptl_inp)

# Run spin-up of 10 years 
mdata2 = madingley_run(madingley_data = mdata, spatial_inputs = sptl_inp, years = 10, apply_hanpp = 0)
mean(mdata2$time_line_stocks$TotalStockBiomass[mdata2$time_line_stocks$Year==10])

# Run spin-up of 10 years 
sptl_inp$hanpp[] = 0.5
mdata3 = madingley_run(madingley_data = mdata, spatial_inputs = sptl_inp, years = 10, apply_hanpp = 1)
mean(mdata3$time_line_stocks$TotalStockBiomass[mdata3$time_line_stocks$Year==10])

# Run spin-up of 10 years 
sptl_inp$hanpp[] = 0.25
mdata4 = madingley_run(madingley_data = mdata, spatial_inputs = sptl_inp, years = 10, apply_hanpp = 1)
mean(mdata4$time_line_stocks$TotalStockBiomass[mdata4$time_line_stocks$Year==10])


madingley_version()
