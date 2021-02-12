#ifndef REPRODUCTION
#define REPRODUCTION

#include "MadingleyInitialisation.h"
#include "ThreadVariables.h"

#include <map>

/** \brief Interface for implementations of the ecological process of reproduction */
class Reproduction {
public:

    /** \brief Generate new cohorts from reproductive potential mass
    @param gridCell The current grid cell 
    @param actingCohort The position of the acting cohort in the jagged array of grid cell cohorts 
    @param currentTimestep The current model time step 
    @param partial Thread-locked variables 
    @param iteroparous Whether the acting cohort is iteroparous, as opposed to semelparous 
    @param currentMonth The current model month */
    virtual void Run( GridCell&, Cohort*, unsigned, ThreadVariables&, bool, unsigned, MadingleyInitialisation& ) {
        std::cout << "ReproductionImplementation RunReproductionEvents should be virtual: you probably don't want to be here" << std::endl;
    }

    /** \brief Assigns surplus body mass to reproductive potential mass
    @param gridCell The current grid cell 
    @param actingCohort The position of the acting cohort in the jagged array of grid cell cohorts 
    @param currentTimestep The current model time step 
    @param trackProcesses An instance of ProcessTracker to hold diagnostics for reproduction */
    virtual void MassAssignment( GridCell&, Cohort*, unsigned, MadingleyInitialisation& ) {
        std::cout << "IReproductionImplementation RunReproductiveMassAssignment should be virtual: you probably don't want to be here" << std::endl;
    }
};
#endif
