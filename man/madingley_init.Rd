\name{madingley_init}
\alias{madingley_init}
\title{MadingleyR: Initialise the Madingley model}
\usage{
# Simple default init
mdata = madingley_init(spatial_window)
# All init parameters manually
mdata = madingley_init(spatial_window, cohort_def, stock_def, spatial_input, max_cohorts)}
\description{This function generates the cohort and stock data.frames (initialisation parameters) needed to run madingley_run().}
\arguments{
 \item{spatial_window}{a vector holding coordinates that specify the spatial model domain, specified in the following order: minimum longitude, maximum longitude, minimum latitude, maximum latitude. For example spatial_window = c(31,35,-5,-1)}
  \item{cohort_def}{a data.frame holding the cohort definitions used for initialising the model. The template or default data.frame can be loaded using: madingley_inputs('cohort definition'). If the cohort_def is not specified, madingley_init() loads the cohort definitions internally using the madingley_inputs('cohort definition') function call}
  \item{stock_def}{a data.frame holding the stock definitions used for initialising the model. The template or default data.frame can be loaded using: madingley_inputs('stock definition'). If the stock_def is not specified, madingley_init() loads the stock definitions internally using the madingley_inputs('stock definition') function call}
  \item{spatial_input}{an R list that holds all of the spatial inputs required for initialising the Madingley model. The default spatial inputs can be loaded using: madingley_inputs('spatial inputs'). If the spatial_input is not specified, madingley_init() loads the inputs internally using the madingley_inputs(''spatial inputs') function call}
  \item{max_cohort}{a single integer holding the maximum number of cohorts allowed per spatial grid cell. The Madingley model mergers similar cohorts if they are highly similar and the maximum number of cohorts is exceeded. This is done for computational reasons (keeping the number of interactions at a reasonable level). The Madingley model merges cohorts if they are highly similar and the maximum number of cohorts is exceeded. Lowering this value will increase model performance at expenses of resolution of cohort diversity. 1000 are normally used, but 500 may be sufficient for preliminary trials. Please note, that setting this value to e.g. 500 in madingley_init() makes sure that max 500 cohorts per grid cell are created. If in a consecutive madingley_run() max_cohort is set to e.g. 1000 the maximum number of cohorts (500 cohorts/grid cell) will increase during the simulation run until the new maximum (1000 cohorts/grid cell) is reached. For consistency it may be preferred to set max_cohort to the same number in a madingley_init() call and the consecutive madingley_run() simulation.}
}
\examples{
# Set spatial model domain
spatial_window = c(31, 35, -5, -1)

# Call the madingley_init function
m_data = madingley_init(spatial_window = spatial_window)

# Load objects needed for a non-default init
spatial_window = c(31, 35, -5, -1)
sptl_inp = madingley_inputs('spatial inputs')
chrt_def = madingley_inputs('cohort definition')
stck_def = madingley_inputs('stock definition')
mdl_prms = madingley_inputs('model parameters')

# Call the madingley_init function with manual inputs
m_data = madingley_init(cohort_def = chrt_def,
                        stock_def = stck_def, spatial_inputs = sptl_inp,
                        spatial_window =s patial_window, max_cohort = 200)
}
