#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <dirent.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <fstream>

#include "GridDatum.h"
#include "BasicDatum.h"
#include "Stopwatch.h"
#include "ThreadVariables.h"
#include "TimeStep.h"
#include "Maths.h"
#include "Parameters.h"
#include "DataRecorder.h"
#include "Types.h"
#include "WriteModelState.h"
#include "Grid.h"
#include "GridCell.h"
//#include "Environment.h"

// included for folder creation within windows
#if _WIN32
    #include <windows.h>
#endif

//##
void WriteModelState::WriteMonthlyTimeLineBiomass( std::string output_directory, unsigned SimulationInMonths_print, 
                                        std::vector< std::vector<double> > StableBiomass ) {

    int n_fg = StableBiomass[0].size();
    
    // open write connection, monthly biomass
    std::string csvName2 = output_directory + "timelines/" + "MontlyCohortBiomass.csv";
    ofstream CSV2;
    CSV2.open (csvName2);

    // write header
    CSV2 <<  "Month" << "," << "Year"; 
    for(int fg = 0; fg < n_fg; fg++){
        std::string fg_string = "Biomass_FG_" + std::to_string(fg);
        CSV2 << "," <<  fg_string;
    }
    CSV2 << std::endl;


    int yearii = 1;
    for(int ii = 0; ii < (SimulationInMonths_print); ii++){
        CSV2 << ii+1 << ", " << yearii;
        for(int fg = 0; fg < n_fg; fg++){
            CSV2  << "," << StableBiomass[ii][fg];
        }
        CSV2 << std::endl;
        if((ii+1) % 12 == 0) yearii++;
    }
    CSV2.close();
}
//##

//##
void WriteModelState::WriteCohortProperties( Grid& madingleyGrid, std::string output_directory, unsigned timeStep ) {
    
    std::string padding = "00000";
	std::string str_int = std::to_string(timeStep);
	std::string name_ts = padding.replace(padding.length()-str_int.length(),padding.length()-str_int.length(),str_int);
	name_ts.resize(5);

    std::string csvName = "C.csv";
    csvName = output_directory + "cohort_properties/" + "FullCohortProperties_" + name_ts + ".csv";

    ofstream CSV;
    CSV.open (csvName);

    // write header
    CSV <<  "GridcellIndex" << "," << 
            "FunctionalGroupIndex" << "," << 
            "JuvenileMass" << "," << 
            "AdultMass" << "," << 
            "IndividualBodyMass" << "," << 
            "CohortAbundance" << "," << 
            "LogOptimalPreyBodySizeRatio" << "," << 
            "BirthTimeStep" << "," << 
            "ProportionTimeActive" << "," << 
            "TrophicIndex" << "," << 
            "IndividualReproductivePotentialMass" << "," << 
            "MaturityTimeStep" << "," << 
            "IsAdult" << "," << 
            "AgeMonths" << "," << 
            "TimeStepsJuviline" << "," << 
            "TimeStepsAdult" <<
            std::endl;

    madingleyGrid.ApplyFunctionToAllCells( [&]( GridCell & gridCell ) {
        gridCell.ApplyFunctionToAllCohorts( [&]( Cohort* c ) {
                CSV <<
                gridCell.GetIndex( ) << "," <<
                c->mFunctionalGroupIndex << "," <<
                c->mJuvenileMass << "," <<
                c->mAdultMass << "," <<
                c->mIndividualBodyMass << "," <<
                c->mCohortAbundance << "," <<
                c->mLogOptimalPreyBodySizeRatio << "," <<
                c->mBirthTimeStep << "," <<
                c->mProportionTimeActive << "," <<
                c->mTrophicIndex << "," <<
                c->mIndividualReproductivePotentialMass << "," <<
                c->mMaturityTimeStep << "," <<
                c->mIsAdult << "," <<
                c->mAgeMonths << "," <<
                c->mTimeStepsJuviline << "," <<
                c->mTimeStepsAdult <<
                std::endl;
        } );
    } );
   CSV.close();
}
//##

