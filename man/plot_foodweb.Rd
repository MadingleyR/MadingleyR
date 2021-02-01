\name{plot_foodweb}
\alias{plot_foodweb}
\title{MadingleyR: Make food web plot}
\usage{plot_foodweb(m_data2, max_flows = 5, sample_data = 100)}
\description{It plots the log10-binned body mass food web.}
\arguments{
 \item{madingley_data}{madingley output list returned by madingley_run()}
 \item{max_flows}{set the maximum number of flows per node}
 \item{color}{TRUE (color version of the plot) or FALSE (grey scale version of the plot)}
 \item{sample_data}{By default the function takes all data into consideration (sample_data = 100) but can be computationally intensive. To speed up the visualization of the results it might be useful to only consider a random sample of the data, e.g. sample_data = 30 will sample 30\% of the data randomly and use it to make the food-web plot}
}

\examples{
# set model params
spatial_window = c(31,35,-5,-1) # Serengeti

# init model
m_data = madingley_init(spatial_window = spatial_window)

# run model
m_data2 = madingley_run(madingley_data = m_data, years = 3)

# make plot
plot_foodweb(m_data2, max_flows = 5, sample_data = 100)
}
