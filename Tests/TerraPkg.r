#-------------------------------------
# Test MadingleyR
# Intergration of Terra (removing raster and rgdal dependencies)
# MadingleyR           -> 1.0.6
# Madingley C++ source ->  2.02
# Functions updated:
# check_and_rewrite_spatial_inputs --> use of raster::cellStats()
# madingley_inputs --> use of raster::raster(), raster::brick()
# madingley_run --> use of raster::maxValue(), raster::res()
# madingley_init --> use of raster::res()
# plot_spatialabundances --> use of raster::extent(), raster::raster(), raster::crop(), raster::resample(), raster::as.matrix(), raster::plot(), raster::stack()
# plot_spatialbiomass --> use of raster::extent(), raster::raster(), raster::crop(), raster::resample(), raster::as.matrix(), raster::plot(), raster::stack()
# plot_window --> raster::raster(), raster::plot()
# write_spatial_inputs_to_temp_dir() --> use of raster::xyFromCell()
#-------------------------------------

#----- REINSTALL PACKAGE
if(FALSE){
  detach("package:MadingleyR", unload = TRUE) # unload if loaded, don't mind the error
  remove.packages("MadingleyR")  # uninstall current package
  library('remotes') # or use library('devtools') # load the remotes package
  install_github('MadingleyR/MadingleyR', subdir='Package', force = TRUE, build_vignettes = FALSE) # install the MadingleyR package
}

#------ SOURCE FUNCTIONS FOR DEBUGGING

# source function for package r dir
if(FALSE){
  dir = "Z:/Repositories/MadingleyR/MadingleyR/Package/R"
  lapply(list.files(dir,full.names = TRUE), source)
  setwd(dir)
}

#------ SMALL TEST CASE

# load MadingleyR package
library(terra)
library(MadingleyR)
madingley_version( ) # make sure new versions are installed: R package = 1.0.2, C++ src = 2.02

# set spatial window and load spatial rasters
spatial_window = c(-11, 25, 35, 60)
plot_spatialwindow(spatial_window)
spatial_inputs = madingley_inputs('spatial inputs')

# cohort input parameters
max_cohorts = 500
cohort_inputs = madingley_inputs('cohort definition')
cohort_inputs$PROPERTY_Initial.number.of.GridCellCohorts = round(max_cohorts/nrow(cohort_inputs))

# initialise model
mdata = madingley_init(spatial_window = spatial_window, spatial_inputs = spatial_inputs, max_cohort = max_cohorts)

# run spin-up for 10 years
mdata2 = madingley_run(madingley_data = mdata, years = 10, spatial_inputs = spatial_inputs, max_cohort = max_cohorts)

# plot results
plot_timelines(mdata2)
plot_densities(mdata2)
plot_trophicpyramid(mdata2)
plot_foodweb(mdata2, max_flows = 5)
plot_spatialbiomass(mdata2, functional_filter = FALSE)
plot_spatialbiomass(mdata2, functional_filter = TRUE)
plot_spatialabundances(mdata2, functional_filter = FALSE)
plot_spatialabundances(mdata2, functional_filter = TRUE)
plot_spatialabundances(mdata2, functional_filter = TRUE, plot = FALSE)
madingley_version( )
