## Case study 1

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
remove_idx = which(mdata2$cohorts$AdultMass > 1e5 & mdata2$cohorts$FunctionalGroupIndex == 0)
mdata2$cohorts = mdata2$cohorts[-remove_idx, ]

# Run large herbivore removal simulation (for 50 years)
mdata4 = madingley_run(madingley_data = mdata2, years = 50, spatial_inputs = sptl_inp) 

# Make plots
par(mfrow = c(1, 2))
plot_foodweb(mdata3, max_flows = 5) # control food-web plot
plot_foodweb(mdata4, max_flows = 5) # large-herbivore removal food-web plot
```
<p align="center">
<img src="Figures/fig5.png" alt="Fig5" width="70%"/>
<br>
<em>Log10-binned food-web plots construction from a control simulation (a) and a simulation in which large and mega (>100 kg) endothermic herbivores were removed (b). Node colour depicts feeding category: carnivores (red), omnivores (blue) and herbivores (green). Grey lines connecting the nodes illustrate the flows between grouped cohorts. These results can be replicated, without any further dependencies, using the code shown in the main text.</em>
<br>
<br>
<br>
</p>
