# Run these 4 lines to make sure package is updated properly
#detach("package:MadingleyR", unload = TRUE) # unload if loaded, don't mind the error
#remove.packages("MadingleyR")  # uninstall current package
#library('remotes') # or use library('devtools') # load the remotes package
#install_github('MadingleyR/MadingleyR', subdir='Package', force = TRUE, build_vignettes = TRUE) # install the MadingleyR package

# load MadingleyR package
library(MadingleyR)
madingley_version( ) # make sure new versions are installed: R package = 1.0.2, C++ src = 2.02

# set spatial window and load spatial rasters
spatial_window = c(-11, 35, 35, 60)
plot_spatialwindow(spatial_window)
sp_in = madingley_inputs('spatial inputs') # load default inputs, including hanpp layer

# source temporary function for making raster data for autotrophs, this will be included the next version of the package
source("https://raw.githubusercontent.com/SHoeks/RandomMadingleyRFunctions/master/make_spatial_biomass_raster_autotrophs.R")

# plot default hanpp layer
plot(sp_in$hanpp, main="HANPP in gC/m^2/year") # by default this layer is in gC/m^2/year

# HANPP explanation
# when running the model, using madingley_run(), the "apply_hanpp" can be set to apply the hanpp raster:
# by default apply_hanpp = FALSE, so the hanpp raster layer is not used
# apply_hanpp can be set to 1, this assumes the sp_in$hanpp input layer to contains fractional values, and applies them
# these values are from 0 to 1, e.g. a value of 0.8 in a raster cell means a reduction of 0.2 in veg production
# apply_hanpp can be set to 2, this assumes the sp_in$hanpp input layer to be in gC/m^2/year, like the default layer

# see ?madingley_run for all inputs parameters 
# (sometimes R needs a restart to load the help files after a new install of the package)
# when running the model the function will also output which option is selected:
# e.g. "HANPP applied using fractional raster, values quantified in spatial hanpp input layer" apply_hanpp = 1
# or "No HANPP applied" apply_hanpp = FALSE or apply_hanpp = 0

# initialise model, hanpp layer not yet relevant here
mdata = madingley_init(spatial_window = spatial_window, spatial_inputs = sp_in, max_cohort = 40)

# run spin-up (without hanpp), you can ignore the apply_hanpp param or set it manually to apply_hanpp = FALSE or 0 
mdata2 = madingley_run(madingley_data = mdata, years = 10, spatial_inputs = sp_in, max_cohort = 40)
autotr_biomass2 = make_spatial_biomass_raster_autotrophs(mdata2)
plot(log10(autotr_biomass2), zlim=c(12,14)) # make plot from rasters

# run continuation (starting from mdata2) with hanpp using the default input raster, in gC/m^2/year, thus setting apply_hanpp = 2
mdata3 = madingley_run(madingley_data = mdata2, years = 10, spatial_inputs = sp_in, max_cohort = 40, apply_hanpp = 2)
autotr_biomass3 = make_spatial_biomass_raster_autotrophs(mdata3)
plot(log10(autotr_biomass3), zlim=c(12,14)) # make plot from rasters

# run continuation (starting from mdata2 again) using a fractional raster (hypothetical example, not correct conversion)
sp_in$hanpp[] = sp_in$hanpp[] + abs(min(sp_in$hanpp[], na.rm = TRUE)) # this is only for the example, not correct
sp_in$hanpp[] = sp_in$hanpp[] / max(sp_in$hanpp[], na.rm = TRUE)  # this is only for the example, not correct
sp_in$hanpp[] = 1-sp_in$hanpp[]  # this is only for the example, not correct
plot(sp_in$hanpp) # values close to 0 mean intensive reduction in veg mass, e.g. a value of 0.1 is a 0.9 reduction
mdata4 = madingley_run(madingley_data = mdata2, years = 10, spatial_inputs = sp_in, max_cohort = 40, apply_hanpp = 1) #!! apply_hanpp = 1
autotr_biomass4 = make_spatial_biomass_raster_autotrophs(mdata4)
plot(log10(autotr_biomass4), zlim=c(12,14)) # make plot from rasters

# still a global value (single fractional value for all cells) can be set using
sp_in$hanpp[] = 0.4 
plot(sp_in$hanpp) # all values in the raster are 0.4 now

# run mandingley continuation with 0.6 reduction (0.4 veg remaining) to all cells, set apply_hanpp = 1
mdata5 = madingley_run(madingley_data = mdata2, years = 10, spatial_inputs = sp_in, max_cohort = 40, apply_hanpp = 1)
autotr_biomass5 = make_spatial_biomass_raster_autotrophs(mdata5)
plot(log10(autotr_biomass5), zlim=c(12,14)) # make plot from rasters

