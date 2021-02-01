\name{madingley_inputs}
\alias{madingley_inputs}
\title{MadingleyR: Load the default Madingley}
\usage{
madingley_inputs() # prints all available options
sptl_inp = madingley_inputs("spatial inputs") # loads the default spatial inputs
chrt_def = madingley_inputs("cohort definition") # loads the default cohort definitions
stck_def = madingley_inputs("stock definition") # loads the default stock definitions
mdl_prms = madingley_inputs("model parameters") # loads the default model parameters
}

\description{This function loads the default MadingleyR input parameters in case the model parameters need to be checked or modified depending on the simulation scenario. The function takes one string input. The available options can be printed to the R console with madingley_inputs( ), see example code below.}

\arguments{
  \item{input_type}{a string specifying the input type to load. The options are: "spatial inputs", "cohort definition", "stock definition" or "model parameters". Running the function without any string will print all available options as shown in the usage above or example below.}
}

\examples{
# Print all the available options to load to the R console
madingley_inputs( )

# Load spatial parameters
sptl_inp = madingley_inputs(input_type = "spatial inputs")

# Load default cohort property definitions
chrt_def = madingley_inputs(input_type = "cohort definition")

# Load default stock property definitions
stck_def = madingley_inputs(input_type = "stock definition")

# Load default model parameters
mdl_prms = madingley_inputs(input_type = "model parameters")

# Set spatial model domain
spatial_window = c(31, 35, -5, -1)

## ----------------------------##
# Change parameters accordingly #
##-----------------------------##

# Call the madingley_init function
m_data = madingley_init(cohort_def = chrt_def,
                        stock_def = stck_def,
                        spatial_inputs = sptl_inp,
                        max_cohort = 500)

# Run the madingley model
m_data2 = madingley_run(madingley_data = m_data,
                        years = 5,
                        max_cohort = 500,
                        cohort_def = chrt_def,
                        stock_def = stck_def,
                        spatial_inputs = sptl_inp,
                        model_parameters = mdl_prms)
}
