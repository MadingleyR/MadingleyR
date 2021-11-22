get_default_model_parameters = function(){

  # default activity params
  Activity_Parameters = c(6.61,1.6,1.51,1.53)
  Activity_names = rep("Activity_Parameters",length(Activity_Parameters))

  # default dispersal params
  Dispersal_Parameters = c(0.0278,0.48,50000,0.0278,0.48,0.8)
  Dispersal_names = rep("Dispersal_Parameters",length(Dispersal_Parameters))

  # default carnivory params
  EatingCarnivory_Parameters = c(0.5,0.7,0.5,0.7,1.0,0.000006,1.0,0.7)
  EatingCarnivory_names = rep("EatingCarnivory_Parameters",length(EatingCarnivory_Parameters))

  # default omnivory params
  EatingCarnivory_Parameters = c(EatingCarnivory_Parameters,0.1)
  EatingCarnivory_names = c(EatingCarnivory_names,"EatingOmnivory_Parameters")

  # default herbivory params
  EatingHerbivory_Parameters = c(0.7,0.7,0.7,0.7,1.0,1.0E-11,1.0,2.1,0.1)
  EatingHerbivory_names = c(rep("EatingHerbivory_Parameters",length(EatingHerbivory_Parameters)))

  # default metabolism params
  MetabolismEctotherm_Parameters = c(0.88,148984000000,0.69,8.617e-5,41918272883,0.69,0.036697248)
  MetabolismEctotherm_names = rep("MetabolismEctotherm_Parameters",length(MetabolismEctotherm_Parameters))
  MetabolismEndotherm_Parameters = c(0.7,9.0809083973E+11,0.69,8.617e-5,0.0366972,37.00)
  MetabolismEndotherm_names = rep("MetabolismEndotherm_Parameters",length(MetabolismEndotherm_Parameters))
  MetabolismHeterotroph_Parameters = c(0.71,0.69,8.617e-5)
  MetabolismHeterotroph_names = rep("MetabolismHeterotroph_Parameters",length(MetabolismHeterotroph_Parameters))

  Mortality_Parameters = c(0.001,0.003,0.6,0.05,1.0)
  Mortality_names = rep("Mortality_Parameters",length(Mortality_Parameters))

  Reproduction_Parameters = c(1.5,0.25,0.05,0.5)
  Reproduction_names = rep("Reproduction_Parameters",length(Reproduction_Parameters))


  VegetationModel_Parameters  = c(0.961644704,0.237468183,0.100597089,0.001184101,
                                  7.154615416,1.270782192,-1.828591558,0.844864063,
                                  0.040273936,1.013070062,0.020575964,-1.195235464,
                                  0.04309283,-1.478393163,0.139462774,-4.395910091,
                                  0.362742634,0.388125108,19.98393943,1.148698636,
                                  8.419032427,0.01,24.0,0.01,24.0,0.01,12.0,1,
                                  0.001,2.0,0.00000226032940698105,0.476,0)
  VegetationModel_names = rep("VegetationModel_Parameters",length(VegetationModel_Parameters))

  All_values = c(Activity_Parameters,
                 Dispersal_Parameters,
                 EatingCarnivory_Parameters,
                 EatingHerbivory_Parameters,
                 MetabolismEctotherm_Parameters,
                 MetabolismEndotherm_Parameters,
                 MetabolismHeterotroph_Parameters,
                 Mortality_Parameters,
                 Reproduction_Parameters,
                 VegetationModel_Parameters)

  ALL_params = c(Activity_names,
                 Dispersal_names,
                 EatingCarnivory_names,
                 EatingHerbivory_names,
                 MetabolismEctotherm_names,
                 MetabolismEndotherm_names,
                 MetabolismHeterotroph_names,
                 Mortality_names,
                 Reproduction_names,
                 VegetationModel_names)

  # create data.frame
  ModelParameters = data.frame(params=ALL_params,values=All_values)

  # add parameter notes
  ModelParameters$category = NA
  ModelParameters$notes = NA
  i = 1

  # Activity_Parameters
  ModelParameters$category[i] = "Activity"
  ModelParameters$notes[i] = "Terrestrial Warming Tolerance Intercept"; i = i + 1
  ModelParameters$category[i] = "Activity"
  ModelParameters$notes[i] = "Terrestrial Warming Tolerance Slope"; i = i + 1
  ModelParameters$category[i] = "Activity"
  ModelParameters$notes[i] = "Terrestrial TSM Intercept"; i = i + 1
  ModelParameters$category[i] = "Activity"
  ModelParameters$notes[i] = "Terrestrial TSM Slope"; i = i + 1
  
  # Dispersal_Parameters
  ModelParameters$category[i] = "Diffusive Dispersal"
  ModelParameters$notes[i] = "Speed Body Mass Scalar"; i = i + 1
  ModelParameters$category[i] = "Diffusive Dispersal"
  ModelParameters$notes[i] = "Speed Body Mass Exponent"; i = i + 1
  ModelParameters$category[i] = "Responsive Dispersal"
  ModelParameters$notes[i] = "Density Threshold Scaling"; i = i + 1
  ModelParameters$category[i] = "Responsive Dispersal"
  ModelParameters$notes[i] = "Speed Body Mass Scalar"; i = i + 1
  ModelParameters$category[i] = "Responsive Dispersal"
  ModelParameters$notes[i] = "Speed Body Mass Exponent"; i = i + 1
  ModelParameters$category[i] = "Responsive Dispersal"
  ModelParameters$notes[i] = "Starvation Dispersal Body Mass Threshold"; i = i + 1

  # EatingCarnivory_Parameters
  ModelParameters$category[i] = "Eating Carnivory"
  ModelParameters$notes[i] = "Handling Time Scalar Terrestrial"; i = i + 1
  ModelParameters$category[i] = "Eating Carnivory"
  ModelParameters$notes[i] = "Handling Time Exponent Terrestrial"; i = i + 1
  ModelParameters$category[i] = "Eating Carnivory"
  ModelParameters$notes[i] = "Handling Time Scalar Marine (not applicable to current version)"; i = i + 1
  ModelParameters$category[i] = "Eating Carnivory"
  ModelParameters$notes[i] = "Handling Time Exponent Marine (not applicable to current version)"; i = i + 1
  ModelParameters$category[i] = "Eating Carnivory"
  ModelParameters$notes[i] = "Referenc eMass"; i = i + 1
  ModelParameters$category[i] = "Eating Carnivory"
  ModelParameters$notes[i] = "Kill Rate Constant"; i = i + 1
  ModelParameters$category[i] = "Eating Carnivory"
  ModelParameters$notes[i] = "Kill Rate Constant Mass Exponent"; i = i + 1
  ModelParameters$category[i] = "Eating Carnivory"
  ModelParameters$notes[i] = "Feeding Preference Standard Deviation"; i = i + 1
  ModelParameters$category[i] = "Eating Omnivory"
  ModelParameters$notes[i] = "Max Allowed Prey Ratio Omnivores"; i = i + 1

  # EatingHerbivory_Parameters
  ModelParameters$category[i] = "Eating Herbivory"
  ModelParameters$notes[i] = "Handling Time Scalar Terrestrial"; i = i + 1
  ModelParameters$category[i] = "Eating Herbivory"
  ModelParameters$notes[i] = "Handling Time Scalar Marine (not applicable to current version)"; i = i + 1
  ModelParameters$category[i] = "Eating Herbivory"
  ModelParameters$notes[i] = "Handling Time Exponent Terrestrial"; i = i + 1
  ModelParameters$category[i] = "Eating Herbivory"
  ModelParameters$notes[i] = "Handling Time Exponent Marine (not applicable to current version)"; i = i + 1
  ModelParameters$category[i] = "Eating Herbivory"
  ModelParameters$notes[i] = "Reference Mass"; i = i + 1
  ModelParameters$category[i] = "Eating Herbivory"
  ModelParameters$notes[i] = "Herbivory Rate Constant"; i = i + 1
  ModelParameters$category[i] = "Eating Herbivory"
  ModelParameters$notes[i] = "Herbivory Rate Mass Exponent"; i = i + 1
  ModelParameters$category[i] = "Eating Herbivory"
  ModelParameters$notes[i] = "Attack Rate Exponent Terrestrial"; i = i + 1
  ModelParameters$category[i] = "Eating Herbivory"
  ModelParameters$notes[i] = "Fraction Edible Stock Mass"; i = i + 1

  # MetabolismEctotherm_Parameters
  ModelParameters$category[i] = "Metabolism Ectotherm"
  ModelParameters$notes[i] = "Metabolism Mass Exponent"; i = i + 1
  ModelParameters$category[i] = "Metabolism Ectotherm"
  ModelParameters$notes[i] = "Normalization Constant"; i = i + 1
  ModelParameters$category[i] = "Metabolism Ectotherm"
  ModelParameters$notes[i] = "Activation Energy"; i = i + 1
  ModelParameters$category[i] = "Metabolism Ectotherm"
  ModelParameters$notes[i] = "Boltzmann Constant"; i = i + 1
  ModelParameters$category[i] = "Metabolism Ectotherm"
  ModelParameters$notes[i] = "Normalization Constant BMR"; i = i + 1
  ModelParameters$category[i] = "Metabolism Ectotherm"
  ModelParameters$notes[i] = "Basal Metabolism Mass Exponent"; i = i + 1
  ModelParameters$category[i] = "Metabolism Ectotherm"
  ModelParameters$notes[i] = "Energy Scalar"; i = i + 1

  # MetabolismEndotherm_Parameters
  ModelParameters$category[i] = "Metabolism Endotherm"
  ModelParameters$notes[i] = "Metabolism Mass Exponent"; i = i + 1
  ModelParameters$category[i] = "Metabolism Endotherm"
  ModelParameters$notes[i] = "Normalization Constant"; i = i + 1
  ModelParameters$category[i] = "Metabolism Endotherm"
  ModelParameters$notes[i] = "Activation Energy"; i = i + 1
  ModelParameters$category[i] = "Metabolism Endotherm"
  ModelParameters$notes[i] = "Boltzmann Constant"; i = i + 1
  ModelParameters$category[i] = "Metabolism Endotherm"
  ModelParameters$notes[i] = "Energy Scalar"; i = i + 1
  ModelParameters$category[i] = "Metabolism Endotherm"
  ModelParameters$notes[i] = "Endotherm Body Temperature"; i = i + 1

  # MetabolismHeterotroph_Parameters
  ModelParameters$category[i] = "Metabolism Heterotroph"
  ModelParameters$notes[i] = "Metabolism Mass Exponent"; i = i + 1
  ModelParameters$category[i] = "Metabolism Heterotroph"
  ModelParameters$notes[i] = "Activation Energy"; i = i + 1
  ModelParameters$category[i] = "Metabolism Heterotroph"
  ModelParameters$notes[i] = "Boltzmann Constant"; i = i + 1

  # Mortality_Parameters
  ModelParameters$category[i] = "Mortality"
  ModelParameters$notes[i] = "Background mortailty Rate"; i = i + 1
  ModelParameters$category[i] = "Mortality"
  ModelParameters$notes[i] = "Senescence mortality Rate"; i = i + 1
  ModelParameters$category[i] = "Mortality"
  ModelParameters$notes[i] = "Starvation logistic Inflection Point"; i = i + 1
  ModelParameters$category[i] = "Mortality"
  ModelParameters$notes[i] = "Starvation logistic Scaling Parameter"; i = i + 1
  ModelParameters$category[i] = "Mortality"
  ModelParameters$notes[i] = "Starvation maximum Starvation Rate"; i = i + 1

  # Reproduction_Parameters
  ModelParameters$category[i] = "Reproduction"
  ModelParameters$notes[i] = "Mass ratio Threshold"; i = i + 1
  ModelParameters$category[i] = "Reproduction"
  ModelParameters$notes[i] = "Mass evolution Probability Threshold"; i = i + 1
  ModelParameters$category[i] = "Reproduction"
  ModelParameters$notes[i] = "Mass evolution Standard Deviation"; i = i + 1
  ModelParameters$category[i] = "Reproduction"
  ModelParameters$notes[i] = "Semelparity adult Mass Allocation"; i = i + 1

  # VegetationModel_Parameters
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Calculate Miami NPP, Max NPP"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Calculate Miami NPP, T1NPP"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Calculate Miami NPP, T2NPP"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Calculate Miami NPP, PNPP"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Fraction Structure Scalar"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Calculate Fraction Evergreen A"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Calculate Fraction Evergreen B"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Calculate Fraction Evergreen C"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Evergreen Annual Leaf Mortality Slope "; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Evergreen Annual Leaf Mortality Intercept"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Deciduous Annual Leaf Mortality Slope"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Deciduous Annual Leaf Mortality Intercept"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Fine Root Mortality Rate Slope"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Fine Root Mortality Rate Intercept"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Structural Mortality P2"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Structural Mortality P1"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Leaf Carbon Fixation, MaxFracStruct"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Half Saturation Fire Mortality Rate"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Scalar Fire Mortality Rate"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "NPP Half Saturation Fire Mortality Rate"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "NPP Scalar Fire Mortality Rate"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Min Evergreen Annual Leaf Mortality"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Max Evergreen Annual Leaf Mortality"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Min Deciduous Annual Leaf Mortality"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Max Deciduous Annual Leaf Mortality"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Min Fine Root Mortality Rate"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Max Fine Root Mortality Rate"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Max Structural Mortality"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Min Structural Mortality"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Base Scalar Fire"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Min Return Interval"; i = i + 1
  ModelParameters$category[i] = "Terrestrial Carbon"
  ModelParameters$notes[i] = "Mass Carbon Per Mass Leaf Dry Matter"; i = i + 1

  ModelParameters$category[i] = "HANPP"
  ModelParameters$notes[i] = "0 = off, 1 = fraction raster (0.9 represent 10% reduction), 3 = value raster (gC/m^2/year)"; i = i + 1

  # return ModelParameters
  return(ModelParameters)
}
