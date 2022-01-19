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

## Troubleshooting

Please note that if the installation via Github fails, the MadingleyR package can be installed locally using the latest static release. Follow this [link](https://github.com/MadingleyR/MadingleyR/releases) for the installation guide, select the latest release to make sure all updates are included.

## Updates
* 05 Sept 2021 - MadingleyR v1.0.0 (source code v2.00): windows output folder fix
* 23 Nov 2021 - MadingleyR v1.0.2 (source code v2.02): add control over spatial HANPP (see [HANPP example](https://github.com/MadingleyR/MadingleyR/blob/master/Tests/HANPP_tests2.r))
* 19 Jan 2022 - MadingleyR v1.0.3 (source code v2.03): unit fixes for biomass of vegetation in plotting functions

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
