# MadingleyR C++ source

### Compile
Compile the C++ source code for the  MadingleyR

* [Compile code for Mac](./CompileMac.md)
* [Compile code for Windows](./CompileWindows.md)
* [Compile code for Linux](./CompileLinux.md)

Download source code [here](https://github.com/MadingleyR/MadingleyR/archive/master.zip) and viewed online [here](https://github.com/MadingleyR/MadingleyR/tree/master/SourceCode).

### Update record

Update description | References | Version Number | Files
--- | --- | --- | ---
Food-web outputting, this allows for the tracking of feeding directions and rates. | source code Hoeks et al. (2020) | <2.00 | ```Eating*.cpp```, ```WriteModelState.cpp```
Large carnivore predator-prey ratio, large carnivores (>21 kg) are able to feed on larger prey. | Carbone et al. (1999) | <2.00 | ```MadingleyInitialisation.cpp```
Spatial control of the initialisation of cohort body mass range, maximum body masses can be set using the spatial input layers provided in the package. | Wilman et al. (2014); IUCN Red List (2017); Myhrvold et al. (2015); Roll et al. (2017) |  <2.00 | ```MadingleyInitialisation.cpp```
Removal of netCDF dependencies, allowing for more straightforward compilation of the C++ source code on Windows. | This repository | 2.00 | ```LoadCSVEnvironment.cpp```
I/O (input and output) handling for communication with R. | This repository | 2.01 | ```Main.cpp```, ```LoadCSVEnvironment.cpp```, ```WriteModelState.cpp```, ```InputParameters.cpp```
Direct control of all parameters included in the model equations that define the ecological behaviour of cohorts and stocks. | This repository | 2.01 | ```InputParameters.cpp```, Applies to ecological files
Control of HANPP using spatial input layer | This repository | 2.02 | ```xxx.cpp```

### References

- Carbone, C., Georgina M. M., Roberts S. C., Macdonald D. W. (1999). Energetic constraints on the diet of terrestrial carnivores. Nature 402, 6759: 286-288. https://doi.org/10.1038/46266
- Hoeks, S., Huijbregts, M. A. J., Busana, M., Harfoot, M. B. J., Svenning, J.-C. and Santini, L. (2020). Mechanistic insights into the role of large carnivores for ecosystem structure and functioning. Ecography. https://doi.org/10.1111/ecog.05191
- Wilman, H., Belmaker, J., Simpson, J., de la Rosa, C., Rivadeneira, M. M., & Jetz, W. (2014). EltonTraits 1.0: Species‐level foraging attributes of the world's birds and mammals: Ecological Archives E095‐178. Ecology, 95(7), 2027-2027. https://doi.org/10.1890/13-1917.1
- IUCN. (2017). The IUCN red list of threatened species. Ver. 2017‐1. – IUCN, www.iucnredlist.org
- Myhrvold, N. P., Baldridge, E., Chan, B., Sivam, D., Freeman, D. L., & Ernest, S. M. (2015). An amniote life‐history database to perform comparative analyses with birds, mammals, and reptiles. Ecology, 96(11), 3109-3109. https://doi.org/10.1890/15-0846R.1
- Roll, U., Feldman, A., Novosolov, M., Allison, A., Bauer, A. M., Bernard, R., ... & Meiri, S. (2017). The global distribution of tetrapods reveals a need for targeted reptile conservation. Nature Ecology & Evolution, 1(11), 1677-1682. https://doi.org/10.1038/s41559-017-0332-2




