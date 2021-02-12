#include "DispersalAdvective.h"

DispersalAdvective::DispersalAdvective( ) {

    mTimeUnitImplementation = "month";
    mHorizontalDiffusivity = 100;
    mAdvectiveModelTimeStepLengthHours = 18;
    mHorizontalDiffusivityKmSqPerADTimeStep = mHorizontalDiffusivity / ( 1000 * 1000 ) * 60 * 60 * mAdvectiveModelTimeStepLengthHours;

    // Calculate the scalar to convert from the time step units used by this implementation of dispersal to the global model time step units
    mDeltaT = mUtilities.ConvertTimeUnits( Parameters::Get( )->GetTimeStepUnits( ), mTimeUnitImplementation );

    // Initialise the advective dispersal temporal scaling to adjust between time steps appropriately
    mAdvectionTimeStepsPerModelTimeStep = mUtilities.ConvertTimeUnits( Parameters::Get( )->GetTimeStepUnits( ), "day" ) * 24 / mAdvectiveModelTimeStepLengthHours;

    // Convert velocity from m/s to km/month. Note that if the _TimeUnitImplementation changes, this will also have to change.
    mVelocityUnitConversion = 60 * 60 * 24 * mUtilities.ConvertTimeUnits( Parameters::Get( )->GetTimeStepUnits( ), "day" ) * mDeltaT / 1000;

    // Set the seed for the random number generator
    unsigned seed = 14141;
    if( Parameters::Get( )->GetDrawRandomly( ) == true ) {
        seed = std::chrono::system_clock::now( ).time_since_epoch( ).count( );
    }
    mRandomNumberA.SetSeed( seed );

}

void DispersalAdvective::Run( Grid& gridForDispersal, Cohort* cohortToDisperse, const unsigned& currentMonth ) {
    // Loop through a number of times proportional to the rescaled dispersal
    for( int mm = 0; mm < mAdvectionTimeStepsPerModelTimeStep; mm++ ) {
        // Get the probability of dispersal and return a candidate cell
        CalculateDispersalProbability( gridForDispersal, cohortToDisperse, currentMonth );
    }
}

inline const double DispersalAdvective::RescaleDispersalSpeed( const double& dispersalSpeed ) const {
    // FIX - Units are metres per second; need to convert to kilometres per global time step (currently one month) - use VelocityUnitConversion for this.
    // FIX - Also rescale based on the time step of the advective dispersal model - currently 18h
    return dispersalSpeed * mVelocityUnitConversion / mAdvectionTimeStepsPerModelTimeStep;
}

void DispersalAdvective::CalculateDispersalProbability( Grid& madingleyGrid, Cohort* c, const unsigned& currentMonth ) {
    // Advective speed in u (longitudinal) direction
    double uAdvectiveSpeed;

    // Advective speed in v (latitudinal) direction
    double vAdvectiveSpeed;

    // Distance travelled in u (longitudinal) direction
    double uDistanceTravelled;

    // Distance travelled in v (latitudinal) direction
    double vDistanceTravelled;

    // U and V components of the diffusive velocity
    Types::DoubleVector diffusiveUandVComponents( 2 );

    // Length in km of a cell boundary latitudinally
    double latCellLength;

    // Length in km of a cell boundary longitudinally
    double lonCellLength;

    // Get the u speed and the v speed from the cell data
    uAdvectiveSpeed = 0.0; //Environment::Get( "uVel", *( c->mDestinationCell ) );
    assert( uAdvectiveSpeed > -9999 );

    vAdvectiveSpeed = 0.0; //Environment::Get( "vVel", *( c->mDestinationCell ) );
    assert( vAdvectiveSpeed > -9999 );
    // Calculate the diffusive movement speed, with a direction chosen at random
    diffusiveUandVComponents = CalculateDiffusion( );
    // Calculate the distance travelled in this dispersal (not global) time step. both advective and diffusive speeds need to have been converted to km / advective model time step
    uDistanceTravelled = RescaleDispersalSpeed( uAdvectiveSpeed ) + diffusiveUandVComponents[0];
    vDistanceTravelled = RescaleDispersalSpeed( vAdvectiveSpeed ) + diffusiveUandVComponents[1];

    // Check that the u distance travelled and v distance travelled are not greater than the cell length
    latCellLength = c->mDestinationCell->GetCellHeight( );
    lonCellLength = c->mDestinationCell->GetCellWidth( );

    //# temp dispersal fix fix
    if( abs( uDistanceTravelled ) > lonCellLength ) uDistanceTravelled = lonCellLength * 0.9;
    if( abs( vDistanceTravelled ) > latCellLength ) vDistanceTravelled = latCellLength * 0.9; 
    //# temp dispersal fix fix


    if( abs( uDistanceTravelled ) > lonCellLength ) std::cout << "BIG U " << uAdvectiveSpeed << std::endl;
    assert( abs( uDistanceTravelled ) <= lonCellLength && "u velocity greater than cell width" );
    assert( abs( vDistanceTravelled ) <= latCellLength && "v velocity greater than cell width" );
    NewCell( madingleyGrid, uDistanceTravelled, vDistanceTravelled, lonCellLength, latCellLength, c );

}

Types::DoubleVector DispersalAdvective::CalculateDiffusion( ) {
    // Create the array with which to send the output
    Types::DoubleVector outputsUandV( 2 );

    // Note that this formulation drops the delta t because we set the horizontal diffusivity to be at the same temporal
    // scale as the time step

    outputsUandV[ 0 ] = mRandomNumberA.GetNormal( ) * sqrt( ( 2.0 * mHorizontalDiffusivityKmSqPerADTimeStep ) );
    outputsUandV[ 1 ] = mRandomNumberA.GetNormal( ) * sqrt( ( 2.0 * mHorizontalDiffusivityKmSqPerADTimeStep ) );

    return outputsUandV;
}
