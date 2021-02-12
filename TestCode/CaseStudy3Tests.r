library(MadingleyR)

# Set model params
spatial_window = c(-20, 50, -36, 40) 
#spatial_window = c(31, 35, -5, -1) # region of interest: Serengeti
sptl_inp = madingley_inputs('spatial inputs') # load default inputs

# Load cohort definitions
chrt_def = madingley_inputs('cohort definition')
chrt_def[,13] = 50
max_chrt = 100

# Initialize model
mdata = madingley_init(spatial_window = spatial_window, 
                       spatial_inputs = sptl_inp,
                       max_cohort = max_chrt,
                       cohort_def = chrt_def)

# Run spin-up of 100 years 
mdata2 = madingley_run(madingley_data = mdata, 
                       spatial_inputs = sptl_inp, 
                       years = 100,
                       max_cohort = max_chrt,
                       cohort_def = chrt_def,
                       output_timestep = c(100,99,100,100))

plot_spatialbiomass(mdata2)
plot_timelines(mdata2)

# Set scenario parameters
m_params = madingley_inputs('model parameters') # load default model parameters
m_params[28, 2] = 1 # available stock biomass
mdata_list = list(mdata2)
#Sys.sleep(10)

# Loop over land-use intensities
for(i in 1:5) {
  
  # Reduce available stock biomass every iteration by 10%
  m_params[28, 2] = 1 - i * 0.1
  print(m_params[28, 2])
  
  # Run model with reduced stock availability
  mdata_list[[i+1]] = madingley_run(madingley_data = mdata_list[[i]], years = 10,
                                    model_parameters = m_params, 
                                    spatial_inputs = sptl_inp,
                                    max_cohort = max_chrt,
                                    cohort_def = chrt_def,
                                    output_timestep = c(999,999,999,999),
                                    silenced = F)
  
  # Rest for a bit
  #Sys.sleep(10)
  
}

# Run model for additional 10 years
mdata4 = madingley_run(madingley_data = mdata_list[[length(mdata_list)]], 
                       years = 10,
                       model_parameters = m_params, 
                       spatial_inputs = sptl_inp,
                       cohort_def = chrt_def,
                       max_cohort = max_chrt)

# Create plot
herb_bef = plot_spatialbiomass(mdata2, functional_filter = TRUE, plot = FALSE)[[1]]
herb_red = plot_spatialbiomass(mdata4, functional_filter = TRUE, plot = FALSE)[[1]]
plot(herb_red/herb_bef)