//##
void WriteModelState::WriteStockProperties( Grid& madingleyGrid, std::string output_directory, unsigned timeStep ) {
    
    std::string padding = "00000";
	std::string str_int = std::to_string(timeStep);
	std::string name_ts = padding.replace(padding.length()-str_int.length(),padding.length()-str_int.length(),str_int);
	name_ts.resize(5);

    std::string csvName2 = "S.csv";
    csvName2 = output_directory + "stock_properties/" + "StockProperties_" + name_ts  + ".csv";
 
    ofstream CSV2;
    CSV2.open (csvName2);

    // write header
    CSV2 << "GridcellIndex" << "," << 
            "FunctionalGroupIndex" << "," << 
            "TotalBiomass" << 
            std::endl;

    // write data
    madingleyGrid.ApplyFunctionToAllCells( [&]( GridCell & gridCell ) {
        gridCell.ApplyFunctionToAllStocks( [&]( Stock & s ) {
              CSV2 <<
              gridCell.GetIndex( ) << "," <<
              s.mFunctionalGroupIndex << "," <<
              s.mTotalBiomass << // in grams
              std::endl;
        } );
    } );

    CSV2.close();
}
//##

//##
void WriteModelState::WriteBinnedPreyFoodWebConnections(Grid& mModelGrid, unsigned timeStep, std::string outputDirectory) {
	
	std::string padding = "00000";
	std::string str_int = std::to_string(timeStep);
	std::string name_ts = padding.replace(padding.length()-str_int.length(),padding.length()-str_int.length(),str_int);
	name_ts.resize(5);
	
    //#############################################################
	// Write food web log binned consumption
	
	std::vector<double> BinNames;
	for(int i = 0; i < 11; i++) BinNames.push_back(pow(10,(i-4)));
	 
    std::string BinCon_name = outputDirectory + "consumption_statistics/" + "PreyBinnedFoodwebConnections_" + name_ts +  ".csv";
    
    //std::cout << "writing file: "<< BinCon_name << std::endl;
		std::cout << "writing food-web files... " << std::endl;

    ofstream CSV_BinCon;
    CSV_BinCon.open (BinCon_name);

    CSV_BinCon << "Pred_AdlBM" << "," << "Pred_FunctionalGroupIndex" << "," << "Prey_LowerBodyMassBin" << "," << "Prey_Diet" << "," << "Prey_Thermoregulation" << "," << "Prey_BiomassConsumed" << std::endl;	


	mModelGrid.ApplyFunctionToAllCells( [&]( GridCell & gridCell ) {
	
		if ( !gridCell.IsMarine( ) ) {
				
				
			gridCell.ApplyFunctionToAllCohorts( [&]( Cohort* c ) {
				
				std::vector<float> vHerbivoreBIN_ecto(11,0.0f);
				std::vector<float> vHerbivoreBIN_endo(11,0.0f);
				std::vector<float> vOmnivoreBIN_ecto(11,0.0f);
				std::vector<float> vOmnivoreBIN_endo(11,0.0f);
				std::vector<float> vCarnivoreBIN_ecto(11,0.0f);
				std::vector<float> vCarnivoreBIN_endo(11,0.0f);
				
				vHerbivoreBIN_ecto = c->mConsumed_HerbivoreBIN_ecto;
				vHerbivoreBIN_endo = c->mConsumed_HerbivoreBIN_endo; 
				vOmnivoreBIN_ecto = c->mConsumed_OmnivoreBIN_ecto;
				vOmnivoreBIN_endo = c->mConsumed_OmnivoreBIN_endo;
				vCarnivoreBIN_ecto = c->mConsumed_CarnivoreBIN_ecto; 
				vCarnivoreBIN_endo = c->mConsumed_CarnivoreBIN_endo;
					
				for(int i = 0; i < 11; i++) 
					if(vHerbivoreBIN_ecto[i]>0) CSV_BinCon << c->mAdultMass << "," << c->mFunctionalGroupIndex << "," << BinNames[i] << "," << "H" << "," << "Ecto" << "," << vHerbivoreBIN_ecto[i] << std::endl;					
				for(int i = 0; i < 11; i++) 
					if(vHerbivoreBIN_endo[i]>0) CSV_BinCon << c->mAdultMass << "," << c->mFunctionalGroupIndex << "," << BinNames[i] << "," << "H" << "," << "Endo" << "," << vHerbivoreBIN_endo[i] << std::endl;					
				for(int i = 0; i < 11; i++) 
					if(vOmnivoreBIN_ecto[i]>0) CSV_BinCon << c->mAdultMass << "," << c->mFunctionalGroupIndex << "," << BinNames[i] << "," << "O" << "," << "Ecto" << "," << vOmnivoreBIN_ecto[i] << std::endl;					
				for(int i = 0; i < 11; i++) 
					if(vOmnivoreBIN_endo[i]>0) CSV_BinCon << c->mAdultMass << "," << c->mFunctionalGroupIndex << "," << BinNames[i] << "," << "O" << "," << "Endo" << "," << vOmnivoreBIN_endo[i] << std::endl;					
				for(int i = 0; i < 11; i++) 
					if(vCarnivoreBIN_ecto[i]>0) CSV_BinCon << c->mAdultMass << "," << c->mFunctionalGroupIndex << "," << BinNames[i] << "," << "C" << "," << "Ecto" << "," << vCarnivoreBIN_ecto[i] << std::endl;					
				for(int i = 0; i < 11; i++) 
					if(vCarnivoreBIN_endo[i]>0) CSV_BinCon << c->mAdultMass << "," << c->mFunctionalGroupIndex << "," << BinNames[i] << "," << "C" << "," << "Endo" << "," << vCarnivoreBIN_endo[i] << std::endl;					
							
			} );
			
		}
	} );

    CSV_BinCon.close();
	//#############################################################
	
}
//##

