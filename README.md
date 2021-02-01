# MadingleyR
R package of the Madingley General Ecosystem Model. The MadingleyR R package streamlines the installation procedure and supports all major operating systems. MadingleyR enables users to combine multiple consecutive simulation runs, making case study specific modifications to MadingleyR objects along the way. 

- The package manual is accessible from within R or can be downloaded from [here](https://github.com/MadingleyR/MadingleyR/raw/master/Manual.pdf)
- Additional information about the package can be found [here](addlink)
- More information about the Madingley model can be found [here](https://journals.plos.org/plosbiology/article?id=10.1371/journal.pbio.1001841)

To install the package run:

```R
# install MadingleyR package
library('remotes') # or use library('devtools')
install_github('MadingleyR/MadingleyR', subdir='Package')
??MadingleyR # open function help pages
```
Test code (from paper):

```R

# Load package
library(MadingleyR)

# Spatial model domain = c(min_long, max_long, min_lat, max_lat)
spatial_window = c(31, 35, -5, -1)

# initialise model (generate cohorts and stocks)
mdata = madingley_init(spatial_window = spatial_window)

# Load inputs manually
madingley_inputs( ) # prints the possible input options to the R console
sptl_inp = madingley_inputs('spatial inputs')
chrt_def = madingley_inputs('cohort definition')
stck_def = madingley_inputs('stock definition')
mdl_prms = madingley_inputs('model parameters')

# initialise model (generate cohorts and stocks)
mdata = madingley_init(spatial_window = c(31, 35, -5, -1),
                       cohort_def = chrt_def,
                       stock_def = stck_def,
                       spatial_inputs = sptl_inp)

# Run the Madingley model
mdata2 = madingley_run(madingley_data = mdata, 
                       years = 100, 
                       cohort_def = chrt_def, 
                       stock_def = stck_def, 
                       spatial_inputs = sptl_inp, 
                       model_parameters = mdl_prms)
 
# Create MadingleyR plots
madingley_plot(mdata2) 
```

Case study spin-up (from paper):

```R
library(MadingleyR)
setwd('C:/MadingleyOut') # create this directory if it does not exist

# Set model params
spatial_window = c(31, 35, -5, -1) # region of interest: Serengeti
sptl_inp = madingley_inputs('spatial inputs') # load default inputs

# set the maximum body masses of the functional groups manually
sptl_inp$Endo_O_max[ ] = 2e+05 # set max size omnivores = 200 kg
sptl_inp$Endo_C_max[ ] = 6e+05 # set max size carnivores = 600 kg
sptl_inp$Ecto_max[ ] = 1.5e+05 # set max size ectotherms = 150 kg

# Initialize model
mdata = madingley_init(spatial_window = spatial_window, spatial_inputs = sptl_inp)

# Run spin-up of 100 years (output results to C:/MadingleyOut)
mdata2 = madingley_run(out_dir = 'C:/MadingleyOut', 
                       madingley_data = mdata, 
                       spatial_inputs = sptl_inp, 
                       years = 100)

# Save environment
save(mdata2, sptl_inp, file = 'model_spin_up.RData')

```

Case study 1 (from paper):

```R
# Load model spin-up
load('C:/MadingleyOut/model_spin_up.RData')

# Run 50-year control simulation (for later comparison)
mdata3 = madingley_run(madingley_data = mdata2, years = 50, spatial_inputs = sptl_inp)

# Remove large (>100 kg) endothermic herbivores from mdata$cohorts
remove_idx = which(mdata2$cohorts$AdultMass > 1e+5 & mdata2$cohorts$FunctionalGroupIndex == 0)
mdata2$cohorts = mdata2$cohorts[-remove_idx, ]

# Run large herbivore removal simulation
mdata4 = madingley_run(madingley_data = mdata2, years = 50, spatial_inputs = sptl_inp) 

# Make plot
par(mfrow = c(1, 2))
plot_foodweb(mdata3, max_flows = 5) # control food-web plot
plot_foodweb(mdata4, max_flows = 5) # large-herbivore removal food-web plot
```

Case study 2 (from paper):

```R
# Load model spin-up
load('C:/MadingleyOut/model_spin_up.RData')

# Set scenario parameters
reps = 10 # set number of replicas per land-use intensity
avail_bio = sort(rep(seq(1.0, 0.2, -0.1), reps), decreasing = T) # accessible biomass
m_params = madingley_inputs('model parameters') # load default model parameters
fg = c('Herbivore', 'Carnivore', 'Omnivore') # vector for aggregating cohorts
stats = data.frame() # used to store individual model output statistics

# Loop over land-use intensities
for(i in 1:length(avail_bio)) {
 m_params[27, 2] = avail_bio[i] # accessible biomass (see model parameters)
 
 cohorts = madingley_run(
  years = 5, 
  madingley_data = mdata2, 
  model_parameters = m_params, 
  spatial_inputs = sptl_inp)$cohorts # store cohort results only
 
 # Calculate cohort biomass
 cohorts$Biomass = cohorts$CohortAbundance * cohorts$IndividualBodyMass 
 cohorts = cohorts[cohorts$FunctionalGroupIndex<3, ] # only keep endotherms
 cohorts = aggregate(cohorts$Biomass, by = list(fg[cohorts$FunctionalGroupIndex + 1]), sum)
 stats = rbind(stats, cohorts) # attach aggregated stats
}

# Calculate mean relative (to control) response per replica simulation
stats$veg_reduced = sort(rep(1 - avail_bio, 3))
m = aggregate(stats$x, by = list(stats$veg_reduced, stats$Group.1), FUN = mean)
m$x_rel = NA;
for(i in fg) { 
 m$x_rel[m$Group.2 == i] = m$x[m$Group.2 == i]/m$x[m$Group.2 == i][1] 
}

# Make final plots
plot(1 - unique(red_avail_bio), m$x_rel[m$Group.2 == 'Herbivore'], 
     col= 'green', pch = 19, ylim = c(0, 1.5), xlim = c(0, 1),
     xlab = 'Relative vegetation biomass inaccessible', ylab = 'Relative change in cohort biomass')
points(1 - unique(red_avail_bio), m$x_rel[m$Group.2 =='Carnivore'], col= 'red', pch = 19)
points(1 - unique(red_avail_bio), m$x_rel[m$Group.2 == 'Omnivore'], col = 'blue', pch = 19)
abline(1, -1, lty = 2)
legend(0.0, 0.2, fg, col=c('green', 'red', 'blue'), pch = 19, box.lwd = 0)

```

Test code:

```R
# unload pkg and remove installation
detach('package:MadingleyR', unload=TRUE)
remove.packages('MadingleyR')
rm(list=ls())

# install pkg
remotes::install_github('MadingleyR/MadingleyR', subdir='Package')

# test run
library(MadingleyR)
?madingley_init
??MadingleyR

# open installation dir
system(paste('open',get_lib_path()))

# open temp dir
system(paste('open',tempdir()))

# set model params
spatial_window = c(31,35,-5,-1) # Serengeti

# init model
m_data = madingley_init(spatial_window = spatial_window)

mm = madingley_inputs("model parameters")

# run model
#m_data2 = madingley_run(out_dir='C:/test test/',madingley_data = m_data, years = 4)
m_data2 = madingley_run(madingley_data = m_data, 
                        years = 1, 
                        dispersal_off = T, 
                        noise_cohort_order = 0.35, 
                        output_timestep = c(99,99,99,99), 
                        parallel = F, 
                        model_parameters = mm)

# plots
plot_timelines(m_data2,select='functional groups',ylim=c(5.5,11.5))
plot_timelines(m_data2,select='feeding guilds',ylim=c(8,17))
plot_foodweb(m_data2,max_flows=5)

plot_densities(m_data2,weighted='biomass',xlim=c(-4,5))
plot_densities(m_data2,weighted='abundance',xlim=c(-4,5))
plot_trophicpyramid(m_data2)
plot_spatialabundances(m_data2,functional_filter=T)
plot_spatialbiomass(m_data2,functional_filter=T)


# non default run
spatial_window = c(110,160,-50,0) # Australia
sptl_inp = madingley_inputs('spatial inputs')
chrt_def = madingley_inputs('cohort definition')
stck_def = madingley_inputs('stock definition')
mdl_prms = madingley_inputs('model parameters')
plot_spatialwindow(spatial_window,sptl_inp[[1]])

cname = paste0(m_data2$cohort_def[, 2], '-', m_data2$cohort_def[, 7], '-', m_data2$cohort_def[, 6])
plot_spatialabundances(m_data2, functional_filter = T, box=F, axes=F, names = cname)


# init model
m_data = madingley_init(cohort_def=chrt_def,
                        stock_def=stck_def,
                        spatial_inputs=sptl_inp,
                        spatial_window=spatial_window,
                        max_cohort = 200)

m_data3 = madingley_run(madingley_data = m_data,
                        years = 4,
                        max_cohort = 50,
                        cohort_def = chrt_def,
                        stock_def = stck_def,
                        spatial_inputs = sptl_inp,
                        model_parameters = mdl_prms)


# plots for paper
plot_spatialwindow(m_data3$spatial_window,sptl_inp[[1]])
plot_timelines(m_data3,select='feeding guilds',ylim=c(8,17))
plot_densities(m_data3,weighted='biomass',xlim=c(-4,5))
plot_foodweb(m_data3,max_flows=5)
plot_trophicpyramid(m_data3)
plot_spatialbiomass(m_data3,functional_filter=F,zlim=c(7,15))
```
