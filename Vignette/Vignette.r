#------- MadingleyR Installation  -------#

# Load the remotes package
library('remotes') # or use library('devtools')

# Install the MadingleyR package
install_github('MadingleyR/MadingleyR', subdir='Package')

#------- Model initialisation  -------#
  
# Load package
library(MadingleyR)

# Spatial model domain = c(min_long, max_long, min_lat, max_lat)
spatial_window = c(31, 35, -5, -1)

# plot the spatial window to check selection
plot_spatialwindow(spatial_window)

# Prints possible input options to the R console
madingley_inputs( ) 

# Load MadingleyR default inputs
sptl_inp = madingley_inputs('spatial inputs')
chrt_def = madingley_inputs('cohort definition')
stck_def = madingley_inputs('stock definition')
mdl_prms = madingley_inputs('model parameters') # useful later for running the model

# View the contents of the spatial inputs
str(sptl_inp,2)

# View the default cohort definitions
print(chrt_def)

# View the default stock definitions
print(stck_def)

# Initialise model the model using the pre-loaded inputs
mdata = madingley_init(spatial_window = spatial_window,
                       cohort_def = chrt_def,
                       stock_def = stck_def,
                       spatial_inputs = sptl_inp)

#------- Running the Madingley model -------#

# Run the Madingley model for 10 years
mdata2 = madingley_run(madingley_data = mdata, 
                       years = 10, 
                       cohort_def = chrt_def, 
                       stock_def = stck_def, 
                       spatial_inputs = sptl_inp, 
                       model_parameters = mdl_prms)

#------- Creating plots -------#

# Plot MadingleyR time lines
plot_timelines(mdata2)

# Plot MadingleyR body mass density
plot_densities(mdata2)

# Plot MadingleyR trophic pyramid
plot_trophicpyramid(mdata2)

# Create MadingleyR log10-binned food-web plot
plot_foodweb(mdata2, max_flows = 5)

# Plot MadingleyR spatial biomass
plot_spatialbiomass(mdata2, functional_filter = TRUE)

  
  
  
