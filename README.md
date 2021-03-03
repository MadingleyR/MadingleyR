## MadingleyR Installation

The MadingleyR package can be directly installed from R using the ```devtools``` or ```remotes``` R package. The following command installs the package using the remotes R package:

```R
# Load the remotes package
library('remotes') # or use library('devtools')

# Install the MadingleyR package
install_github('MadingleyR/MadingleyR', subdir='Package')
```

In addition to installing the MadingleyR dependencies (```rgdal```, ```sp```, ```data.table``` and ```raster```), the installation process also downloads the precompiled C++ executable (no compilation needed), default spatio-temporal input layers and all other default input parameters and includes them in the installation folder.

## Content

* Package Manual
  * [R markdown vignette pdf](./Manual/UserManual.pdf)
  * [Download R markdown code](./Manual/UserManual.rmd)
  * [Download R code only](./Manual/UserManual.r) (for testing purposes)
* Function description
  * [Package manual pdf](./Manual/RFunctionDescription.pdf)
* Example case studies
  * [Overview](./CaseStudies/)
  * [Case study 1](./CaseStudies/CASESTUDY1.md)
  * [Case study 2](./CaseStudies/CASESTUDY2.md)
  * [Case study 3](./CaseStudies/CASESTUDY3.md)
  * [Full case study codes](https://github.com/MadingleyR/MadingleyR/tree/master/CaseStudies)
* Compile C++ source code
  * [Mac](./SourceCode/CompileMac.md)
  * [Windows](./SourceCode/CompileWindows.md)
  * [Linux](./SourceCode/CompileLinux.md)

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



