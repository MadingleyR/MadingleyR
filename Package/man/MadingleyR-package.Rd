\name{MadingleyR-package}
\alias{MadingleyR-package}
\alias{MadingleyR}
\docType{package}

\title{MadingleyR package}

\description{The MadingleyR package is a wrapper around the C++ code of the Madingley model. It was intended as an accessible tool to lower the barrier of using the Madingley model, thereby broadening the Madingley community and making simulations easier to reproduce.}

\details{
\strong{The MadingleyR package has 3 major functions:}

\itemize{
  \item \code{madingley_inputs()}: Loads the default Madingley inputs
  \item \code{madingley_init()}: Initialises the Madingley model
  \item \code{madingley_run()}: Runs the Madingley model
}

A full guide to load the required inputs and setup and run a Madingley simulation can be found on: \url{https://madingleyr.github.io/MadingleyR/Manual/UserManual.pdf}\cr\cr

 
\strong{MadingleyR provides several plotting functions:}
 
\itemize{
  \item \code{plot_spatialwindow()}: Plots the selected spatial window (spatial model domain)
  \item \code{plot_densities()}: Plots abundance or biomass density against body mass
  \item \code{plot_foodweb()}: Plots athefood-web plot
  \item \code{plot_spatialabundances()}: Creates a spatial abundance plot
  \item \code{plot_spatialbiomass()}: Creates a spatial biomass plot
  \item \code{plot_timelines()}: Plots the time series (either abundance or biomass)
  \item \code{plot_trophicpyramid()}: Creates trophic pyramide plot
}

Examples of the plots can be found in the full guide linked above.\cr\cr


\strong{Package and C++ source code version:}

\itemize{
  \item \code{madingley_version()}: Prints package and C++ source code versions
}\cr

\strong{The Madingley C++ source code can be compiled on Linux automatically using:}

\itemize{
  \item \code{madingley_download_source()}:	Downloads the MadingleyR source code
  \item \code{madingley_compile()}:	Compiles the source code on Linux
}

For more information on compiling the source code (Linux, MacOS, Windows), see: \url{https://github.com/MadingleyR/MadingleyR/tree/master/SourceCode}\cr\cr

}