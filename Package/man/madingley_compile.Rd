\name{madingley_compile}
\alias{madingley_compile}
\title{MadingleyR: Compiles the source code on Linux}
\usage{
# Compile the source code
madingley_compile(madingley_path = "~/Documents/MadingleyR-master/SourceCode/madingley", install_new_binary = FALSE)

# The source code can be downloaded using
src_path = madingley_download_source("~/Documents")

# src_path can be used in madingley_compile()
madingley_compile(madingley_path = src_path)
}

\description{This function compiles the source code for Linux. Usually the binary supplied with the package works on Linux systems. However, this function can be used to compile the source code and installed it into the pacakge library. This function is also useful when making alteration to the source code. The compiled binary file can be found within the specified source folder (labelled with the current date).
}
 
\arguments{
  \item{madingley_path}{The path of the madingley source code files (can be obtained using madingley download)}
  \item{install_new_binary}{a bool (TRUE or FALSE) indicating if the compiled source needs to be installed directly into the pacakge library (TRUE) or not (FALSE). By default this is set to FALSE.}
}

\examples{
# The source code can be downloaded using
src_path = madingley_download_source("~/Documents")

# Compile source code, src_path can be used in madingley_compile()
madingley_compile(madingley_path = src_path)
}
