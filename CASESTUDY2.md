## Case study 2

In the second example, we reduced the relative biomass of autotrophs (i.e., vegetation) accessible for herbivory and observed how it affected the biomass of endotherms. First a 100-year spin-up simulation was run using the default MadingleyR input parameters. The code below shows how the initialisation and spin-up simulation can be done, case study one uses the exact same procedure and provides more explanation on the code ([see](./CASESTUDY1.md)). Please note that ```madingley_run()``` tries to write outputs to ```C:/MadingleyOut```, make sure this folder exists or modify the path.

```R
library(MadingleyR)

# Set model params
spatial_window = c(31, 35, -5, -1) # region of interest: Serengeti
sptl_inp = madingley_inputs('spatial inputs') # load default inputs

# Initialise model
mdata = madingley_init(spatial_window = spatial_window, spatial_inputs = sptl_inp)

# Run spin-up of 100 years (output results to C:/MadingleyOut)
mdata2 = madingley_run(out_dir = 'C:/MadingleyOut', 
                       madingley_data = mdata, 
                       spatial_inputs = sptl_inp, 
                       years = 100)
```


Next, this spin-up simulation is extended by an additional 5 years without any reduction in available autotroph biomass and the end state was used as the control. The 100-year spin-up was then also used to run 8 independent land-use intensity experiments where the fraction accessible stock mass for herbivory was reduced by 0.1 increments to test the effects over a gradient of land-use intensities. This was done by modifying the default model input parameters (```madingley_inputs('model parameters')```). Each land-use intensity experiment was run for 5 years and with 10 replicas. 

```R

# Set scenario parameters
reps = 10 # set number of replicas per land-use intensity
avail_bio = sort(rep(seq(1.0, 0.2, -0.1), reps), decreasing = T) # accessible biomass
m_params = madingley_inputs('model parameters') # load default model parameters
fg = c('Herbivore', 'Carnivore', 'Omnivore') # vector for aggregating cohorts
stats = data.frame() # used to store individual model output statistics

# Loop over land-use intensities
for(i in 1:length(avail_bio)) {
 m_params[28, 2] = avail_bio[i] # accessible biomass (see model parameters)
 
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
```
The end state of each land-use intensity scenario can then be compared to the control run using the code below:

```R

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
<p align="center">
<img src="Figures/fig6.png" alt="Fig6" width="50%"/>
<br>
<em>Relative change in biomass of endotherm cohorts compared to the control simulation (biomass experiment/biomass control) plotted against the proportion of plant biomass made inaccessible to herbivores and omnivores. A relative change in biomass of 1 indicates no change. Data points represent the relative change in biomass of endothermic carnivores (red), omnivores (blue) and herbivores (green) averaged over 10 replicas extracted at the end of the 5-year simulation experiment. The dashed line indicates the impact expected if the biomass of endotherms decreased linearly with the amount of plant made inaccessible for feeding (i.e. y = −x).</em>
<br>
<br>
<br>
</p>
