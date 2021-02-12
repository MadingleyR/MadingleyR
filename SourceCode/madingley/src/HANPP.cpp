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
           unsigned currentMonth ) {
        
        // Factor to convert NPP from units per m2 to units per km2
        double m2Tokm2Conversion = 1000000.0;

        //extraction parameters
        std::string humanNPPScenarioType= Parameters::Get( )->GetHumanNPPScenarioType();
        double humanNPPExtractionScale  = Parameters::Get( )->GetHumanNPPExtractionScale();
        double humanNPPScenarioDuration = Parameters::Get( )->GetHumanNPPScenarioDuration();
        unsigned burninSteps            = Parameters::Get( )->GetBurninSteps();
        unsigned impactSteps            = Parameters::Get( )->GetImpactSteps();
        unsigned recoverySteps          = Parameters::Get( )->GetRecoverySteps();
        
        //return value
        double RemovalRate=0;

        if( humanNPPScenarioType == "hanpp" ) {
            
          if (currentTimestep > burninSteps){

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

        } else if( humanNPPScenarioType == "constant" ){
            
          if (currentTimestep > burninSteps){

            // If the burn-in period has been completed, then remove the specified constant
            // fraction from the acting autotroph stock
                RemovalRate = humanNPPExtractionScale;
          }
          
        } else if (humanNPPScenarioType == "temporary") {
            // If the spin-up period has been completed and the period of impact has not elapsed,
            // then remove the specified constant fraction from the acting autotroph stock
            if ((currentTimestep > burninSteps) && (currentTimestep <= (burninSteps + impactSteps))){
                RemovalRate = humanNPPExtractionScale;
            }

        } else if (humanNPPScenarioType == "escalating") {
            // If the spin-up period has been completed, then remove a proportion of plant matter
            // according to the number of time-steps that have elapsed since the spin-up ended
            if (currentTimestep > burninSteps)  {
                RemovalRate = (min(1.0, (((currentTimestep - burninSteps) / 12.0) * humanNPPExtractionScale)));
            }

        } else if (humanNPPScenarioType == "temp-escalating") {
            // If the spin-up period has been completed and the period of impact has not elapsed, 
            // then remove a proportion of plant matter
            // according to the number of time-steps that have elapsed since the spin-up ended
            if ((currentTimestep > burninSteps) && (currentTimestep <= (burninSteps + impactSteps))) {
                        RemovalRate = (min(1.0, (((currentTimestep - burninSteps) / 12.0) * humanNPPExtractionScale)));
            }

        } else if (humanNPPScenarioType == "temp-escalating-const-rate")  {
            // If the spin-up period has been completed and the period of impact (specified by the third scenario element
            // has not elapsed, then remove a proportion of plant matter
            // according to the number of time-steps that have elapsed since the spin-up ended

            unsigned ConstImpactSteps = humanNPPScenarioDuration* mUtilities.ConvertTimeUnits("year", Parameters::Get( )->GetTimeStepUnits( ) );
            if ((currentTimestep > burninSteps) && (currentTimestep <= (burninSteps + ConstImpactSteps))) {
                RemovalRate = (min(1.0, (((currentTimestep - burninSteps) / 12.0) * humanNPPExtractionScale)));
            }

        } else if (humanNPPScenarioType == "temp-escalating-const-rate-duration") {
            // If the spin-up period has been completed and the period of impact (specified by the third scenario element)
            // has not elapsed, then remove a proportion of plant matter
            // according to the number of time-steps that have elapsed since the spin-up ended

            unsigned ConstImpactSteps = humanNPPScenarioDuration* mUtilities.ConvertTimeUnits("year", Parameters::Get( )->GetTimeStepUnits( ) );

            if ((currentTimestep > burninSteps) && (currentTimestep <= (burninSteps + impactSteps)))  {
                RemovalRate = (min(1.0,
                                   min(((ConstImpactSteps / 12.0) * humanNPPExtractionScale),
                                      (((currentTimestep - burninSteps) / 12.0) * humanNPPExtractionScale))));
            }
;
        } else if (humanNPPScenarioType == "temp-escalating-declining") {
            // If the spin-up period has been completed, then apply a level of harvesting
            // according to the number of time-steps that have elapsed since the spin-up ended
           if ((currentTimestep > burninSteps) && (currentTimestep <= (burninSteps + impactSteps)))  {
                RemovalRate = (min(1.0, (((currentTimestep - burninSteps) / 12.0) * humanNPPExtractionScale)));
            } else if ((currentTimestep > (burninSteps + impactSteps)) & (currentTimestep <= (burninSteps + impactSteps + recoverySteps))){
                RemovalRate = (min(1.0, (((burninSteps + impactSteps + recoverySteps - currentTimestep) / 12.0) * humanNPPExtractionScale)));
            }

        } else if( humanNPPScenarioType == "none" ) {
          //removal rate remains zero

        } else {
            cout<<"humanNPPScenarioType "<<humanNPPScenarioType<<endl;
            cout<<"humanNPPExtractionScale "<<humanNPPExtractionScale <<endl;
            cout<<"humanNPPScenarioDuration "<<humanNPPScenarioDuration <<endl;
            cout<<"burninSteps "<<burninSteps <<endl;           
            cout<<"impactSteps "<<impactSteps <<endl;           
            cout<<"recoverySteps "<<recoverySteps <<endl;          
            assert(humanNPPScenarioType =="none"  );
        }

        return RemovalRate;
    }
    //----------------------------------------------------------------------------------------------

