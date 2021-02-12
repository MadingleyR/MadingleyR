#include "DispersalResponsive.h"
#include "InputParameters.h"

DispersalResponsive::DispersalResponsive( ) {
    mTimeUnitImplementation = "month"; 
    mDensityThresholdScaling = 50000; //Dispersal_Parameters[2]
    mDispersalSpeedBodyMassScalar = 0.0278; //Dispersal_Parameters[3]
    mDispersalSpeedBodyMassExponent = 0.48; //Dispersal_Parameters[4]
    mStarvationDispersalBodyMassThreshold = 0.8; //Dispersal_Parameters[5]

    UseNonDefaultModelParameters = InputParameters::Get( )->GetUseNonDefaultModelParameters();

    if(UseNonDefaultModelParameters==1){
        Dispersal_Parameters = InputParameters::Get( )->Get_Dispersal_Parameters();
        mDensityThresholdScaling = Dispersal_Parameters[2];
        mDispersalSpeedBodyMassScalar = Dispersal_Parameters[3];
        mDispersalSpeedBodyMassExponent = Dispersal_Parameters[4];
        mStarvationDispersalBodyMassThreshold = Dispersal_Parameters[5];
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

void DispersalResponsive::Run( Grid& gridForDispersal, Cohort* cohortToDisperse, const unsigned& currentMonth ) {
    // Starvation driven dispersal takes precedence over density driven dispersal (i.e. a cohort can't do both). Also, the delta 
    // arrays only allow each cohort to perform one type of dispersal each time step
    bool cohortDispersed = false;

    // Check for starvation-driven dispersal
    cohortDispersed = CheckStarvationDispersal( gridForDispersal, cohortToDisperse );

    if( !cohortDispersed ) {
        // Check for density driven dispersal
        CheckDensityDrivenDispersal( gridForDispersal, cohortToDisperse );
    }
}

bool DispersalResponsive::CheckStarvationDispersal( Grid& gridForDispersal, Cohort* cohortToDisperse ) {
    // A boolean to check whether a cohort has dispersed
    bool cohortHasDispersed = false;

    // Check for starvation driven dispersal
    // What is the present body mass of the cohort?
    // Note that at present we are just tracking starvation for adults
    double individualBodyMass = cohortToDisperse->mIndividualBodyMass;
    double adultMass = cohortToDisperse->mAdultMass;

    // Assume a linear relationship between probability of dispersal and body mass loss, up to _StarvationDispersalBodyMassThreshold
    // at which point the cohort will try to disperse every time step
    if( individualBodyMass < adultMass ) {
        double proportionalPresentMass = individualBodyMass / adultMass;

        // If the body mass loss is greater than the starvation dispersal body mass threshold, then the cohort tries to disperse
        if( proportionalPresentMass < mStarvationDispersalBodyMassThreshold ) {
            // Cohort tries to disperse

            CalculateDispersalProbability( gridForDispersal, cohortToDisperse, CalculateDispersalSpeed( adultMass ) );

            // Note that regardless of whether or not it succeeds, if a cohort tries to disperse, it is counted as having dispersed for the purposes of not then allowing it to disperse
            // based on its density.
            cohortHasDispersed = true;
            // Otherwise, the cohort has a chance of trying to disperse proportional to its mass lass
        } else {

            // Cohort tries to disperse with a particular probability
            // Draw a random number
            //std::uniform_real_distribution<double> randomNumber( 0.0, 1.0 );
            double randomValue = mRandomNumberA.GetUniform( );
            if( ( ( 1.0 - proportionalPresentMass ) / ( 1.0 - mStarvationDispersalBodyMassThreshold ) ) > randomValue ) {

                CalculateDispersalProbability( gridForDispersal, cohortToDisperse, CalculateDispersalSpeed( adultMass ) );
                cohortHasDispersed = true;
            }
        }
    }
    return cohortHasDispersed;
}

void DispersalResponsive::CheckDensityDrivenDispersal( Grid& gridForDispersal, Cohort* cohortToDisperse ) {
    // Check the population density
    double numberOfIndividuals = cohortToDisperse->mCohortAbundance;

    // Get the cell area, in kilometres squared
    double cellArea = cohortToDisperse->mCurrentCell->GetCellArea( );

    // If below the density threshold
    if( ( numberOfIndividuals / cellArea ) < mDensityThresholdScaling / cohortToDisperse->mAdultMass ) {
        // Check to see if it disperses (based on the same movement scaling as used in diffusive movement)
        // Calculate dispersal speed for that cohort
        double dispersalSpeed = CalculateDispersalSpeed( cohortToDisperse->mAdultMass );

        // Cohort tries to disperse
        CalculateDispersalProbability( gridForDispersal, cohortToDisperse, dispersalSpeed );
    }
}

double DispersalResponsive::CalculateDispersalSpeed( double bodyMass ) {
    return mDispersalSpeedBodyMassScalar * pow( bodyMass, mDispersalSpeedBodyMassExponent );
}

void DispersalResponsive::CalculateDispersalProbability( Grid& madingleyGrid, Cohort* c, double dispersalSpeed ) {
    double latCellLength = c->mCurrentCell->GetCellHeight( );
    double lonCellLength = c->mCurrentCell->GetCellWidth( );

    // Pick a direction at random
    //std::uniform_real_distribution<double> randomNumber( 0.0, 1.0 ); // FIX - Real distribution?
    double randomDirection = mRandomNumberA.GetUniform( ) * 2 * acos( -1. );

    // Calculate the u and v components given the dispersal speed
    double uSpeed = dispersalSpeed * cos( randomDirection );
    double vSpeed = dispersalSpeed * sin( randomDirection );

    // Check that the whole cell hasn't moved out (i.e. that dispersal speed is not greater than cell length). 
    // This could happen if dispersal speed was high enough; indicates a need to adjust the time step, or to slow dispersal


    //# temp dispersal fix fix
    if( uSpeed > lonCellLength ) uSpeed = lonCellLength * 0.9;
    if( vSpeed > latCellLength ) vSpeed = latCellLength * 0.9;
    //# temp dispersal fix fix

    if( uSpeed > lonCellLength ) std::cout << "Dispersal Big U " << uSpeed << std::endl;
    if( vSpeed > latCellLength ) std::cout << "Dispersal Big V " << vSpeed << std::endl;

    assert( ( ( uSpeed < lonCellLength ) && ( vSpeed < latCellLength ) ) && "Dispersal probability should always be <= 1" );

    NewCell( madingleyGrid, uSpeed, vSpeed, lonCellLength, latCellLength, c );
}
