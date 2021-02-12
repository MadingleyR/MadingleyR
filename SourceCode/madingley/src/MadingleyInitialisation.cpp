#include "MadingleyInitialisation.h"
#include <random>
#include <iostream>
#include "InputParameters.h"
#include <stdlib.h>


MadingleyInitialisation::MadingleyInitialisation( ) {
    mOutputPath = "";
    mNextCohortID = 0;
}

MadingleyInitialisation::MadingleyInitialisation( long long& nextCohortID, double& numberOfCohorts, double& numberOfStocks, Grid& modelGrid, bool UseMadingleySpinUp ) {

    TypeOfRun = InputParameters::Get( )->GetTypeOfRun();

    // apply model spin up
    if(TypeOfRun=="run") {
        //std::cout << "Loading cohorts and stocks from file" << std::endl;
        //std::cout << "Importing cohort spin-up properties from: " << CohortCSVLocation << " (using flag overwrite)" << std::endl;
        CohortCSVLocation = InputParameters::Get( )->GetCohortCSVLocation();
        CohortData = readSpinUpStateCohort( CohortCSVLocation ); // Cohort data
        //std::cout << "Importing stock spin-up properties from: " << StockCSVLocation << " (using flag overwrite)"<< std::endl;
        StockCSVLocation = InputParameters::Get( )->GetStockCSVLocation();
        StockData = readSpinUpStateStock( StockCSVLocation ); // Stock data
    } else {
        std::cout << "Initializing cohorts and stocks using cohort and stock definitions" << std::endl;
    }

    //read and store environmental layers
    //Environment::Get( );
    mNextCohortID = 0;

    ReadInitialisationFiles( );
	//std::cout << "Spatial grid setup" << std::endl;
    modelGrid.SetUpGrid( );
	//std::cout << "Spatial grid setup done" << std::endl;

    //---------------- grid env vectors, loaded csv data -------------------
    // grid env vectors[cellIndex], loaded csv data
    eGV_awc = InputParameters::Get( )->GetSpatialLayers_awc();
    eGV_land_mask = InputParameters::Get( )->GetSpatialLayers_land_mask();
    eGV_hanpp = InputParameters::Get( )->GetSpatialLayers_hanpp();
    eGV_realm_class = InputParameters::Get( )->GetSpatialLayers_realm_class();
    eGV_Ecto_max = InputParameters::Get( )->GetSpatialLayers_Ecto_max(); // Ecto_max
    eGV_Endo_C_max = InputParameters::Get( )->GetSpatialLayers_Endo_C_max(); // Endo_C_max
    eGV_Endo_H_max = InputParameters::Get( )->GetSpatialLayers_Endo_H_max(); // Endo_H_max
    eGV_Endo_O_max = InputParameters::Get( )->GetSpatialLayers_Endo_O_max(); // Endo_O_max

    // 3d env grid vectors[timeIndex][cellIndex]
    eGV_dtr = InputParameters::Get( )->GetSpatialLayers_dtr(); // diurnal_temperature_range
    eGV_gff = InputParameters::Get( )->GetSpatialLayers_gff(); // ground_frost_frequency
    eGV_nst = InputParameters::Get( )->GetSpatialLayers_nst(); // near-surface_temperature
    eGV_prec = InputParameters::Get( )->GetSpatialLayers_prec(); // precipitation
    eGV_tnpp = InputParameters::Get( )->GetSpatialLayers_tnpp(); // terrestrial_net_primary_productivity
    //--------------------------------------------------------------------
    
    //---Environmental set layers--------------------
    unsigned gc_counter = 0;
    std::vector< double > dtr(12,0.0), nst(12,0.0), gff(12,0.0), prec(12,0.0), tnpp(12,0.0);
    modelGrid.ApplyFunctionToAllCells( [&]( GridCell & gc ) {
      
      // temp time dependent vectors for this gridcell
      for( unsigned i = 0; i < 12; i++) {
        dtr[i] = eGV_dtr[i][gc_counter];
        nst[i] = eGV_nst[i][gc_counter];
        gff[i] = eGV_gff[i][gc_counter];
        prec[i] = eGV_prec[i][gc_counter];
        tnpp[i] = eGV_tnpp[i][gc_counter];
      }
      
      // get time independent values for this gridcell
      double realm = eGV_realm_class[gc_counter];
      double awc = eGV_awc[gc_counter];
      int land_mask = (int)eGV_land_mask[gc_counter];
      double hanpp = eGV_hanpp[gc_counter];
      
      // set time independent values for this gridcell
      gc.SetGridcellRealmClassification( realm );
      gc.SetGridcellAvailableWaterCapacity( awc );
      gc.SetGridcellLandMask( land_mask );
      gc.SetGridcellHanpp( hanpp );
      
      // set time dependent values for this gridcell
      gc.SetGridcellDiurnalTemperatureRange( dtr );
      gc.SetGridcellNearSurfaceTemp( nst );
      gc.SetAVGSDTemp( realm, nst );
      gc.SetFrostandFire( realm, awc, nst, gff, prec );
      gc.SetGridcellGroundFrostFrequency( gff );
      gc.SetGridcellPrecipitation( prec );
      gc.SetGridcellTerrestrialNPP( tnpp );
      gc.SetNPPSeasonalityAndBreeding( tnpp );
      
      // set body mass maximum values for this gridcell
      gc.Set_Ecto_max( eGV_Ecto_max[gc_counter] );
      gc.Set_Endo_C_max( eGV_Endo_C_max[gc_counter] );
      gc.Set_Endo_H_max( eGV_Endo_H_max[gc_counter] );
      gc.Set_Endo_O_max( eGV_Endo_O_max[gc_counter] );
      
      gc_counter++;
    } );
    //-----------------------------------------------
    
    //---Environmental set layers--------------------
    // modelGrid.ApplyFunctionToAllCells( [&]( GridCell & cell ) {
    //   for( unsigned i = 0; i < 12; i++) {
    //     std::cout << cell.GetGridcellBreedingSeason( i ) << std::endl;
    //   }
    // } );
    //-----------------------------------------------
    
    // Set up the cohorts and stocks
    mInitializationTimer.Start( );

    long totalCohorts = 0;
    long totalStocks = 0;

    unsigned int NumberGridCellsToInitiate = Parameters::Get( )->GetNumberOfGridCells( );
    unsigned int GridCellCounter = 1;

    
    // Initialize cohorts and stock in gridcells
    std::cout << "Initializing " << NumberGridCellsToInitiate << " gridcells";
    modelGrid.ApplyFunctionToAllCells( [&]( GridCell & c ) {

        // Insert cohorts and stocks
        if(TypeOfRun=="run") {
            //std::cout << "Loading cohorts and stocks from file" << std::endl;
            if( !c.IsMarine( ) ){    
                totalStocks += SeedStocksApplySpinUp( c, StockData );
            }
        } else {
            //std::cout << "Creating cohorts and stocks using cohort and stock definitions" << std::endl;
            // Create cohorts and stocks using user input
            totalCohorts += SeedGridCellCohorts( c );  
            totalStocks += SeedGridCellStocks( c );
            
        }
        // Output ini progress (per gridcell print ".")
		if(GridCellCounter < 20) std::cout << ".";
        
        GridCellCounter++;

    } );
	std::cout << " done" << std::endl;

    if(TypeOfRun=="run") {

        int maxAttempts = 10;

        for (int count = 0; count < maxAttempts; count++) {

            if(count == 0) {
                std::cout << "Loading cohort and stock data";
            }else{
                std::cout << "Retry " << count << " loading cohort and stock data";
            }

            try { /* Try reading cohort data */ 

                totalCohorts = SeedCohortsApplySpinUpFast( modelGrid, CohortData ); // if(UseMadingleySpinUp==1) 
                std::cout << ", load succesful" << std::endl;
                count = maxAttempts; //don't retry
            
            } catch (...) { /* Exit if fails */ 
                
                std::cout << ", loading failed" << std::endl;
            
            }
        }

    }

   
    CohortData.clear();
    StockData.clear();

    std::cout << totalCohorts << " cohorts and " << totalStocks << " stocks initialized" << std::endl;
    std::cout << "     " << std::endl;

    nextCohortID = mNextCohortID;
    numberOfCohorts = totalCohorts;
    numberOfStocks = totalStocks;
    mInitializationTimer.Stop( );
    Cohort::ResetMassFluxes( );
    //std::cout << "Time required: " << mInitializationTimer.GetElapsedTimeSecs( ) << std::endl;
	//std::cout << "Starting simulation" << std::endl;
}

