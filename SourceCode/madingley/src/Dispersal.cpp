#include "Dispersal.h"

Dispersal::Dispersal( ) {
    ;
}

/** \brief Run the dispersal implementation */
void Dispersal::Run( Grid& gridForDispersal, Cohort* cohortToDisperse, const unsigned& currentMonth ) {
    std::cout << "Called virtual dispersal runner: probably not what you want" << std::endl;
}

void Dispersal::ResetRandom( ) {
    unsigned seed = 14141;
    if( Parameters::Get( )->GetDrawRandomly( ) == true ) {
        seed = std::chrono::system_clock::now( ).time_since_epoch( ).count( );
    }
    mRandomNumberA.Reset( );
    mRandomNumberA.SetSeed( seed );
    mRandomNumberB.Reset( );
}

void Dispersal::NewCell( Grid& madingleyGrid, double& uSpeed, double& vSpeed, double & LonCellLength, double & LatCellLength, Cohort* c ) {
    // Calculate the area of the grid cell that is now outside in the diagonal direction
    // Get the cell area, in kilometres squared
    double CellArea = c->mDestinationCell->GetCellArea( );
    LonCellLength = c->mDestinationCell->GetCellWidth( );
    LatCellLength = c->mDestinationCell->GetCellHeight( );
    //GridCell& g = madingleyGrid.GetACell( c->mDestinationLocation );
    //assert(c->mDestinationCell==&g);
    double AreaOutsideBoth = abs( uSpeed * vSpeed );

    // Calculate the area of the grid cell that is now outside in the u direction (not including the diagonal)
    double AreaOutsideU = abs( uSpeed * LatCellLength ) - AreaOutsideBoth;

    // Calculate the proportion of the grid cell that is outside in the v direction (not including the diagonal
    double AreaOutsideV = abs( vSpeed * LonCellLength ) - AreaOutsideBoth;

    // Convert areas to a probability
    double DispersalProbability = ( AreaOutsideU + AreaOutsideV + AreaOutsideBoth ) / CellArea;
    // Check that we don't have any issues

    //# temp dispersal fix fix
    if( DispersalProbability > 1 ) DispersalProbability = 0.99;
    //# temp dispersal fix fix

    if( DispersalProbability > 1 ) std::cout << "Bad Dispersal Probability " << DispersalProbability << std::endl;
    //assert(DispersalProbability <= 1 && "Dispersal probability should always be <= 1");
    GridCell* DestinationCell = 0;
    // Check to see in which axis the cohort disperses

    // Note that the values in the dispersal array are the proportional area moved outside the grid cell in each direction; we simply compare the random draw to this
    // to determine the direction in which the cohort moves probabilistically
    //std::uniform_real_distribution<double> randomNumber( 0.0, 1.0 );
    //double RandomValue = randomNumber( RandomNumberGenerator );
    double RandomValue = mRandomNumberB.GetUniform( );

    if( DispersalProbability >= RandomValue ) {
        int signu = ( uSpeed > 0 ) - ( uSpeed < 0 );
        int signv = ( vSpeed > 0 ) - ( vSpeed < 0 );
        // Longitudinally
        if( RandomValue <= AreaOutsideU / CellArea ) {
            signv = 0;
        } else {
            //Latitudinally
            if( RandomValue <= ( AreaOutsideU / CellArea + AreaOutsideV / CellArea ) ) {
                signu = 0;
            }
        }
        // try to get a cell.
        GridCell* FreshCell = madingleyGrid.GetNewCell( c->mDestinationCell, signv, signu );
        if( FreshCell != 0 ) {
            DestinationCell = FreshCell;
            Location L = madingleyGrid.GetNewLocation( c->mDestinationLocation, signv, signu );
            c->TryLivingAt( FreshCell, L );
        }
    }
}
