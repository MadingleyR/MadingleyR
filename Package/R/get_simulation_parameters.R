get_simulation_parameters = function(){
  structure(list(
    Parameter = structure(c(13L, 14L, 21L, 23L, 10L, 15L, 5L, 4L, 11L, 2L, 8L, 6L, 7L, 1L, 9L, 12L, 22L, 19L, 3L, 20L, 16L, 18L, 17L), 
                          .Label = c("BurninSteps", "DrawRandomly", "EndBodyMass", "ExtinctionThreshold", "GridCellSize", "HumanNPPExtractionScale", 
                                     "HumanNPPScenarioDuration", "HumanNPPScenarioType", "ImpactSteps", "LengthOfSimulationInYears", "MaximumNumberOfCohorts", 
                                     "RecoverySteps", "RootDataDirectory", "RunParallel", "RunUntilStable", "SelectCarnivores", "SelectHerbivores", "SelectOmnivores", 
                                     "StartBodyMass", "StepBodyMass", "ThreadNumber", "TimeStepStartExtinction", "TimeStepUnits"), class = "factor"), 
     Value = structure(c(1L, 3L, 9L, 11L, 6L, 3L, 3L, 3L, 4L, 13L, 12L, 2L, 2L, 2L, 2L, 2L, 5L, 10L, 8L, 7L, 3L, 2L, 2L), 
                       .Label = c("../MadingleyData-master/NETCDF/", "0", "1", "1000", "1200", "2", "2000", "21000", "24", 
                                  "25000", "month", "none", "yes"), class = "factor")), 
    class = "data.frame", row.names = c(NA, -23L))
}
