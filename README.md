## MadingleyR Installation

The MadingleyR package can be directly installed from R using the ```devtools``` or ```remotes``` R package. The following command installs the package using the remotes R package:

```R
# Load the remotes package
library('remotes') # or use library('devtools')

# Install the MadingleyR package
install_github('MadingleyR/MadingleyR', subdir='Package')
```

In addition to installing the MadingleyR dependencies (```rgdal```, ```sp```, ```data.table``` and ```raster```), the installation process also downloads the precompiled C++ executable, default spatio-temporal input layers and all other default input parameters and includes them in the installation folder.

## Content

* Basic use
  * [R markdown vignette](https://madingleyr.github.io/MadingleyR/Vignette/Vignette.html)
  * [Download R markdown vignette as pdf](./Vignette/Vignette.pdf)
  * [R markdown vignette code, for testing purposes](./Vignette/Vignette.rmd)
* Function description
  * [Package manual](./Manual.pdf)
* Case studies
  * [Case study 1](./Vignette/CaseStudies/CASESTUDY1.md)
  * [Case study 2](./Vignette/CaseStudies/CASESTUDY2.md)
  * [Case study 3](./Vignette/CaseStudies/CASESTUDY3.md)
* Compile source
  * [Compile source Mac](./SourceCode/CompileMac.md)
  * [Compile source Windows](./SourceCode/CompileWindows.md)
  * [Compile source Linux](./SourceCode/CompileLinux.md)

## Useful links

* Additional information about the package can be found [here](addlink) (paper link not yet available)
* More information about the Madingley model can be found [here](https://journals.plos.org/plosbiology/article?id=10.1371/journal.pbio.1001841)

## MadingleyR workflow

<p align="center">
<img src="Figures/fig1.png" alt="Fig1" width="90%"/>
<br>
<em>Overview of the MadingleyR workflow.</em>
<br>
<br>
<br>
</p>

## Contact information

email: NA@na.na



