get_cohort_restart_header = function(){
  string_h = "GridcellIndex,FunctionalGroupIndex,JuvenileMass,AdultMass,IndividualBodyMass,CohortAbundance,"
  string_h = paste0(string_h,"LogOptimalPreyBodySizeRatio,BirthTimeStep,ProportionTimeActive,")
  string_h = paste0(string_h,"TrophicIndex,IndividualReproductivePotentialMass,MaturityTimeStep")
  return(string_h)
}
