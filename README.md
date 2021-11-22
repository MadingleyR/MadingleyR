## MadingleyR Installation

The MadingleyR package can be directly installed from R using the ```devtools``` or ```remotes``` R package. The following command installs the package using the remotes R package:

```R
# Load the remotes package
library('remotes') # or use library('devtools')

# Install the MadingleyR package
install_github('MadingleyR/MadingleyR', subdir='Package', build_vignettes = TRUE)

# Load MadingleyR package 
library('MadingleyR')

# Get version MadingleyR and C++ source code
madingley_version( )

# View the MadingleyR tutorial vignette
vignette('MadingleyR')
```

In addition to installing the MadingleyR dependencies (```rgdal```, ```sp```, ```data.table``` and ```raster```), the installation process also downloads the precompiled C++ executable (no compilation needed), default spatio-temporal input layers and all other default input parameters and includes them in the installation folder.

## Content

* Tutorial vignette
  * [PDF MadingleyR tutorial vignette](./Package/vignettes/MadingleyR.pdf)
  * [Download R markdown code](./Package/vignettes/MadingleyR.Rmd)
* Function description
  * [Function description pdf](./Documentation/RFunctionDescription.pdf)
  * [MadingleyR parameter description](./Documentation/modelparams.pdf)
  * [Units spatial inputs layers](./Documentation/SpatialInputsUnits.pdf)
* Example case studies
  * [Overview](./CaseStudies/)
  * [Case study 1](./CaseStudies/CASESTUDY1.md)
  * [Case study 2](./CaseStudies/CASESTUDY2.md)
  * [Case study 3](./CaseStudies/CASESTUDY3.md)
  * [Full case study codes](https://github.com/MadingleyR/MadingleyR/tree/master/CaseStudies/FullCaseStudyCodes/)
* C++ source code
  * [Update record](./SourceCode/README.md)
  * [Compile on Mac](./SourceCode/CompileMac.md)
  * [Compile on Windows](./SourceCode/CompileWindows.md)
  * [Compile on Linux](./SourceCode/CompileLinux.md)

## Updates
* 22 Nov 2021 - MadingleyR v1.0.2 (source code v2.02): add control over spatial HANPP

## Useful links

* Additional information about the package can be found [here](https://onlinelibrary.wiley.com/doi/full/10.1111/geb.13354)
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