void MadingleyInitialisation::ReadInitialisationFiles( ) {

    //std::cout << "Reading functional group definitions..." << std::endl;
    mInitialisationFileStrings["CohortFunctional"] = Constants::cCohortDefinitionsFileName;
    mCohortFunctionalGroupDefinitions = FunctionalGroupDefinitions( Constants::cCohortDefinitionsFileName );
    mInitialisationFileStrings["StockFunctional"] = Constants::cStockDefinitionsFileName;
    mStockFunctionalGroupDefinitions = FunctionalGroupDefinitions( Constants::cStockDefinitionsFileName );
    mModelMassBins.SetUpMassBins( Constants::cMassBinDefinitionsFileName );
    //assert( CellRarefaction >= 1 && "Cell rarefaction cannot be less than 1" ); // FIX - Does this need to be uncommented?
}

long MadingleyInitialisation::SeedGridCellCohorts( GridCell& gcl ) {
    long totalCohorts = 0;
    unsigned numCohortsThisCell = 0;
    // Define local variables
    double cohortJuvenileMass;
    double cohortAdultMassRatio;
    double cohortAdultMass;
    double expectedLnAdultMassRatio;
    double totalNewBiomass = 0.0;
    double optimalPreyBodySizeRatio;

    //std::cout << "gridcell is marine? " << gcl.IsMarine( ) << std::endl;

    gcl.SetCohortSize( mCohortFunctionalGroupDefinitions.mAllFunctinoalGroupsIndex.size( ) );
    for( int FunctionalGroup: mCohortFunctionalGroupDefinitions.mAllFunctinoalGroupsIndex ) {
        int N = mCohortFunctionalGroupDefinitions.GetBiologicalPropertyOneFunctionalGroup( "Initial number of GridCellCohorts", FunctionalGroup );
        if( ( mCohortFunctionalGroupDefinitions.GetTraitNames( "Realm", FunctionalGroup ) == "terrestrial" && !gcl.IsMarine( ) ) ||
                ( mCohortFunctionalGroupDefinitions.GetTraitNames( "Realm", FunctionalGroup ) == "marine" && gcl.IsMarine( ) ) ) {

            numCohortsThisCell += N;
        }
    }
    if( numCohortsThisCell > 0 ){
        //Loop over all functional groups in the model
        for( int functionalGroup: mCohortFunctionalGroupDefinitions.mAllFunctinoalGroupsIndex ) {
            // If it is a functional group that corresponds to the current realm, then seed cohorts
            if( ( mCohortFunctionalGroupDefinitions.GetTraitNames( "Realm", functionalGroup ) == "terrestrial" && !gcl.IsMarine( ) ) ||
                    ( mCohortFunctionalGroupDefinitions.GetTraitNames( "Realm", functionalGroup ) == "marine" && gcl.IsMarine( ) ) ) {
                // Get the minimum and maximum possible body masses for organisms in each functional group
                double massMinimum = mCohortFunctionalGroupDefinitions.GetBiologicalPropertyOneFunctionalGroup( "minimum mass", functionalGroup );
                double massMaximum = mCohortFunctionalGroupDefinitions.GetBiologicalPropertyOneFunctionalGroup( "maximum mass", functionalGroup );

                double proportionTimeActive = mCohortFunctionalGroupDefinitions.GetBiologicalPropertyOneFunctionalGroup( "proportion suitable time active", functionalGroup );

                // Loop over the initial number of cohorts
                unsigned numberOfCohortsInThisFunctionalGroup = 1;

                numberOfCohortsInThisFunctionalGroup = mCohortFunctionalGroupDefinitions.GetBiologicalPropertyOneFunctionalGroup( "initial number of gridcellcohorts", functionalGroup );

                for( unsigned jj = 0; jj < numberOfCohortsInThisFunctionalGroup; jj++ ) {
                    mRandomNumber.SetSeed( ( unsigned int )( jj + 1 ), ( unsigned int )( ( jj + 1 ) * 3 ) );

                    // setup uniform sample
                    std::random_device rd;  //Will be used to obtain a seed for the random number engine
                    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
                    std::uniform_real_distribution<> dis(0.0, 1.0);
                    
                    //################ SET MAXIMUM ALDULT BODY MASS USING SPATIAL MAX BM MAPS (csv inputs)

                    //------------------------------------------------------------------------------
                    // Import layers from csv for setting maximum cohort biomass
                    // TestingLayer1 = Maximum endotherm carnivore biomass                                  ==> Endo_C_max.nc4
                    // TestingLayer2 = Maximum endotherm herbivore biomass                                  ==> Endo_H_max.nc4
                    // TestingLayer3 = Maximum endotherm omnivore biomass                                   ==> Endo_O_max.nc4
                    // TestingLayer4 = Maximum ectotherm biomass (only applies to iteroparity cohorts)      ==> Ecto_max.nc4
                    //------------------------------------------------------------------------------

                    // Draw adult mass from a log-normal distribution with mean -6.9 and standard deviation 10.0,
                    // within the bounds of the minimum and maximum body masses for the functional group

                    double UniRandom = rand() / (RAND_MAX + 1.); //dis(gen) 

                    if(mCohortFunctionalGroupDefinitions.GetTraitNames( "reproductive strategy", functionalGroup ) == "iteroparity" ) {

                        double spatial_massMaximum = 0;

                        // get spatial_massMaximum ectotherms
                        if( mCohortFunctionalGroupDefinitions.GetTraitNames( "Endo/Ectotherm" , functionalGroup ) == "ectotherm" ) {
                            
			    	        spatial_massMaximum = massMaximum;

                        // get spatial_massMaximum endotherms
                        }else{
                            if( mCohortFunctionalGroupDefinitions.GetTraitNames( "Nutrition source", functionalGroup ) == "carnivore" ) {
                                spatial_massMaximum = gcl.Get_Endo_C_max( );//Environment::Get( "TestingLayer1", gcl.GetIndex( ) );
                            }else if( mCohortFunctionalGroupDefinitions.GetTraitNames( "Nutrition source", functionalGroup ) == "herbivore" ) {
                                spatial_massMaximum = gcl.Get_Endo_H_max( ); //massMaximum; //Environment::Get( "TestingLayer2", gcl.GetIndex( ) );
                            }else{ // omnivores
                                spatial_massMaximum = gcl.Get_Endo_O_max( ); //45000;
                            }
                        }
                    
                        cohortAdultMass = pow( 10, ( UniRandom * ( log10( spatial_massMaximum ) - log10( 50 * massMinimum ) ) + log10( 50 * massMinimum ) ) );


                    }else{ // All semelparity cohorts are not set using spatial maps of max body masses

                        // Default model body mass initialization, massMaximum from input csv
                        cohortAdultMass = pow( 10, ( UniRandom * ( log10( massMaximum ) - log10( 50 * massMinimum ) ) + log10( 50 * massMinimum ) ) );
                    
                    }
                    //#################################################################################

                    //std::cout << dis(gen) << " " << rand() / (RAND_MAX + 1.) << std::endl;

                    //# update to improve e.g. predation process of large carnivores (only applies to terrestrial fauna)
                    if( mCohortFunctionalGroupDefinitions.GetTraitNames( "Endo/Ectotherm" , functionalGroup ) == "endotherm" &&
                        cohortAdultMass >= 21000 && 
                        mCohortFunctionalGroupDefinitions.GetTraitNames( "Nutrition source", functionalGroup ) == "carnivore" ) {
                        
                        optimalPreyBodySizeRatio = std::max( 0.01, mRandomNumber.GetNormal( 1, 0.02 ) );
                    
                    }else{

                        optimalPreyBodySizeRatio = std::max( 0.01, mRandomNumber.GetNormal( 0.1, 0.02 ) );
                        
                    } 
                    //# end new code
                    //std::cout << optimalPreyBodySizeRatio << std::endl;

                    if( !gcl.IsMarine( ) ) {
                        do {
                            expectedLnAdultMassRatio = 2.24 + 0.13 * log( cohortAdultMass );

                            cohortAdultMassRatio = 1.0 + mRandomNumber.GetLogNormal( expectedLnAdultMassRatio, 0.5 );
                            cohortJuvenileMass = cohortAdultMass * 1.0 / cohortAdultMassRatio;
                        } while( cohortAdultMass <= cohortJuvenileMass || cohortJuvenileMass < massMinimum );
                    } else {
                        do {
                            expectedLnAdultMassRatio = 2.24 + 0.13 * log( cohortAdultMass );

                            cohortAdultMassRatio = 1.0 + 10 * mRandomNumber.GetLogNormal( expectedLnAdultMassRatio, 0.5 );
                            ;
                            cohortJuvenileMass = cohortAdultMass * 1.0 / cohortAdultMassRatio;
                        } while( cohortAdultMass <= cohortJuvenileMass || cohortJuvenileMass < massMinimum );
                    }

                    double NewBiomass = ( 3300. / numCohortsThisCell ) * 100 * 3000 *
                            pow( 0.6, ( log10( cohortJuvenileMass ) ) ) * ( gcl.GetCellArea( ) );

                    totalNewBiomass += NewBiomass;
                    double NewAbund = 0.0;

                    NewAbund = NewBiomass / cohortJuvenileMass;

                    //#### trophicindex
                    double trophicindex = 0.0;
                    if( mCohortFunctionalGroupDefinitions.GetTraitNames( "Nutrition source", functionalGroup ) == "herbivore" ) {
                        trophicindex = 2.0;
                    }
                    if( mCohortFunctionalGroupDefinitions.GetTraitNames( "Nutrition source", functionalGroup ) == "omnivore" ) {
                        trophicindex = 2.5;
                    }
                    if( mCohortFunctionalGroupDefinitions.GetTraitNames( "Nutrition source", functionalGroup ) == "carnivore" ) {
                        trophicindex = 3.0;
                    }
                    //####

                    double individualReproductivePotentialMass = 0;
                    unsigned maturityTimeStep = std::numeric_limits<unsigned>::max( );


                    int isAdult = 0;
                    int ageMonths = 0;
                    int timeStepsJuviline = 0;
                    int timeStepsAdult = 0;

                    //####### test 
                    //if( mCohortFunctionalGroupDefinitions.GetTraitNames( "Endo/Ectotherm" , functionalGroup ) == "ectotherm" ) NewAbund = NewAbund / 10.0;

                    // Initialise the new cohort with the relevant properties
                    Cohort* NewCohort=new Cohort( gcl, functionalGroup, cohortJuvenileMass, cohortAdultMass, cohortJuvenileMass, NewAbund,
                            optimalPreyBodySizeRatio, 0, proportionTimeActive, mNextCohortID, trophicindex, individualReproductivePotentialMass, maturityTimeStep,
                            isAdult, ageMonths, timeStepsJuviline, timeStepsAdult );

                    // Add the new cohort to the list of grid cell cohorts
                    gcl.mCohorts[functionalGroup].push_back( NewCohort );
                    // Increment the variable tracking the total number of cohorts in the model
                    totalCohorts++;

                }
            }
        }
    }
    return totalCohorts;
}

