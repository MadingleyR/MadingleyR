library(MadingleyR)

# Set model params
spatial_window = c(-20, 50, -36, 40) 
spatial_window = c(31, 35, -5, -1) # smaller test region
sptl_inp = madingley_inputs('spatial inputs') # load default inputs

# Set years
years_spinup = 100
years_vegred = 10
years_postred = 10

# Load cohort definitions
chrt_def = madingley_inputs('cohort definition')
chrt_def[,13] = 40
max_chrt = sum(chrt_def[,13])

# Initialize model
mdata = madingley_init(spatial_window = spatial_window, 
                       spatial_inputs = sptl_inp,
                       max_cohort = max_chrt,
                       cohort_def = chrt_def)

# Run spin-up of 100 years 
mdata2 = madingley_run(madingley_data = mdata, 
                       years = years_spinup,
                       spatial_inputs = sptl_inp, 
                       max_cohort = max_chrt,
                       cohort_def = chrt_def,
                       output_timestep = rep(years_spinup-1,4),
                       out_dir = "~/Desktop/TempMadingleyOuts/")

plot_spatialbiomass(mdata2)
plot_timelines(mdata2)

# Set scenario parameters
m_params = madingley_inputs('model parameters') # load default model parameters
m_params[28, 2] = 1 # available stock biomass
mdata_list = list(mdata2)
#Sys.sleep(10)

save.image("~/Desktop/TempMadingleyOuts/env_sinup.RData")

# Loop over land-use intensities
for(i in 1:5) {
  
  # Reduce available stock biomass every iteration by 10%
  m_params[28, 2] = 1 - i * 0.1
  print(m_params[28, 2])
  
  # Run model with reduced stock availability
  mdata_list[[i+1]] = madingley_run(madingley_data = mdata_list[[i]], 
                                    years = years_vegred,
                                    model_parameters = m_params, 
                                    spatial_inputs = sptl_inp,
                                    max_cohort = max_chrt,
                                    cohort_def = chrt_def,
                                    output_timestep = c(999,999,999,999),
                                    silenced = F,
                                    out_dir = "~/Desktop/TempMadingleyOuts/")
  
  # Rest for a bit
  #Sys.sleep(10)
  
}

save.image("~/Desktop/TempMadingleyOuts/env_reduction.RData")

# Run model for additional 10 years
mdata4 = madingley_run(madingley_data = mdata_list[[length(mdata_list)]], 
                       years = years_postred,
                       model_parameters = m_params, 
                       spatial_inputs = sptl_inp,
                       cohort_def = chrt_def,
                       max_cohort = max_chrt,
                       out_dir = "~/Desktop/TempMadingleyOuts/")



# Create plot
herb_bef = plot_spatialbiomass(mdata2, functional_filter = TRUE, plot = FALSE)[[1]]
herb_red = plot_spatialbiomass(mdata4, functional_filter = TRUE, plot = FALSE)[[1]]
plot(herb_red/herb_bef)

save.image("~/Desktop/TempMadingleyOuts/env_stabilization.RData")
