#include "MortalityBackground.h"
#include "InputParameters.h"


MortalityBackground::MortalityBackground( std::string globalModelTimeStepUnit ) {
    mTimeUnitImplementation = "Day";
    mMortailtyRate = 0.001; //Mortality_Parameters[0]

    UseNonDefaultModelParameters = InputParameters::Get( )->GetUseNonDefaultModelParameters();
    
    if(UseNonDefaultModelParameters==1){
        Mortality_Parameters = InputParameters::Get( )->Get_Mortality_Parameters();
        mMortailtyRate = Mortality_Parameters[0];
    }

    // Calculate the scalar to convert from the time step units used by this implementation of mortality to the global model time step units
    mDeltaT = mUtilities.ConvertTimeUnits( globalModelTimeStepUnit, mTimeUnitImplementation );
}

double MortalityBackground::CalculateMortalityRate( Cohort* actingCohort, double bodyMassIncludingChangeThisTimeStep, unsigned currentTimestep ) {
  
    // output rate to console
    // std::cout << "MortalityBackground = " << mMortailtyRate * mDeltaT << std::endl;
    
    // Convert from mortality rate per mortality formulation time step to mortality rate per model time step
    return mMortailtyRate * mDeltaT;
}