//##
void WriteModelState::WriteSpatialGridProperties( Grid& madingleyGrid, std::string output_directory ) {
    
    for( unsigned month = 0; month < 12; month += 1 ) {    

        std::string padding = "00000";
        std::string str_int = std::to_string(month);
        std::string name_ts = padding.replace(padding.length()-str_int.length(),padding.length()-str_int.length(),str_int);
        name_ts.resize(5);
 
        //Environment::Update( mUtilities.GetCurrentMonth( month )  ); // Update environment according to month
        
        std::string csvNameGrid = output_directory + "grid_properties/" +  "GridProperties_" + name_ts + ".csv";
        ofstream csvGrid;
        csvGrid.open (csvNameGrid);
        csvGrid << "GridcellIndex" << "," << "IsMarine" << "," << 
                   "GetRealm" << "," << "GetLatitudeIndex" << "," << 
                   "GetLongitudeIndex" << "," << "GetCellArea" << "," << 

                   "GetGridcellRealm" << "," << "GetGridcellLandMask" << "," << 
                   "AvailableWater" << "," << "GetGridcellHanpp" << "," << 
                   "TemperatureRange" << "," << "Temp" << "," << 
                   "GroundFrost" << "," << "Precipitation" << "," << 
                   
                   "TerrestrialNPP" << "," << "Get_Ecto_max" << "," << 
                   "Get_Endo_C_max" << "," << "Get_Endo_H_max" << "," << 
                   "Get_Endo_O_max" << "," << "TotalPrecip" << "," << 
                   "Seasonality" << "," << "BreedingSeason" << "," << 

                   "AnnualTemperature" << "," << "SDTemperature" << "," << 
                   "ExpTDevWeight" << "," << "AET" << "," << 
                   "TotalAET" << "," << "FractionMonthFrost" << "," << 
                   "FractionYearFrost" << "," << "FractionYearFire" << std::endl;

        madingleyGrid.ApplyFunctionToAllCells( [&]( GridCell & g ) {
            //if( !g.IsMarine( ) ) {
                csvGrid << g.GetIndex() << "," <<
                g.IsMarine( ) << "," <<
                g.GetRealm( ) << "," <<
                g.GetLatitudeIndex( ) << "," <<
                g.GetLongitudeIndex( ) << "," <<
                g.GetCellArea( ) << "," <<
                g.GetGridcellRealmClassification( ) << "," <<
                g.GetGridcellLandMask( ) << "," <<
                g.GetGridcellAvailableWaterCapacity( ) << "," <<
                g.GetGridcellHanpp( ) << "," <<
                g.GetGridcellDiurnalTemperatureRange( month ) << "," <<
                g.GetGridcellNearSurfaceTemp( month ) << "," <<
                g.GetGridcellGroundFrostFrequency( month ) << "," <<
                g.GetGridcellPrecipitation( month ) << "," <<
                g.GetGridcellTerrestrialNPP( month ) << "," <<

                g.Get_Ecto_max( ) << "," <<
                g.Get_Endo_C_max( ) << "," <<
                g.Get_Endo_H_max( ) << "," <<
                g.Get_Endo_O_max( ) << "," <<

                g.GetGridcellTotalPrecip( ) << "," <<
                g.GetGridcellSeasonality( month ) << "," <<
                g.GetGridcellBreedingSeason( month ) << "," <<
                g.GetGridcellAnnualTemperature( ) << "," <<
                g.GetGridcellSDTemperature( ) << "," <<
                g.GetGridcellExpTDevWeight( month ) << "," <<
                g.GetGridcellAET( month ) << "," <<
                g.GetGridcellTotalAET( ) << "," <<
                g.GetGridcellFractionMonthFrost( month ) << "," <<
                g.GetGridcellFractionYearFrost( ) << "," <<
                g.GetGridcellFractionYearFire( ) << std::endl; 
            //}
        } );
        csvGrid.close();
    }
}
//##

