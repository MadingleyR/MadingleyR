#include "DispersalDiffusive.h"
#include "InputParameters.h"

DispersalDiffusive::DispersalDiffusive( ) {
    mTimeUnitImplementation = "month";
    mDispersalSpeedBodyMassScalar = 0.0278; //Dispersal_Parameters[0]
    mDispersalSpeedBodyMassExponent = 0.48; //Dispersal_Parameters[1]

    UseNonDefaultModelParameters = InputParameters::Get( )->GetUseNonDefaultModelParameters();
   

    if(UseNonDefaultModelParameters==1){
        Dispersal_Parameters = InputParameters::Get( )->Get_Dispersal_Parameters();
        mDispersalSpeedBodyMassScalar = Dispersal_Parameters[0];
        mDispersalSpeedBodyMassExponent = Dispersal_Parameters[1];
    }

    // Calculate the scalar to convert from the time step units used by this implementation of dispersal to the global model time step units
    mDeltaT = mUtilities.ConvertTimeUnits( Parameters::Get( )->GetTimeStepUnits( ), mTimeUnitImplementation );

    // Set the seed for the random number generator
    unsigned seed = 14141;
    if( Parameters::Get( )->GetDrawRandomly( ) == true ) {
        seed = std::chrono::system_clock::now( ).time_since_epoch( ).count( );
    }
    mRandomNumberA.SetSeed( seed );
}

void DispersalDiffusive::Run( Grid& gridForDispersal, Cohort* cohortToDisperse, const unsigned& currentMonth ) {
    // Calculate dispersal speed for the cohort         
    double dispersalSpeed = CalculateDispersalSpeed( cohortToDisperse->mIndividualBodyMass );

    CalculateDispersalProbability( gridForDispersal, cohortToDisperse, dispersalSpeed );

}

double DispersalDiffusive::CalculateDispersalSpeed( double bodyMass ) {
    return mDispersalSpeedBodyMassScalar * pow( bodyMass, mDispersalSpeedBodyMassExponent );
}

void DispersalDiffusive::CalculateDispersalProbability( Grid& madingleyGrid, Cohort* cohort, double dispersalSpeed ) {
    // Check that the u speed and v speed are not greater than the cell length. If they are, then rescale them; this limits the max velocity
    // so that cohorts cannot be advected more than one grid cell per time step
    double latCellLength = cohort->mCurrentCell->GetCellHeight( );
    double lonCellLength = cohort->mCurrentCell->GetCellWidth( );

    // Pick a direction at random
    double randomDirection = mRandomNumberA.GetUniform( )* 2 * acos( -1. );


    // Calculate the u and v components given the dispersal speed
    double uSpeed = dispersalSpeed * cos( randomDirection );
    double vSpeed = dispersalSpeed * sin( randomDirection );
    NewCell( madingleyGrid, uSpeed, vSpeed, lonCellLength, latCellLength, cohort );
}

