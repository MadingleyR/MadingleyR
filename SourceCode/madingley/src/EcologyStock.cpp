#include "EcologyStock.h"
#include "InputParameters.h"

void EcologyStock::RunWithinCellEcology( GridCell& gcl, Stock& actingStock, unsigned currentTimeStep, unsigned currentMonth, MadingleyInitialisation& params ) {

    FunctionalGroupDefinitions& madingleyStockDefinitions = params.mStockFunctionalGroupDefinitions;
    //changes here to make this code consistent with the merge-and-refactor C# code
    if( gcl.IsMarine( ) ) {
        // Run the autotroph processor
        mMarineNPPtoAutotrophStock.ConvertNPPToAutotroph( gcl, actingStock, currentMonth );
    } else {
        // Run the dynamic plant model to update the leaf stock for this time step
        double NPPWetMatter = mDynamicPlantModel.UpdateLeafStock( gcl, actingStock, currentTimeStep, madingleyStockDefinitions.GetTraitNames( "leaf strategy", actingStock.mFunctionalGroupIndex ) == "deciduous", Parameters::Get( )->GetTimeStepUnits( ), currentMonth );

        // Apply human appropriation of NPP - note in the latest C# version this is changed to include the NPPWetMatter calculated above
        if(InputParameters::Get( )->Get_USE_HANPP()>0.5){
            
            double USE_HANPP = InputParameters::Get( )->Get_USE_HANPP();
            double fractionReducedVegProduction = 1.0; 
            //std::cout << "USE_HANPP: " << USE_HANPP << std::endl;
            
            if(USE_HANPP>0.7 && USE_HANPP<1.4){
                
                // hanpp raster applied as fractional values, a value in a raster cell of 0.9 means 10% reduction of veg production
                double fhanpp = mHANPP.RemoveHumanAppropriatedMatter( NPPWetMatter, gcl,actingStock, currentTimeStep, currentMonth, true );
                //std::cout << "fhanpp: " << fhanpp << std::endl;
                fractionReducedVegProduction = fhanpp; 
                //if(fhanpp>1.5) std::cout << "incorrect definition of HANPP! values > 1, absolute input raster provided?"<< std::endl;
                
            }else if(USE_HANPP>1.7){
                
                // HANPP applied spatially using raster with values in gC/m^2/year
                double hanpp = mHANPP.RemoveHumanAppropriatedMatter( NPPWetMatter, gcl,actingStock, currentTimeStep, currentMonth, false );
                //std::cout << "hanpp: " << hanpp << std::endl;
                fractionReducedVegProduction = ( 1 - hanpp );
            }
            
            // apply to stock
            //std::cout << "fractionReducedVegProduction: " << fractionReducedVegProduction << std::endl;
            actingStock.mTotalBiomass += NPPWetMatter * fractionReducedVegProduction;

        }else{
            
            // No HANPP
            actingStock.mTotalBiomass += NPPWetMatter * 1.0; 

        }
        
        
    }
}
