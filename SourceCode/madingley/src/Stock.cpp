#include "Stock.h"
#include "TerrestrialCarbon.h"
//#include "Environment.h"
#include "GridCell.h"
#include "Constants.h"

Stock::Stock( FunctionalGroupDefinitions& stockDefinitions, const unsigned functionalGroup, GridCell& gridCell, bool& success ) {

    mCell = NULL;

    mFunctionalGroupIndex = functionalGroup;
    // Get the individual body masses for organisms in each stock functional group
    mIndividualBodyMass = stockDefinitions.GetBiologicalPropertyOneFunctionalGroup( "individual mass", functionalGroup );

    success = false;

    // If it is a functional group that corresponds to the current realm, then seed the stock
    //if( !gridCell.IsMarine( ) && Environment::Get( "Precipitation", gridCell ) != Constants::cMissingValue && Environment::Get( "Temperature", gridCell ) != Constants::cMissingValue ) {
    if( !gridCell.IsMarine( ) && gridCell.GetGridcellPrecipitation(0) != Constants::cMissingValue && gridCell.GetGridcellNearSurfaceTemp(0) != Constants::cMissingValue ) {
        if( stockDefinitions.GetTraitNames( "Realm", functionalGroup ) == "terrestrial" ) {
            // An instance of the terrestrial carbon model class
            TerrestrialCarbon PlantModel;

            // Calculate predicted leaf mass at equilibrium for this stock
            mTotalBiomass = PlantModel.CalculateEquilibriumLeafMass( gridCell, stockDefinitions.GetTraitNames( "leaf strategy", functionalGroup ) == "deciduous" );
            success = true;
        }
    //} else if( gridCell.IsMarine( ) && Environment::Get( "NPP", gridCell ) != Constants::cMissingValue ) {
    } else if( gridCell.IsMarine( ) && gridCell.GetGridcellTerrestrialNPP(0) != Constants::cMissingValue ) {
        if( stockDefinitions.GetTraitNames( "Realm", functionalGroup ) == "marine" ) {
            mTotalBiomass = 1.e12;
            success = true;
        }
    }
    mCell = &gridCell;
}
