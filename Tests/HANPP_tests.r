#detach("package:MadingleyR", unload = TRUE) # unload if loaded, don't mind the error
#remove.packages("MadingleyR")  # uninstall package
#library('remotes') # or use library('devtools') # load the remotes package
#install_github('MadingleyR/MadingleyR', subdir='Package', force = TRUE, build_vignettes = TRUE) # install the MadingleyR package
library(MadingleyR)
madingley_version( ) # check pkg = 1.0.2, C++ src = 2.02

# Set model params
spatial_window = c(35, 37, 50, 52) # region of interest
plot_spatialwindow(spatial_window)
sp_in = madingley_inputs('spatial inputs') # load default inputs

# Initialise model
mdata = madingley_init(spatial_window = spatial_window, spatial_inputs = sp_in)

# Run model of 10 years, without veg reduction
mdata2 = madingley_run(madingley_data = mdata, spatial_inputs = sp_in, years = 10, apply_hanpp = 0) # apply_hanpp = 0, hanpp raster not used

# Run model of 10 years, reduce veg by 50%
sp_in$hanpp[] = 0.5 # this sets all the values of the hanpp raster to 0.5, thus applying the reduction on all cells
mdata3 = madingley_run(madingley_data = mdata, spatial_inputs = sp_in, years = 10, apply_hanpp = 1) # apply_hanpp = 1, fractional raster applied

# Run model of 10 years, reduce veg by 75%
sp_in$hanpp[] = 0.25 # this sets all the values of the hanpp raster to 0.25, thus reducing the veg production by 0.75
mdata4 = madingley_run(madingley_data = mdata, spatial_inputs = sp_in, years = 10, apply_hanpp = 1) # apply_hanpp = 1, fractional raster applied

# Check the veg reduction, sometimes more years are needed to arrived at the right values
mean(mdata2$time_line_stocks$TotalStockBiomass[mdata2$time_line_stocks$Year==10])
mean(mdata3$time_line_stocks$TotalStockBiomass[mdata3$time_line_stocks$Year==10])
mean(mdata4$time_line_stocks$TotalStockBiomass[mdata4$time_line_stocks$Year==10])
