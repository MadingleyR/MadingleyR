# Case studies

### Case study 1

To investigate the role of large and megaherbivores in the Serengeti, we removed endothermic herbivores with a body mass >100 kg from the simulation. This was done by first initialising and running the model for 100-year period (model spin-up) using the default cohort definitions, stock definitions, spatio-temporal input layers and model parameters:

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
```

From the resulting MadingleyR object (```mdata2```) we remove all endothermic herbivore cohorts with a body mass of >100 kg. The modified object was then used to run a consecutive simulation of 50 years. For the control simulation, the same principle was applied, but without any modification to the object created by the model spin-up. Food-web results were plotted for means of comparison between the control and removal simulation:

```R
# Run 50-year control simulation (for later comparison)
mdata3 = madingley_run(madingley_data = mdata2, years = 50, spatial_inputs = sptl_inp)

# Remove large (>100 kg) endothermic herbivores from mdata$cohorts
remove_idx = which(mdata2$cohorts$AdultMass > 1e+5 & mdata2$cohorts$FunctionalGroupIndex == 0)
mdata2$cohorts = mdata2$cohorts[-remove_idx, ]

# Run large herbivore removal simulation (for 50 years)
mdata4 = madingley_run(madingley_data = mdata2, years = 50, spatial_inputs = sptl_inp) 

# Make plots
par(mfrow = c(1, 2))
plot_foodweb(mdata3, max_flows = 5) # control food-web plot
plot_foodweb(mdata4, max_flows = 5) # large-herbivore removal food-web plot
```
<p>
<img src="Figures/fig5.png" alt="Fig5" width="70%"/>
<br>
<em>Log10-binned food-web plots construction from a control simulation (a) and a simulation in which large and mega (>100 kg) endothermic herbivores were removed (b). Node colour depicts feeding category: carnivores (red), omnivores (blue) and herbivores (green). Grey lines connecting the nodes illustrate the flows between grouped cohorts. These results can be replicated, without any further dependencies, using the code shown in the main text.</em>
<br>
<br>
<br>
</p>

### Case study 2

In the second example, we reduced the relative biomass of autotrophs (i.e., vegetation) accessible for herbivory and observed how it affected the biomass of endotherms. First a 100-year spin-up simulation was run using the default MadingleyR input parameters (as described for case study 1).

```R
library(MadingleyR)
setwd('C:/MadingleyOut') # create this directory if it does not exist

# Set model params
spatial_window = c(31, 35, -5, -1) # region of interest: Serengeti
sptl_inp = madingley_inputs('spatial inputs') # load default inputs

# Initialize model
mdata = madingley_init(spatial_window = spatial_window, spatial_inputs = sptl_inp)

# Run spin-up of 100 years (output results to C:/MadingleyOut)
mdata2 = madingley_run(out_dir = 'C:/MadingleyOut', 
                       madingley_data = mdata, 
                       spatial_inputs = sptl_inp, 
                       years = 100)
```
This spin-up simulation was then extended by an additional 5 years without any reduction in available autotroph biomass and the end state was used as the control. The 100-year spin-up was then also used to run 8 independent land-use intensity experiments where the fraction accessible stock mass for herbivory was reduced by 0.1 increments to test the effects over a gradient of land-use intensities. This was done by modifying the default model input parameters (overview of all default parameters is shown in Supporting Information Appendix S4 and S5). Each land-use intensity experiment was run for 5 years and with 10 replicas. The end state of each land-use intensity scenario was compared to the control run.

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
<p>
<img src="Figures/fig6.png" alt="Fig6" width="50%"/>
<br>
<em>Relative change in biomass of endotherm cohorts compared to the control simulation (biomass experiment/biomass control) plotted against the proportion of plant biomass made inaccessible to herbivores and omnivores. A relative change in biomass of 1 indicates no change. Data points represent the relative change in biomass of endothermic carnivores (red), omnivores (blue) and herbivores (green) averaged over 10 replicas extracted at the end of the 5-year simulation experiment. The dashed line indicates the impact expected if the biomass of endotherms decreased linearly with the amount of plant made inaccessible for feeding (i.e. y = −x).</em>
<br>
<br>
<br>
</p>
