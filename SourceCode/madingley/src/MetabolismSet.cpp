#include "MetabolismSet.h"

MetabolismSet::MetabolismSet( std::string globalModelTimeStepUnit ) {
    // Add the basic endotherm metabolism implementation to the list of implementations
    MetabolismEndotherm* MetabolismEndothermImplementation = new MetabolismEndotherm( globalModelTimeStepUnit );
    Implementations[ "basic endotherm" ] = MetabolismEndothermImplementation;

    // Add the basic ectotherm metabolism implementation to the list of implementations
    MetabolismEctotherm* MetabolismEctothermImplementation = new MetabolismEctotherm( globalModelTimeStepUnit );
    Implementations[ "basic ectotherm" ] = MetabolismEctothermImplementation;
}

MetabolismSet::~MetabolismSet( ) {
    delete Implementations[ "basic endotherm" ];
    delete Implementations[ "basic ectotherm" ];
}

void MetabolismSet::InitializeEcologicalProcess( GridCell& gcl, MadingleyInitialisation& params, std::string implementationKey ) {

}

void MetabolismSet::RunEcologicalProcess( GridCell& gcl, Cohort* actingCohort, unsigned currentTimestep, ThreadVariables& partial,
  unsigned currentMonth, MadingleyInitialisation& params ) {
    if( params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Heterotroph/Autotroph", actingCohort->mFunctionalGroupIndex ) == "heterotroph" ) {
        if( params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Endo/Ectotherm", actingCohort->mFunctionalGroupIndex ) == "endotherm" ) {
            Implementations[ "basic endotherm" ]->Run( actingCohort, currentTimestep, currentMonth );
        } else {
            Implementations[ "basic ectotherm" ]->Run( actingCohort, currentTimestep, currentMonth );
        }
    }
}
