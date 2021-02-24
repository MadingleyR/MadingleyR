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
        if(InputParameters::Get( )->GetUseNonDefaultModelParameters()==1){
            std::vector<double> Params = InputParameters::Get( )->Get_VegetationModel_Parameters();
            //double fhanpp = mHANPP.RemoveHumanAppropriatedMatter( NPPWetMatter, gcl,actingStock, currentTimeStep, currentMonth );
            actingStock.mTotalBiomass += NPPWetMatter * Params[32]; //( 1 - fhanpp );
        }else{
            actingStock.mTotalBiomass += NPPWetMatter * 1.0; 
        }
        
        
    }
}
