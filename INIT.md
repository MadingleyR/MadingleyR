### Model initialisation

The function ```madingley_init()``` initialises a model run by generating a cohort and stock data set. Both data sets are returned as data frames in a list object (here named: ```mdata```) after the ```madingley_init()``` finishes. The cohort data set contains functional information for all cohorts (i.e. heterotrophs) needed to run a Madingley simulation (```mdata$cohorts```). The stock data set holds the functional information concerning the stocks (i.e. photo-autotrophs) (```mdata$stocks```). The generated data sets are based on the functional definitions defined in ```cohort_def``` (cohort definitions) and ```stock_def``` (stock definitions). ```spatial_window``` defines the boundaries of the spatial location, formatted as a vector containing four coordinates in the following order: 1) minimum longitude, 2) maximum longitude, 3) minimum latitude and 4) maximum latitude. The R code shown below illustrates the use of the ```madingley_init()``` function for an area that includes the Serengeti. 

```R
# Load package
library(MadingleyR)

# Spatial model domain = c(min_long, max_long, min_lat, max_lat)
spatial_window = c(31, 35, -5, -1)

# Load inputs manually
madingley_inputs( ) # prints the possible input options to the R console
sptl_inp = madingley_inputs('spatial inputs')
chrt_def = madingley_inputs('cohort definition')
stck_def = madingley_inputs('stock definition')
mdl_prms = madingley_inputs('model parameters') # useful later for running the model

# Initialise model the model using the pre-loaded inputs
mdata = madingley_init(spatial_window = spatial_window,
                       cohort_def = chrt_def,
                       stock_def = stck_def,
                       spatial_inputs = sptl_inp
                       )
```
