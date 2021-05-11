\name{plot_spatialwindow}
\alias{plot_spatialwindow}
\title{MadingleyR: Plot spatial window}
\usage{
plot_spatialwindow(spatial_window = c(31, 35, -5, -1))
}
\description{Plots the spatial window needed for initialising the Madingley model.}

\arguments{
  \item{spatial_window}{a vector holding coordinates that specify the spatial model domain, specified in the following order: minimum longitude, maximum longitude, minimum latitude, maximum latitude. For example spatial_window = c(31,35,-5,-1).}
  \item{input_raster}{by default input_raster = 0, will plot the realm_classification raster, a custom raster can be provided to plot the spatial windows against another background, these can be obtained from madingley_inputs("spatial inputs"), see example below.}
  \item{window_color}{the color of the spatial window, by default window_color = "red".}
}

\examples{
# Spatial model domain = c(min_long, max_long, min_lat, max_lat)
spatial_window = c(31, 35, -5, -1)

# Plot the spatial window to check selection
plot_spatialwindow(spatial_window)

# With different raster as background (average yearly temperature)
sptl_inp = madingley_inputs("spatial inputs")
mean_yearly_temperature = mean(sptl_inp[[10]])
plot_spatialwindow(spatial_window, input_raster = mean_yearly_temperature, window_color = "purple")
}