//##
void WriteModelState::WriteCohortConsumptionSummary(Grid& mModelGrid, unsigned timeStep, std::string outputDirectory ) {
    
    std::string padding = "00000";
	std::string str_int = std::to_string(timeStep);
	std::string name_ts = padding.replace(padding.length()-str_int.length(),padding.length()-str_int.length(),str_int);
	name_ts.resize(5);

    std::string csvName = outputDirectory + "consumption_statistics/" + "CohortConsumptionSummary_" + name_ts + ".csv";
    ofstream CSV;
    CSV.open (csvName);
    CSV << "GridcellIndex" << "," << "Cohort_FunctionalGroupIndex" << "," << "Cohort_AdultMass" << "," << 
        "Cohort_Consumed_Autotroph" << "," << 
        "Cohort_Consumed_Herbivore" << "," << 
        "Cohort_Consumed_Omnivore" << "," <<
        "Cohort_Consumed_Carnivore" << std::endl;

    mModelGrid.ApplyFunctionToAllCells( [&]( GridCell & gridCell ) {
        gridCell.ApplyFunctionToAllCohorts( [&]( Cohort* c ) {
            CSV << gridCell.GetIndex( ) << "," << c->mFunctionalGroupIndex << "," << c->mAdultMass << "," << 
            c->mConsumed_Autotroph << "," << c->mConsumed_Herbivore << "," << 
            c->mConsumed_Omnivore  << "," << c->mConsumed_Carnivore << std::endl;           
        } );
    } );
    CSV.close();
}
//##

//##
void WriteModelState::WriteCohortBiomassBinned(Grid& mModelGrid, unsigned timeStep, std::string outputDirectory, int n_FG ) {

    // check body mass bins per functional group
    std::vector<double> AldMassBins = {0.0001,0.0010,0.0100,0.1000,1.0000,10.0000,100.0000,1000.0000,10000.0000}; // round(10^seq(-4,4,1),4)
    std::vector< std::vector<double> > AbundancePerBin( n_FG ,std::vector<double>( AldMassBins.size(), 0 ));
    std::vector< std::vector<double> > BiomassPerBin( n_FG ,std::vector<double>( AldMassBins.size(), 0 ));
    std::vector<double> MaxBM_kg(n_FG,0.0);
    std::vector<double> MinBM_kg(n_FG,1000000000.0);
    mModelGrid.ApplyFunctionToAllCells( [&]( GridCell & gcl ) {
        gcl.ApplyFunctionToAllCohorts( [&]( Cohort* c ) {
            if(c->mAdultMass > MaxBM_kg[c->mFunctionalGroupIndex]) MaxBM_kg[c->mFunctionalGroupIndex] = c->mAdultMass;
            if(c->mAdultMass < MinBM_kg[c->mFunctionalGroupIndex]) MinBM_kg[c->mFunctionalGroupIndex] = c->mAdultMass;
            int AdultMassBin = 0;
            for(unsigned i = 0; i < AldMassBins.size(); i++) {
                if( c->mAdultMass  > (AldMassBins[i]*1000) ) {
                    AdultMassBin = i;
                }else{
                    break;
                }
            }
            BiomassPerBin[c->mFunctionalGroupIndex][AdultMassBin] += ( c->mIndividualBodyMass + c->mIndividualReproductivePotentialMass ) * c->mCohortAbundance;
            AbundancePerBin[c->mFunctionalGroupIndex][AdultMassBin] += c->mCohortAbundance;
        } );
    } );


	std::string padding = "00000";
	std::string str_int = std::to_string(timeStep);
	std::string name_ts = padding.replace(padding.length()-str_int.length(),padding.length()-str_int.length(),str_int);
	name_ts.resize(5);

    // Write csv for binned cohort stats
    std::string CSV_binned_FunctionalGroups = outputDirectory + "cohort_properties/" + "BinnedCohortStatistics_" + name_ts + ".csv";
    ofstream csvBinned;
    csvBinned.open (CSV_binned_FunctionalGroups);
    csvBinned << "FunctionalGroupIndex" << "," << 
                 "LowerBodyMassBin" << "," << 
                 "UpperBodyMassBin" << "," << 
                 "TotalAbundance" << "," << 
                 "TotalBiomass_kg" << std::endl;

    double sum_check2;
    for( int j = 0; j < AbundancePerBin.size( ); j++ ) { // loop over functional groups
        sum_check2 = 0;
        for( int i = 0; i < AbundancePerBin[j].size( ); i++ ) sum_check2 += AbundancePerBin[j][i]; // check if functional group is present
        if(sum_check2>0){
            for( int i = 0; i < (AbundancePerBin[j].size( )-1); i++ ) { // loop over bins within functional group
                    csvBinned << j << "," <<
                    AldMassBins[i] << "," <<
                    AldMassBins[i+1] << "," <<
                    AbundancePerBin[j][i] << "," <<
                    BiomassPerBin[j][i] / 1000 << std::endl;
            }
        }
    }
    csvBinned.close();

}
//##

