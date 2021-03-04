# Load the remotes package
#library('remotes') # or use library('devtools')
# Install the MadingleyR package
#install_github('MadingleyR/MadingleyR', subdir='Package')

####

library(MadingleyR)

# Set model params
spatial_window = c(31, 35, -5, -1) # region of interest: Serengeti
sptl_inp = madingley_inputs('spatial inputs') # load default inputs

# Initialise model
mdata = madingley_init(spatial_window = spatial_window, spatial_inputs = sptl_inp)

# Run spin-up of 100 years (output results to C:/MadingleyOut)
mdata2 = madingley_run(madingley_data = mdata,
                       spatial_inputs = sptl_inp,
                       years = 100)


####

# Set scenario parameters
reps = 5 # set number of replicas per land-use intensity
fractional_veg_production = seq(1.0, 0.1, -0.1) # accessible biomass
m_params = madingley_inputs('model parameters') # load default model parameters
fg = c('Herbivore', 'Carnivore', 'Omnivore') # vector for aggregating cohorts
stats = data.frame() # used to store individual model output statistics

# Loop over land-use intensities
for(j in 1:reps){
  
  mdata3 = mdata2 # copy spin-up MadingleyR object to use in replica
  
  for(i in 1:length(fractional_veg_production)) {
  
    print(paste0("rep: ",j," fraction veg reduced: ",fractional_veg_production[i]))
    
    m_params[86, 2] = fractional_veg_production[i] # lower veg production
    
    mdata4 = madingley_run(
      years = 50,
      madingley_data = mdata3,
      model_parameters = m_params,
      output_timestep = c(99,99,99,99),
      spatial_inputs = sptl_inp,
      silenced = TRUE) 
    
    # Calculate cohort biomass
    cohorts = mdata4$cohorts
    cohorts$Biomass = cohorts$CohortAbundance * cohorts$IndividualBodyMass
    cohorts = cohorts[cohorts$FunctionalGroupIndex<3, ] # only keep endotherms
    cohorts = aggregate(cohorts$Biomass, by = list(fg[cohorts$FunctionalGroupIndex + 1]), sum)
    stats = rbind(stats, cohorts) # attach aggregated stats
  }
}

####

# Calculate mean relative (to control) response per replica simulation
stats$veg_reduced = rep(sort(rep(1 - fractional_veg_production, 3)),reps)
m = aggregate(stats$x, by = list(stats$veg_reduced, stats$Group.1), FUN = median)
m$x_rel = NA;
for(i in fg) {
  m$x_rel[m$Group.2 == i] = m$x[m$Group.2 == i]/m$x[m$Group.2 == i][1]
}

# Make final plots
plot(1 - unique(fractional_veg_production), m$x_rel[m$Group.2 == 'Herbivore'],
     col= 'green', pch = 19, ylim = c(0, 1.5), xlim = c(0, 1),
     xlab = 'Relative vegetation biomass inaccessible', ylab = 'Relative change in cohort biomass')
points(1 - unique(fractional_veg_production), m$x_rel[m$Group.2 =='Carnivore'], col= 'red', pch = 19)
points(1 - unique(fractional_veg_production), m$x_rel[m$Group.2 == 'Omnivore'], col = 'blue', pch = 19)
abline(1, -1, lty = 2)
legend(0.0, 0.3, fg, col=c('green', 'red', 'blue'), pch = 19, box.lwd = 0)
