#include "AutotrophProcessor.h"

AutotrophProcessor::AutotrophProcessor( ) {
    mPhytoplanktonConversionRatio = 10;
    mMsqToKmSqConversion = 1000000;
}

void AutotrophProcessor::ConvertNPPToAutotroph( GridCell& gcl, Stock& actingStock, unsigned currentMonth ) {
    // Get gridcell NPP 
    double NPP = gcl.GetGridcellTerrestrialNPP(currentMonth); // Environment::Get( "NPP", gcl );
    // If NPP is a missing value then set to zero
    if( NPP == Constants::cMissingValue ) NPP = 0.0;

    // Check that this is an ocean cell
    if( gcl.IsMarine( ) ) {
        // Check that the units of oceanic NPP are gC per m2 per day
        //assert(params.Units["OceanNPP"] == "gC/m2/day" && "Oceanic NPP data are not in the correct units for this formulation of the model");

        //Convert to g/cell/month
        NPP *= mMsqToKmSqConversion;

        //Multiply by cell area to get g/cell/day
        NPP *= gcl.GetCellArea( );

        //Convert to g wet matter, assuming carbon content of phytoplankton is 10% of wet matter
        NPP *= mPhytoplanktonConversionRatio;

        //Finally convert to g/cell/month and add to the stock totalbiomass
        NPP *= mUtilities.ConvertTimeUnits( Parameters::Get( )->GetTimeStepUnits( ), "day" );

        actingStock.mTotalBiomass += NPP;

        // If the biomass of the autotroph stock has been made less than zero (i.e. because of negative NPP) then reset to zero
        if( actingStock.mTotalBiomass < 0.0 )
            actingStock.mTotalBiomass = 0.0;
    }// Else if neither on land or in the ocean
    else {
        std::cout << "This is not a marine cell!" << std::endl;
        // Set the autotroph biomass to zero
        actingStock.mTotalBiomass = 0.0;
    }
    assert( actingStock.mTotalBiomass >= 0.0 && "stock negative" );
}
