#include "MortalitySenescence.h"
#include "InputParameters.h"

/** \brief Constructor for senscence mortality: assigns all parameter values */
MortalitySenescence::MortalitySenescence( std::string globalModelTimeStepUnit ) {
    mTimeUnitImplementation = "Day";
    mMortalityRate = 0.003;  //Mortality_Parameters[1]

    UseNonDefaultModelParameters = InputParameters::Get( )->GetUseNonDefaultModelParameters();

    if(UseNonDefaultModelParameters==1){
        Mortality_Parameters = InputParameters::Get( )->Get_Mortality_Parameters();
        mMortalityRate = Mortality_Parameters[1];
    }

    // Calculate the scalar to convert from the time step units used by this implementation of mortality to the global model time step units
    mDeltaT = mUtilities.ConvertTimeUnits( globalModelTimeStepUnit, mTimeUnitImplementation );
}

double MortalitySenescence::CalculateMortalityRate( Cohort* actingCohort, double bodyMassIncludingChangeThisTimeStep, unsigned currentTimestep ) {
    
    double TimeToMaturity;
    double AgePostMaturity;
    TimeToMaturity = 1.0 * actingCohort->mTimeStepsJuviline;
    AgePostMaturity = 1.0 * actingCohort->mTimeStepsAdult;

    // // Calculate the age (in model time steps) that the cohort reached maturity
    // TimeToMaturity = actingCohort->mMaturityTimeStep - actingCohort->mBirthTimeStep;

    // // Calculate how many model time steps since the cohort reached maturity
    // AgePostMaturity = currentTimestep - actingCohort->mMaturityTimeStep;
    
    // Calculate the time since maturity as a fraction of the time that it took the cohort to reach maturity
    double FractionalAgePostMaturity = AgePostMaturity / ( TimeToMaturity + 1 );

    // Calculate the mortality rate per mortality formulation time step as a function of the exponential of the previous fraction
    double AgeRelatedMortalityRate = mMortalityRate * exp( FractionalAgePostMaturity );

    // output rate to console
    // std::cout << "MortalitySenescence = " << AgeRelatedMortalityRate * mDeltaT << std::endl;
    
    // Convert the mortality rate from formulation time step units to model time step units
    return AgeRelatedMortalityRate * mDeltaT;
}
