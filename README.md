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

* Basic use
  * R markdown vignette: [view as html](https://madingleyr.github.io/MadingleyR/Vignette/Vignette.html); [pdf](./Vignette/Vignette.pdf)
  * Download as [R code](./Vignette/Vignette.rmd)
* Function description
  * [Package manual pdf](./Manual.pdf)
* Example case studies
  * Case study 1: [html](./Vignette/CaseStudies/CASESTUDY1.md); [pdf](./Vignette/CaseStudies/CASESTUDY1.pdf)
  * Case study 2: [html](./Vignette/CaseStudies/CASESTUDY2.md); [pdf](./Vignette/CaseStudies/CASESTUDY2.pdf)
  * Case study 3: [html](./Vignette/CaseStudies/CASESTUDY3.md); [pdf](./Vignette/CaseStudies/CASESTUDY3.pdf)
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



