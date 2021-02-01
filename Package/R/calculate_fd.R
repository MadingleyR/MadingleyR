calculate_fd = function(madingley_data_input,traits_select = c("JuvenileMass","AdultMass","IndividualBodyMass","AgeMonths",
                                                "DEFINITION_Nutrition.source","DEFINITION_Endo.Ectotherm",
                                                "TrophicIndex"),sample_size=500){
  
  if(!"FD" %in% rownames(installed.packages())){
    stop("package named 'FD' not installed")
  }
  require(FD)
  
  
  # prepare traits
  if(class(madingley_data_input)=="list") {
    cohort = madingley_data_input$cohorts
    add_traits = madingley_data_input$cohort_def[cohort$FunctionalGroupIndex+1,]
    traits = cbind(cohort,add_traits)
    row.names(traits) = paste0("sp_",1:nrow(cohort))
    traits = traits[,traits_select]
  }else{
    cohort = madingley_data_input
    traits = cohort
    row.names(traits) = paste0("sp_",1:nrow(cohort))
    #print(names(traits))
    #print(traits_select)
    traits = traits[,traits_select]
  }

  
  # prepare abundances
  abundances = round(cohort$AdultMass)
  names(abundances) = paste0("sp_",1:nrow(cohort))
  
  # sample to reduce computation time
  sampled = sample(1:nrow(cohort),sample_size)
  print(sampled)
  abundances = abundances[sampled]
  traits = traits[sampled,]
  
  # compute fd
  fd = dbFD(traits, abundances, messages = F, m = "min")
  return(fd)
}