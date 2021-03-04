\name{plot_densities}
\alias{plot_densities}
\title{MadingleyR: Plot abundance or biomass density against body mass}
\usage{
plot_densities(m_data2, weighted = 'biomass' , xlim=c(-4, 5))
plot_densities(m_data2, weighted = 'abundance' , xlim=c(-4, 5))
}
\description{It plots the abundance or biomass density against (log10) body mass.}

\arguments{
  \item{madingley_data}{Output list returned by madingley_run()}
  \item{weighted}{select either 'biomass' or 'abundance' (pass as string)}
  \item{xlim}{min and max log10 body mass plotted}
  \item{ylim}{min and max density}
  \item{col}{the colours used (length needs to be equal to the number of groups plotted, by default length = 6)}
  \item{plot}{If TRUE it generates the plot, if FALSE it returns the data output formatted for plotting which can be used for further analysis}
}

\examples{
# set model params
spatial_window = c(31,35,-5,-1) # Serengeti

# init model
m_data = madingley_init(spatial_window = spatial_window)

# run model
m_data2 = madingley_run(madingley_data = m_data, years = 3)

# make plots
plot_densities(m_data2, weighted = 'biomass' , xlim=c(-4, 5))
plot_densities(m_data2, weighted = 'abundance' , xlim=c(-4, 5))
}
