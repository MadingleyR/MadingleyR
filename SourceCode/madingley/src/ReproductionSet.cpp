#include "ReproductionSet.h"
#include "Reproduction.h"

/**  \brief Constructor for Reproduction: fills the list of available implementations of reproduction */
ReproductionSet::ReproductionSet( std::string globalModelTimeStepUnit, bool drawRandomly ) {
    // Add the basic reproduction implementation to the list of implementations
    ReproductionBasic* ReproductionImplementation = new ReproductionBasic( globalModelTimeStepUnit, drawRandomly );
    mImplementations["reproduction basic"] = ReproductionImplementation;
}

/** Destructor ensure we tidy everything up */
ReproductionSet::~ReproductionSet( ) {
    delete mImplementations["reproduction basic"];
}

void ReproductionSet::InitializeEcologicalProcess( GridCell& gcl, MadingleyInitialisation& params, std::string implementationKey ) {

}

void ReproductionSet::RunEcologicalProcess( GridCell& gcl, Cohort* actingCohort, unsigned currentTimestep, ThreadVariables& partial,
  unsigned currentMonth, MadingleyInitialisation& params ) {
    // Holds the reproductive strategy of a cohort
    bool _Iteroparous = ( params.mCohortFunctionalGroupDefinitions.GetTraitNames( "reproductive strategy", actingCohort->mFunctionalGroupIndex ) == "iteroparity" );
    // Assign mass to reproductive potential
    mImplementations["reproduction basic"]->MassAssignment( gcl, actingCohort, currentTimestep, params );
    // Run reproductive events. Note that we can't skip juveniles here as they could conceivably grow to adulthood and get enough biomass to reproduce in a single time step
    // due to other ecological processes
    mImplementations["reproduction basic"]->Run( gcl, actingCohort, currentTimestep, partial, _Iteroparous, currentMonth, params );
}
