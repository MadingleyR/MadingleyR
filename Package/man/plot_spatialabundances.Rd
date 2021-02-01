\name{plot_spatialabundances}
\alias{plot_spatialabundances}
\title{MadingleyR: Create spatial abundance plot}
\usage{plot_spatialabundances(m_data2, functional_filter = TRUE)}
\description{It plots the spatial abundances of functional groups spatially.}
\arguments{
\strong{Input arguments}
\item{madingley_data}{madingley output list returned by madingley_run()}
\item{functional_filter}{TRUE or FALSE}
\item{plot}{If set to TRUE it makes the plot, if set to FALSE the function returns a raster or raster brick (default = TRUE)}

\strong{Visual plotting arguments}
\item{box}{TRUE or FALSE (default = TRUE)}
\item{axes}{TRUE or FALSE (default = TRUE)}
\item{names}{plot titles: by default it uses the numbering of the functional groups following the cohort definitions (see madingley_inputs('cohort definition'))}
\item{cex_legend_title}{font size of legend title (default = 1)}
\item{cex_axis}{font size of legend numbering (default = 1.3)}
\item{legend_mar}{the legend margin (default = 20)}
\item{distance_legend_title}{the distance from the legend to the legend title (default = 4)}
\item{legend_width}{the width of the legend bar (default = 2.5)}
}

\examples{
# set model params
spatial_window = c(31,35,-5,-1) # Serengeti

# init model
m_data = madingley_init(spatial_window = spatial_window)

# run model
m_data2 = madingley_run(madingley_data = m_data, years = 3)

# make plot
plot_spatialabundances(m_data2, functional_filter = TRUE)
}
