get_input_header = function(n){
  headers = c(
  "DEFINITION_Heterotroph/Autotroph,DEFINITION_Nutrition source,DEFINITION_Diet,DEFINITION_Realm,DEFINITION_Mobility,DEFINITION_Reproductive strategy,DEFINITION_Endo/Ectotherm,PROPERTY_Herbivory assimilation,PROPERTY_Carnivory assimilation,PROPERTY_Proportion suitable time active,PROPERTY_Minimum mass,PROPERTY_Maximum mass,PROPERTY_Initial number of GridCellCohorts,NOTES_group description",
  "Mass bin lower bound",
  "Parameter,Value",
  "DEFINITION_Heterotroph/Autotroph,DEFINITION_Nutrition source,DEFINITION_Diet,DEFINITION_Realm,DEFINITION_Mobility,DEFINITION_Leaf strategy,PROPERTY_Herbivory assimilation,PROPERTY_Carnivory assimilation,PROPERTY_Proportion herbivory,PROPERTY_Individual mass"
  )
  return(headers[n])
}