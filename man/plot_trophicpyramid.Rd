\name{plot_trophicpyramid}
\alias{plot_trophicpyramid}
\title{MadingleyR: Creates trophic pyramide plot}
\usage{plot_trophicpyramid(m_data2)}
\description{The function plots a trophic pyramid from the Madingley simulation outputs.}

\arguments{
  \item{madingley_data}{madingley output list returned by madingley_run()}
  \item{flow_round}{number of decimals for rounding the quantification of flows (default = 2)}
  \item{mass_round}{number of decimals for rounding the quantification of mass (default = 2)}
  \item{col}{vector including the colours for each trophic level}
  \item{text_col}{the colour of the text quantifying the mass of each trophic level}
}

\examples{
# set model params
spatial_window = c(31,35,-5,-1) # Serengeti

# init model
m_data = madingley_init(spatial_window = spatial_window)

# run model
m_data2 = madingley_run(madingley_data = m_data, years = 3)

# make plot
plot_trophicpyramid(m_data2)
}
