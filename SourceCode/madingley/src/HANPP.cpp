#include "HANPP.h"

/** \file HANPP.cpp
 * \brief the HANPP implemetation file
 */

    HumanAutotrophMatterAppropriation::HumanAutotrophMatterAppropriation( ) {
    }

    //----------------------------------------------------------------------------------------------
    double HumanAutotrophMatterAppropriation::RemoveHumanAppropriatedMatter(
           double NPPWetMatter, 
           GridCell& gcl, 
           Stock& actingStock, 
           unsigned currentTimestep, 
           unsigned currentMonth, 
           bool fractionalHANPPInput ) {
        
        //return value
        double RemovalRate=0;
        
        if(fractionalHANPPInput){
            
            RemovalRate = gcl.GetGridcellHanpp();
            
        }else{
        
            // Factor to convert NPP from units per m2 to units per km2
            double m2Tokm2Conversion = 1000000.0;
    

    
            // Loop over stocks in the grid cell and calculate the total biomass of all stocks
            double TotalAutotrophBiomass = 0.0;
            gcl.ApplyFunctionToAllStocks( [&](Stock& s) {
                TotalAutotrophBiomass += s.mTotalBiomass;
            });
            
            // Get the total amount of NPP appropriated by humans from this cell
            //double HANPP = Environment::Get( "TerrestrialHANPP", gcl ) * Environment::Get( "Seasonality", gcl );
            double HANPP = gcl.GetGridcellHanpp() * gcl.GetGridcellSeasonality( currentMonth );
    
            // If HANPP value is missing, then assume zero
            if( HANPP == Constants::cMissingValue ) HANPP = 0.0;
    
            // Allocate HANPP for this stock according to the proportion of total autotroph biomass that the stock represents
            if( TotalAutotrophBiomass == 0.0 ) {
                HANPP = 0.0;
            } else {
                HANPP *= ( actingStock.mTotalBiomass / TotalAutotrophBiomass );
            }
    
    
            // Convert gC/m2/month to gC/km2/month
            HANPP *= m2Tokm2Conversion;
    
            // Multiply by cell area (in km2) to get g/cell/day
            HANPP *= gcl.GetCellArea( );
    
            // Convert from gC to g dry matter
            double DryMatterAppropriated = HANPP * 2;
    
            // Convert from g dry matter to g wet matter
            double WetMatterAppropriated = DryMatterAppropriated * 2;
            //Calculate the rate of HANPP offtake
            if (NPPWetMatter <= 0.0){
                    RemovalRate = 0.0;
            } else {
                    RemovalRate = min(1.0, WetMatterAppropriated / NPPWetMatter);
            }
        }
         
        return RemovalRate;
    }
    //----------------------------------------------------------------------------------------------

