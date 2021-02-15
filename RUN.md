### Running the Madingley model

After generating cohorts and stocks, a simulation can be started using the ```madingley_run()``` function. The ```madingley_run()``` function requires the initialisation data set produced by the ```madingley_init()``` function ([see initialisation](./INIT.md)). A typical Madingley simulation first requires a spin-up phase that allows ecosystem components to reach a stable state. This phase usually consists of a 100 to 1000-year model simulation without any model user induced changes. The code below runs the Madingley model for 100 years (```years = 100```) using the previously generated ```mdata``` object ([generated here](./INIT.md)). The standard model input variables (e.g. cohort definitions, stock definitions, spatial inputs and/or model parameters) can be changed for ```madingley_run()``` via the following input parameters: ```cohort_def```, ```stock_def```, ```spatial_inputs```, ```model_parameters``` ([see loading inputs](./INPUTS.md)).

```R
# Run the Madingley model
mdata2 = madingley_run(madingley_data = mdata, 
                       years = 100, 
                       cohort_def = chrt_def, 
                       stock_def = stck_def, 
                       spatial_inputs = sptl_inp, 
                       model_parameters = mdl_prms)
```
