get_default_cohort_def = function(){
  df = structure(list(DEFINITION_Heterotroph.Autotroph = structure(c(1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L), .Label = "Heterotroph", class = "factor"),
                 DEFINITION_Nutrition.source = structure(c(3L, 1L, 1L, 1L, 2L, 2L, 3L, 3L, 2L, 2L, 2L, 1L, 3L, 2L, 1L, 3L, 2L, 1L, 3L), .Label = c("Carnivore", "Herbivore", "Omnivore"), class = "factor"), 
                 DEFINITION_Diet = structure(c(2L, 1L, 1L, 1L, 3L, 3L, 1L, 1L, 3L, 3L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L), .Label = c("All", "AllSpecial", "Planktivore"), class = "factor"), 
                 DEFINITION_Realm = structure(c(1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 2L, 2L, 2L, 2L, 2L, 2L, 2L, 2L, 2L), .Label = c("Marine", "Terrestrial"), class = "factor"), 
                 DEFINITION_Mobility = structure(c(1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 2L, 2L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L), .Label = c("Mobile", "Planktonic"), class = "factor"), 
                 DEFINITION_Reproductive.strategy = structure(c(1L, 1L, 1L, 2L, 1L, 2L, 1L, 2L, 1L, 2L, 1L, 1L, 1L, 2L, 2L, 2L, 1L, 1L, 1L), .Label = c("iteroparity", "semelparity"), class = "factor"), 
                 DEFINITION_Endo.Ectotherm = structure(c(2L, 2L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 2L, 2L, 2L, 1L, 1L, 1L, 1L, 1L, 1L), .Label = c("Ectotherm", "Endotherm"), class = "factor"), 
                 PROPERTY_Herbivory.assimilation = c(0, 0, 0, 0, 0.7, 0.7, 0.6, 0.6, 0.7, 0.7, 0.5, 0, 0.38, 0.5, 0, 0.36, 0.5, 0, 0.36), 
                 PROPERTY_Carnivory.assimilation = c(0.8, 0.8, 0.8, 0.8, 0, 0, 0.64, 0.64, 0, 0, 0, 0.8, 0.64, 0, 0.8, 0.64, 0, 0.8, 0.64), 
                 PROPERTY_Proportion.suitable.time.active = c(0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5), 
                 PROPERTY_Minimum.mass = c(10000, 100, 1e-04, 1e-04, 1e-04, 1e-04, 1e-05, 1e-05, 1e-05, 1e-05, 1, 5, 5, 0.04, 0.08, 0.04, 1, 1.5, 1.5), 
                 PROPERTY_Maximum.mass = c(1.5e+08, 5e+07, 2e+06, 2e+06, 10000, 10000, 1e+05, 1e+05, 0.1, 0.1, 7e+06, 8e+05, 150000, 500, 2000, 2000, 100000, 100000, 55000), 
                 PROPERTY_Initial.number.of.GridCellCohorts = c(0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 50, 50L, 50L, 50L, 50L, 50L, 50L, 50L, 50L), 
                 NOTES_group.description = structure(c(1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L, 1L), .Label = "None", class = "factor")), 
            class = "data.frame", row.names = c(NA, -19L))
  df = df[df$DEFINITION_Realm=="Terrestrial",]
  rownames(df) = NULL
  return(df)
}