long MadingleyInitialisation::SeedGridCellStocks( GridCell& gcl ) {

    long totalStocks = 0;

    // Loop over all stock functional groups in the model
    for( int functionalGroupIndex: mStockFunctionalGroupDefinitions.mAllFunctinoalGroupsIndex ) {
        
        // Initialise the new stock with the relevant properties
        bool success;
        Stock NewStock( mStockFunctionalGroupDefinitions, functionalGroupIndex, gcl, success );
        // Add the new stock to the list of grid cell stocks
        if( success && !gcl.IsMarine( ) ) { //# test no marine stock ini
            gcl.mStocks[functionalGroupIndex].push_back( NewStock );
            totalStocks++;
        }


    }


    return totalStocks;
}

/////////////////////////////////////////////////////////////
// Functions to seed model using Spin-up outputs           //
/////////////////////////////////////////////////////////////

long MadingleyInitialisation::SeedStocksApplySpinUp( GridCell& gcl, std::vector< std::vector<std::string> > StockData ) {

    long totalStocks = 0;
    unsigned numStocksThisCell = 0;
    int gridCellIndex = gcl.GetIndex( );

    //std::cout << gridCellIndex << std::endl;
    std::vector<int> GCindics;
    std::transform(StockData[0].begin(), StockData[0].end(), std::back_inserter(GCindics),[](const std::string& str) { return std::stoi(str); });

    int firstIndex;
    bool CellPopulated;
    if (std::binary_search (GCindics.begin(), GCindics.end(), gridCellIndex)) {
      CellPopulated = true;
      firstIndex = find(GCindics.begin(), GCindics.end(), gridCellIndex) - GCindics.begin();
    } else CellPopulated = false;

    //std::cout << "  stocks?  ==>" <<  CellPopulated << std::endl;

    // Loop over all stock functional groups in the model
    for( int functionalGroupIndex: mStockFunctionalGroupDefinitions.mAllFunctinoalGroupsIndex ) {

            // Initialise the new stock with the relevant properties
            bool success;
            Stock NewStock( mStockFunctionalGroupDefinitions, functionalGroupIndex, gcl, success );
            // Add the new stock to the list of grid cell stocks
            if( success && !gcl.IsMarine( ) ) { //# test no marine stock ini
                gcl.mStocks[functionalGroupIndex].push_back( NewStock );
                totalStocks++;
            }
    }
    //std::cout << " " << std::endl;

    // reassing mTotalBiomass using spinup stock data
    if ( CellPopulated > 0 ) {
        //std::cout << "\033[0;35m" << numStocksThisCell << " " << totalStocks << "\033[0m" << std::endl;
        int StockCounter = firstIndex;
        gcl.ApplyFunctionToAllStocks( [&]( Stock & s ) {
            //cout << "ini mass: " << s.mTotalBiomass << " spinup mass: " << std::stod(StockData[2][StockCounter]) << std::endl; 
            s.mTotalBiomass = std::stod(StockData[2][StockCounter]);
            StockCounter++;
            totalStocks++;
        } );
    }
    //std::cout << "  " << std::endl;
    return totalStocks;
}

