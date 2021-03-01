\name{madingley_download_source}
\alias{madingley_download_source}
\title{MadingleyR: Downloads the MadingleyR source code}
\usage{
# The source code can be downloaded using
src_path = madingley_download_source(madingley_path = "~/Documents")

# src_path and the downloaded files can be used in madingley_compile()
madingley_compile(madingley_path = src_path)
}

\description{This function downloads and extracts the MadingleyR source code (zip) into the specified folder.}
 
\arguments{
  \item{madingley_path}{The download folder}
}

\examples{
# The source code can be downloaded using
src_path = madingley_download_source(madingley_path = "~/Documents")

# Compile source code, src_path can be used in madingley_compile()
madingley_compile(madingley_path = src_path)
}
