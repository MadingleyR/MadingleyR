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
                                  0.001,2.0,0.00000226032940698105,0.476,1.0)
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
  ModelParameters$notes = NA
  i = 1

  # Activity_Parameters
  ModelParameters$notes[i] = "Activity: Terrestrial Warming Tolerance Intercept"; i = i + 1
  ModelParameters$notes[i] = "Activity: Terrestrial Warming Tolerance Slope"; i = i + 1
  ModelParameters$notes[i] = "Activity: Terrestrial TSM Intercept"; i = i + 1
  ModelParameters$notes[i] = "Activity: Terrestrial TSM Slope"; i = i + 1

  # Dispersal_Parameters
  ModelParameters$notes[i] = "Diffusive Dispersal: Speed Body Mass Scalar"; i = i + 1
  ModelParameters$notes[i] = "Diffusive Dispersal: Speed Body Mass Exponent"; i = i + 1
  ModelParameters$notes[i] = "Responsive Dispersal: Density Threshold Scaling"; i = i + 1
  ModelParameters$notes[i] = "Responsive Dispersal: Speed Body Mass Scalar"; i = i + 1
  ModelParameters$notes[i] = "Responsive Dispersal: Speed Body Mass Exponent"; i = i + 1
  ModelParameters$notes[i] = "Responsive Dispersal: Starvation Dispersal Body Mass Threshold"; i = i + 1

  # EatingCarnivory_Parameters
  ModelParameters$notes[i] = "Eating Carnivory: Handling Time Scalar Terrestrial"; i = i + 1
  ModelParameters$notes[i] = "Eating Carnivory: Handling Time Exponent Terrestrial"; i = i + 1
  ModelParameters$notes[i] = "Eating Carnivory: Handling Time Scalar Marine (not applicable to current version)"; i = i + 1
  ModelParameters$notes[i] = "Eating Carnivory: Handling Time Exponent Marine (not applicable to current version)"; i = i + 1
  ModelParameters$notes[i] = "Eating Carnivory: Referenc eMass"; i = i + 1
  ModelParameters$notes[i] = "Eating Carnivory: Kill Rate Constant"; i = i + 1
  ModelParameters$notes[i] = "Eating Carnivory: Kill Rate Constant Mass Exponent"; i = i + 1
  ModelParameters$notes[i] = "Eating Carnivory: Feeding Preference Standard Deviation"; i = i + 1
  ModelParameters$notes[i] = "Eating Omnivory: Max Allowed Prey Ratio Omnivores"; i = i + 1

  # EatingHerbivory_Parameters
  ModelParameters$notes[i] = "Eating Herbivory: Handling Time Scalar Terrestrial"; i = i + 1
  ModelParameters$notes[i] = "Eating Herbivory: Handling Time Scalar Marine (not applicable to current version)"; i = i + 1
  ModelParameters$notes[i] = "Eating Herbivory: Handling Time Exponent Terrestrial"; i = i + 1
  ModelParameters$notes[i] = "Eating Herbivory: Handling Time Exponent Marine (not applicable to current version)"; i = i + 1
  ModelParameters$notes[i] = "Eating Herbivory: Reference Mass"; i = i + 1
  ModelParameters$notes[i] = "Eating Herbivory: Herbivory Rate Constant"; i = i + 1
  ModelParameters$notes[i] = "Eating Herbivory: Herbivory Rate Mass Exponent"; i = i + 1
  ModelParameters$notes[i] = "Eating Herbivory: Attack Rate Exponent Terrestrial"; i = i + 1
  ModelParameters$notes[i] = "Eating Herbivory: Fraction Edible Stock Mass"; i = i + 1

  # MetabolismEctotherm_Parameters
  ModelParameters$notes[i] = "Metabolism Ectotherm: Metabolism Mass Exponent"; i = i + 1
  ModelParameters$notes[i] = "Metabolism Ectotherm: Normalization Constant"; i = i + 1
  ModelParameters$notes[i] = "Metabolism Ectotherm: Activation Energy"; i = i + 1
  ModelParameters$notes[i] = "Metabolism Ectotherm: Boltzmann Constant"; i = i + 1
  ModelParameters$notes[i] = "Metabolism Ectotherm: Normalization Constant BMR"; i = i + 1
  ModelParameters$notes[i] = "Metabolism Ectotherm: Basal Metabolism Mass Exponent"; i = i + 1
  ModelParameters$notes[i] = "Metabolism Ectotherm: Energy Scalar"; i = i + 1

  # MetabolismEndotherm_Parameters
  ModelParameters$notes[i] = "Metabolism Endotherm: Metabolism Mass Exponent"; i = i + 1
  ModelParameters$notes[i] = "Metabolism Endotherm: Normalization Constant"; i = i + 1
  ModelParameters$notes[i] = "Metabolism Endotherm: Activation Energy"; i = i + 1
  ModelParameters$notes[i] = "Metabolism Endotherm: Boltzmann Constant"; i = i + 1
  ModelParameters$notes[i] = "Metabolism Endotherm: Energy Scalar"; i = i + 1
  ModelParameters$notes[i] = "Metabolism Endotherm: Endotherm Body Temperature"; i = i + 1

  # MetabolismHeterotroph_Parameters
  ModelParameters$notes[i] = "Metabolism Heterotroph: Metabolism Mass Exponent"; i = i + 1
  ModelParameters$notes[i] = "Metabolism Heterotroph: Activation Energy"; i = i + 1
  ModelParameters$notes[i] = "Metabolism Heterotroph: Boltzmann Constant"; i = i + 1

  # Mortality_Parameters
  ModelParameters$notes[i] = "Mortality Background: Mortailty Rate"; i = i + 1
  ModelParameters$notes[i] = "Mortality Senescence: Mortality Rate"; i = i + 1
  ModelParameters$notes[i] = "Mortality Starvation: Logistic Inflection Point"; i = i + 1
  ModelParameters$notes[i] = "Mortality Starvation: Logistic Scaling Parameter"; i = i + 1
  ModelParameters$notes[i] = "Mortality Starvation: Maximum Starvation Rate"; i = i + 1

  # Reproduction_Parameters
  ModelParameters$notes[i] = "Reproduction: Mass Ratio Threshold"; i = i + 1
  ModelParameters$notes[i] = "Reproduction: Mass Evolution Probability Threshold"; i = i + 1
  ModelParameters$notes[i] = "Reproduction: Mass Evolution Standard Deviation"; i = i + 1
  ModelParameters$notes[i] = "Reproduction: Semelparity Adult Mass Allocation"; i = i + 1

  # VegetationModel_Parameters
  ModelParameters$notes[i] = "Terrestrial Carbon: Calculate Miami NPP, Max NPP"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Calculate Miami NPP, T1NPP"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Calculate Miami NPP, T2NPP"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Calculate Miami NPP, PNPP"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Fraction Structure Scalar"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Calculate Fraction Evergreen A"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Calculate Fraction Evergreen B"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Calculate Fraction Evergreen C"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Evergreen Annual Leaf Mortality Slope "; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Evergreen Annual Leaf Mortality Intercept"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Deciduous Annual Leaf Mortality Slope"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Deciduous Annual Leaf Mortality Intercept"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Fine Root Mortality Rate Slope"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Fine Root Mortality Rate Intercept"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Structural Mortality P2"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Structural Mortality P1"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Leaf Carbon Fixation, MaxFracStruct"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Half Saturation Fire Mortality Rate"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Scalar Fire Mortality Rate"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: NPP Half Saturation Fire Mortality Rate"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: NPP Scalar Fire Mortality Rate"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Min Evergreen Annual Leaf Mortality"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Max Evergreen Annual Leaf Mortality"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Min Deciduous Annual Leaf Mortality"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Max Deciduous Annual Leaf Mortality"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Min Fine Root Mortality Rate"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Max Fine Root Mortality Rate"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Max Structural Mortality"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Min Structural Mortality"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Base Scalar Fire"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Min Return Interval"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Mass Carbon Per Mass Leaf Dry Matter"; i = i + 1
  ModelParameters$notes[i] = "Terrestrial Carbon: Hnpp reduction fraction, lower vegetation production available to herbivores"; i = i + 1

  # return ModelParameters
  return(ModelParameters)
}