std::vector< std::vector<std::string> > MadingleyInitialisation::readSpinUpStateCohort( std::string filedir ) {

    //std::cout << filedir << std::endl;
    std::vector< std::vector<std::string> > v(16,std::vector<std::string>(0));
    std::ifstream file ( filedir );
    unsigned col = 0;
    unsigned row = 0;
    std::string value;

    while ( file.good() )
    {
        getline ( file, value, ',' );
        int pos;
        if ((pos=value.find('\n')) != std::string::npos) {
          std::string end = value; end.erase(pos);
          std::string start = value; start.erase(0,pos+1);
          // last value current row
          if(end.length() != 0) { //cout << " v = " << end << " | row = " << row << " | col = " << col << endl;
            v[col].push_back(end);
          }
          row++; col = 0; // update row and col index
          // first value next row
          if(start.length() != 0) { //cout << " v = " << start << " | row = " << row << " | col = " << col << endl;
            v[col].push_back(start);
          }
        } else { //cout << " v = " << value << " | row = " << row << " | col = " << col << endl;
          v[col].push_back(value);
        }
        col++;
    }
    //std::cout << "number of rows in file: " << row << std::endl; 
    return v;

}

std::vector< std::vector<std::string> > MadingleyInitialisation::readSpinUpStateStock( std::string filedir ) {

    //std::cout << filedir << std::endl;
    std::vector< std::vector<std::string> > v(3,std::vector<std::string>(0));
    std::ifstream file ( filedir );
    unsigned col = 0;
    unsigned row = 0;
    std::string value;

    while ( file.good() )
    {
        getline ( file, value, ',' );
        int pos;
        if ((pos=value.find('\n')) != std::string::npos) {
          std::string end = value; end.erase(pos);
          std::string start = value; start.erase(0,pos+1);
          // last value current row
          if(end.length() != 0) { //cout << " v = " << end << " | row = " << row << " | col = " << col << endl;
            v[col].push_back(end);
          }
          row++; col = 0; // update row and col index
          // first value next row
          if(start.length() != 0) { //cout << " v = " << start << " | row = " << row << " | col = " << col << endl;
            v[col].push_back(start);
          }
        } else { //cout << " v = " << value << " | row = " << row << " | col = " << col << endl;
          v[col].push_back(value);
        }
        col++;
    }
    //std::cout << "number of rows in file: " << row << std::endl;
    return v;
}