//##
#if __linux__
void WriteModelState::CreateOutputSubfolders( std::string outputDirectory, std::string TypeOfRun ) { 

    std::string dir_string1 = outputDirectory + "cohort_properties/";
    std::string dir_string2 = outputDirectory + "stock_properties/";
    std::string dir_string3 = outputDirectory + "grid_properties/";
    std::string dir_string4 = outputDirectory + "consumption_statistics/";
    std::string dir_string5 = outputDirectory + "timelines/";
    mkdir( dir_string1.c_str(), S_IRWXU);
    mkdir( dir_string2.c_str(), S_IRWXU);
    if(TypeOfRun=="run") mkdir( dir_string3.c_str(), S_IRWXU);
    if(TypeOfRun=="run") mkdir( dir_string4.c_str(), S_IRWXU);
    if(TypeOfRun=="run") mkdir( dir_string5.c_str(), S_IRWXU);
}
#elif __APPLE__
void WriteModelState::CreateOutputSubfolders( std::string outputDirectory, std::string TypeOfRun ) { 

    std::string dir_string1 = outputDirectory + "cohort_properties/";
    std::string dir_string2 = outputDirectory + "stock_properties/";
    std::string dir_string3 = outputDirectory + "grid_properties/";
    std::string dir_string4 = outputDirectory + "consumption_statistics/";
    std::string dir_string5 = outputDirectory + "timelines/";
    mkdir( dir_string1.c_str(), S_IRWXU);
    mkdir( dir_string2.c_str(), S_IRWXU);
    if(TypeOfRun=="run") mkdir( dir_string3.c_str(), S_IRWXU);
    if(TypeOfRun=="run") mkdir( dir_string4.c_str(), S_IRWXU);
    if(TypeOfRun=="run") mkdir( dir_string5.c_str(), S_IRWXU);
}
#elif _WIN32
void WriteModelState::CreateOutputSubfolders( std::string outputDirectory, std::string TypeOfRun ) { 

    std::string dir_string1 = outputDirectory + "cohort_properties/";
    std::string dir_string2 = outputDirectory + "stock_properties/";
    std::string dir_string3 = outputDirectory + "grid_properties/";
    std::string dir_string4 = outputDirectory + "consumption_statistics/";
    std::string dir_string5 = outputDirectory + "timelines/";
    CreateDirectory( dir_string1.c_str(),NULL);
    CreateDirectory( dir_string2.c_str(),NULL);
    if(TypeOfRun=="run") CreateDirectory( dir_string3.c_str(),NULL);
    if(TypeOfRun=="run") CreateDirectory( dir_string4.c_str(),NULL);
    if(TypeOfRun=="run") CreateDirectory( dir_string5.c_str(),NULL);
}
#endif
//##

//##
void WriteModelState::WriteStockTimeline( Grid& madingleyGrid, std::string output_directory, unsigned timeStep ) {
    
    std::string padding = "00000";
	std::string str_int = std::to_string(timeStep);
	std::string name_ts = padding.replace(padding.length()-str_int.length(),padding.length()-str_int.length(),str_int);
	name_ts.resize(5);

    std::string csvName2 = "S.csv";
    csvName2 = output_directory + "timelines/" +  "MonthlyStockBiomass" + ".csv";
 
    ofstream CSV2;
    CSV2.open (csvName2, ios::out | ios::app);

    // calc sum biomass over all gridcells and all stock functional groups 
    double StockBiomass = 0.0;
    madingleyGrid.ApplyFunctionToAllCells( [&]( GridCell & gridCell ) {
        gridCell.ApplyFunctionToAllStocks( [&]( Stock & st ) { StockBiomass += st.mTotalBiomass; } );
    } );

    // write data
    CSV2 << StockBiomass/1000 << std::endl; // calculate to kg

    CSV2.close();
}
//##