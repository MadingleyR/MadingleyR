get_default_stock_def = function(){
  structure(list(DEFINITION_Heterotroph.Autotroph = structure(c(1L, 1L, 1L), .Label = "Autotroph", class = "factor"), 
                 DEFINITION_Nutrition.source = structure(c(1L, 1L, 1L), .Label = "Photosynthesis", class = "factor"), 
                 DEFINITION_Diet = c(NA, NA, NA), DEFINITION_Realm = structure(c(1L, 2L, 2L), .Label = c("Marine", "Terrestrial"), class = "factor"), 
                 DEFINITION_Mobility = structure(c(1L, 2L, 2L), .Label = c("Planktonic", "Sessile"), class = "factor"), 
                 DEFINITION_Leaf.strategy = structure(c(3L, 1L, 2L), .Label = c("Deciduous", "Evergreen", "na"), class = "factor"), 
                 PROPERTY_Herbivory.assimilation = c(NA, NA, NA), 
                 PROPERTY_Carnivory.assimilation = c(NA, NA, NA), 
                 PROPERTY_Proportion.herbivory = c(NA, NA, NA), 
                 PROPERTY_Individual.mass = c(1e-04, 0, 0)), 
            class = "data.frame", row.names = c(NA, -3L))
  
}