/////////////////// ini using spin-up fast ///////////////////
long MadingleyInitialisation::SeedCohortsApplySpinUpFast( Grid& modelGrid, std::vector< std::vector<std::string> > CohortData ) {

    // number of cohorts in total
    long nrows = CohortData[0].size();
    //std::cout << "n _ cohorts to ini: " << nrows << std::endl;
    long totalCohorts = 0;

    for( unsigned jj = 0; jj < nrows; jj++ ) {

        // CohortData[0] ==> gridcell index
        unsigned gridCellIndex = std::stoul(CohortData[0][jj]);

        // CohortData[1] ==> functional index
        int functionalGroup = std::stoi(CohortData[1][jj]);

        // CohortData[2] ==> JuvenileMass
        double cohortJuvenileMass = std::stod(CohortData[2][jj]);

        // CohortData[3] ==> AdultMass
        double cohortAdultMass = std::stod(CohortData[3][jj]);

        // CohortData[4] ==> IndividualBodyMass
        double IndividualBodyMass = std::stod(CohortData[4][jj]);

        // CohortData[5] ==> CohortAbundance
        double NewAbund = std::stod(CohortData[5][jj]);

        // CohortData[6] ==> LogOptimalPreyBodySizeRatio
        double optimalPreyBodySizeRatio = exp(std::stod(CohortData[6][jj]));

        // CohortData[7] ==> BirthTimeStep
        unsigned BirthTimeStep = std::stod(CohortData[7][jj]);;

        // CohortData[8] ==> ProportionTimeActive
        double ProportionTimeActive = std::stod(CohortData[8][jj]);

        // CohortData[9] ==> TrophicIndex
        double trophicindex = std::stod(CohortData[9][jj]);

        // CohortData[10] ==> individualReproductivePotentialMass
        //double individualReproductivePotentialMass = 0;
        double individualReproductivePotentialMass = std::stod(CohortData[10][jj]);
        
        // CohortData[11] ==> maturityTimeStep
        //unsigned maturityTimeStep = std::numeric_limits<unsigned>::max( );
        unsigned maturityTimeStep = std::stod(CohortData[11][jj]);

        //################################################################################

        // CohortData[12] ==> mIsAdult
        int isAdult = std::stoi(CohortData[12][jj]);

        // CohortData[13] ==> mAgeMonths
        int ageMonths = std::stoi(CohortData[13][jj]);

        // CohortData[14] ==> mTimeStepsJuviline
        int timeStepsJuviline = std::stoi(CohortData[14][jj]);

        // CohortData[15] ==> mTimeStepsAdult
        int timeStepsAdult = std::stoi(CohortData[15][jj]);

        // std::cout << 
        // gridCellIndex << ", "<< 
        // functionalGroup << ", "<< 
        // cohortJuvenileMass << ", "<< 
        // cohortAdultMass << ", "<< 
        // IndividualBodyMass << ", "<< 
        // NewAbund << ", "<< 
        // optimalPreyBodySizeRatio << ", "<< 
        // BirthTimeStep << ", "<< 
        // ProportionTimeActive << ", "<< 
        // trophicindex << ", "<< 
        // individualReproductivePotentialMass << ", "<< 
        // maturityTimeStep << std::endl;

        modelGrid.GetACell( gridCellIndex ).SetCohortSize( mCohortFunctionalGroupDefinitions.mAllFunctinoalGroupsIndex.size( ) );

        Cohort* NewCohort=new Cohort( modelGrid.GetACell( gridCellIndex ), functionalGroup, cohortJuvenileMass, cohortAdultMass, IndividualBodyMass, NewAbund,
                optimalPreyBodySizeRatio, BirthTimeStep, ProportionTimeActive, mNextCohortID, trophicindex, individualReproductivePotentialMass, maturityTimeStep,
                isAdult, ageMonths, timeStepsJuviline, timeStepsAdult );
        
        totalCohorts++; //  total number of cohorts in the model

        //std::cout << "iterator jj: " << jj << " gridcellindex: " << gridCellIndex << " totalCohorts: " << totalCohorts << endl;
        //std::cout << " " << endl;

        modelGrid.GetACell( gridCellIndex ).mCohorts[functionalGroup].push_back( NewCohort );
    }

    ////////////////////////////////////////////////////////////////////////////////////
    std::vector<double> v;
    for( unsigned jj = 0; jj < nrows; jj++ ) {
        v.push_back(std::stoul(CohortData[0][jj]));
    }
    modelGrid.ApplyFunctionToAllCells( [&]( GridCell & gridCell ) {      
        if(!gridCell.IsMarine()){
            double x = gridCell.GetIndex();
            if(std::find(v.begin(), v.end(), x) == v.end()) {
                //std::cout << "empty cell "<< x << std::endl;
                modelGrid.GetACell( x ).SetCohortSize( mCohortFunctionalGroupDefinitions.mAllFunctinoalGroupsIndex.size( ) );
                Cohort* NewCohort=new Cohort( modelGrid.GetACell( x ), 10, 1, 1, 1, 1,1, 0, 0, mNextCohortID, 2, 0, std::numeric_limits<unsigned>::max( ), 0, 0, 0, 0 );
                modelGrid.GetACell( x ).mCohorts[10].push_back( NewCohort );
                totalCohorts++;
            }
        }
    } );
    return totalCohorts;
}
