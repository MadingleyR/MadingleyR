#include "Madingley.h"
#include "FileWriter.h"
#include "GridDatum.h"
#include "BasicDatum.h"
#include "InputParameters.h"

#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <dirent.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <omp.h>


Madingley::Madingley( ) {

    // Set up list of global diagnostics
    SetUpGlobalDiagnosticsList( );

    // get output dir
    FileWriter fileWriter;
    //mOutputDirectory = fileWriter.GetOutputDirectory( );
    mOutputDirectory = InputParameters::Get( )->GetOutputFolderName( );
    //std::cout << "Output Directory: " << mOutputDirectory << std::endl;
    //std::cout << std::endl;

    // initialise the cohort ID to zero
    mIniTimer.Start( );
    mNextCohortID = 0;
    bool UseMadingleySpinUp = Parameters::Get( )->GetApplyModelSpinup( ); // read yes(1)/no(0) from input csv 
    mParams = MadingleyInitialisation( mNextCohortID, mGlobalDiagnosticVariables["NumberOfCohortsInModel"],
      mGlobalDiagnosticVariables["NumberOfStocksInModel"], mModelGrid, UseMadingleySpinUp );
    mIniTimer.Stop( );
    //std::cout << "Initialisation took: " << mIniTimer.GetElapsedTimeSecs( ) << " (sec)" << std::endl;

    // lookup vars
    mDispersalSet = new DispersalSet( );
    mStockLeafStrategy = mParams.mStockFunctionalGroupDefinitions.mTraitLookupFromIndex[ "leaf strategy" ];
    mCohortNutritionSource = mParams.mCohortFunctionalGroupDefinitions.mTraitLookupFromIndex[ "nutrition source" ];
    mCohortThermoregulation = mParams.mCohortFunctionalGroupDefinitions.mTraitLookupFromIndex[ "endo/ectotherm" ];
    mCohortReproductiveStrategy = mParams.mCohortFunctionalGroupDefinitions.mTraitLookupFromIndex[ "reproductive strategy" ];


    TypeOfRun = InputParameters::Get( )->GetTypeOfRun();

    if(TypeOfRun=="spin"){
        // initialize model output
        WriteModelState writemodelstate; 
        writemodelstate.CreateOutputSubfolders( mOutputDirectory, TypeOfRun ); // creat output subdirs
        writemodelstate.WriteCohortProperties( mModelGrid, mOutputDirectory, 99999 ); // write cohort properties
        writemodelstate.WriteStockProperties( mModelGrid, mOutputDirectory, 99999 ); // write stock properties
    }

}

void Madingley::Run( ) {

    // get write flags
    TimestepWritingBinnedCohortStatistics = InputParameters::Get( )->GetTimestepWritingBinnedCohortStatistics();
    TimestepWritingFullCohortProperties = InputParameters::Get( )->GetTimestepWritingFullCohortProperties();
    TimestepWritingPreyBinnedFoodwebConnections = InputParameters::Get( )->GetTimestepWritingPreyBinnedFoodwebConnections(); // slows down code, tracking in EatingCarnivore.cpp
    TimestepWritingStockProperties = InputParameters::Get( )->GetTimestepWritingStockProperties();

    // store TotalTime
    double TotalTime = 0;
    int SimulationInMonths_print = Parameters::Get( )->GetLengthOfSimulationInMonths( );
    mDispersals = 0;

    // new create vector with relevent grid indices (terrestrial only)
    std::vector<int> TerrestrialGridcellIndices; unsigned cellCounter = 0;
    mModelGrid.ApplyFunctionToAllCells( [&]( GridCell & gcl ) {
        if ( !gcl.IsMarine( ) ) {
            TerrestrialGridcellIndices.push_back( gcl.GetIndex( ) );
            cellCounter++;
        } 
    } );

    // std::cout << "Running terrestrial only ( "<< cellCounter << " of the " << 
    // Parameters::Get( )->GetNumberOfGridCells( ) << " gridcells )"<< std::endl;
    // std::cout << std::endl;

    // store biomass diagnostics
    int BinnedStatCounter = 0;
    std::vector<double> AldMassBins = {0.0001,0.0010,0.0100,0.1000,1.0000,10.0000,100.0000,1000.0000,10000.0000}; // round(10^seq(-4,4,1),4)
    std::vector< std::vector<double> > StableBiomass( SimulationInMonths_print ,std::vector<double>( mParams.mCohortFunctionalGroupDefinitions.GetNumberOfFunctionalGroups( ) ));

    // initialize model output
    WriteModelState writemodelstate; 
    writemodelstate.CreateOutputSubfolders( mOutputDirectory, "run" ); // creat output subdirs

    WriteGridProperties = InputParameters::Get( )->GetWriteGridProperties();
    if(WriteGridProperties==1) {
      for( unsigned t = 0; t < 12; t++){
        TimeStep::Get( )->SetMonthly( t );
        writemodelstate.WriteSpatialGridProperties( mModelGrid, mOutputDirectory ); // write spatial grid properties
      }
      
    }

    double recDispersalTimer, recEcologyTimer = 0;


    //## check if the model should by run without dispersal
    int RunWithoutDispersal = InputParameters::Get( )->GetRunWithoutDispersal();
    //##

    //## check if the model should by run in parallel
    int RunInParallel = InputParameters::Get( )->GetRunInParallel();
    //##



    //########### Run the model
    for( unsigned timeStep = 0; timeStep < Parameters::Get( )->GetLengthOfSimulationInMonths( ); timeStep += 1 ) {
      
        TimeStep::Get( )->SetMonthly( timeStep ); // set current month of the year
        mCurrentTimeStep = timeStep; // get current time 
        mCurrentMonth = mCurrentTimeStep % 12; // get current month of the year
        //std::cout << mCurrentMonth << std::endl;
        double temperature_temp;
        mModelGrid.ApplyFunctionToAllCells( [&]( GridCell & gcl ) {
            if ( !gcl.IsMarine( ) ) {
                temperature_temp += gcl.GetGridcellNearSurfaceTemp( mCurrentMonth );
            } 
        } );
        temperature_temp = temperature_temp/12;
        PrintToRConsole(timeStep,SimulationInMonths_print,recEcologyTimer,recDispersalTimer,temperature_temp); //output timestep info
      
        ////////////----- WITHIN GRIDCELL ECOLOGY -------------------------------------
        mEcologyTimer.Start( );
        ResetCohortConsumptionVectors( mModelGrid ); // reset consumption cohort vectors 
        //Environment::Update( mCurrentMonth ); 
        
        if(RunInParallel==1){
            RunWithinCellsInParallel( cellCounter, TerrestrialGridcellIndices ); // run in gridcell ecology in parallel 
        }else{
            RunWithinCells( cellCounter, TerrestrialGridcellIndices ); // run in gridcell ecology serial
        }
        
        if( timeStep > 0 ) CalculateTrophicIndex( ); // Calculate cohort trophic index
        mEcologyTimer.Stop( );
        recEcologyTimer = mEcologyTimer.GetElapsedTimeSecs( );
        //std::cout << "Within grid ecology took: " << mEcologyTimer.GetElapsedTimeSecs( ) << std::endl;
        ////////////----- WITHIN GRIDCELL ECOLOGY -------------------------------------
        
        ////////////----- BETWEEN GRIDCELL ECOLOGY -------------------------------------
        mDispersalTimer.Start( );
        std::vector<double> OutputDispersalVector; // output dispersal vector
        if(RunWithoutDispersal==0) {
            OutputDispersalVector = RunCrossGridCellEcology( mDispersals ); // run dispersal
        }
        mDispersalTimer.Stop( );
        recDispersalTimer = mDispersalTimer.GetElapsedTimeSecs( );
        //std::cout << "Across grid ecology took: " << mDispersalTimer.GetElapsedTimeSecs( ) << std::endl;
        ////////////----- BETWEEN GRIDCELL ECOLOGY -------------------------------------

        ////////////----- OUTPUT WRITING -------------------------------------
        mOutputTimer.Start( );
        StableBiomass[ timeStep ] = CalculateOutputBiomass( timeStep, mParams.mCohortFunctionalGroupDefinitions.GetNumberOfFunctionalGroups( ) );  // cohort biomass statistics    
        writemodelstate.WriteMonthlyTimeLineBiomass( mOutputDirectory, timeStep, StableBiomass ); // write timeline
        writemodelstate.WriteStockTimeline( mModelGrid, mOutputDirectory, timeStep ); 

        if( timeStep >= TimestepWritingFullCohortProperties ) {
            // write cohort properties
            writemodelstate.WriteCohortProperties( mModelGrid, mOutputDirectory, timeStep ); 
        }
        if( timeStep >= TimestepWritingStockProperties ) { 
            // write stock properties
            writemodelstate.WriteStockProperties( mModelGrid, mOutputDirectory, timeStep ); 
        }
        if( timeStep >= TimestepWritingPreyBinnedFoodwebConnections ) {
            // write foodweb connections
            writemodelstate.WriteBinnedPreyFoodWebConnections( mModelGrid, timeStep, mOutputDirectory );
        }
        if( timeStep >= TimestepWritingPreyBinnedFoodwebConnections ) {
            // write cohort consumption summary
            writemodelstate.WriteCohortConsumptionSummary( mModelGrid, timeStep, mOutputDirectory ); 
        }
        if( timeStep >= TimestepWritingBinnedCohortStatistics ) {
            // write cohort consumption summary
            writemodelstate.WriteCohortBiomassBinned( mModelGrid, timeStep, mOutputDirectory, 
            mParams.mCohortFunctionalGroupDefinitions.GetFunctionalGroupIndex( "Heterotroph/Autotroph", "heterotroph", false ).size() );  
        }
        
        mOutputTimer.Stop( );
        ////////////----- OUTPUT WRITING -------------------------------------

        TotalTime += mEcologyTimer.GetElapsedTimeSecs( ) + mOutputTimer.GetElapsedTimeSecs( ) + mDispersalTimer.GetElapsedTimeSecs( ); // Check model runtime
        //std::cout << "Current total model runtime: " << TotalTime << " seconds (" << TotalTime/60/60 << " hours)" << std::endl;
    
    }
    //########### End run the model

    // write files to re-run model with
    writemodelstate.WriteCohortProperties( mModelGrid, mOutputDirectory, 99999 ); // write cohort properties
    writemodelstate.WriteStockProperties( mModelGrid, mOutputDirectory, 99999 ); // write stock properties

}

void Madingley::RunWithinCells( unsigned cellCounter, std::vector<int> TerrestrialGridcellIndices ) {
    // Instantiate a class to hold thread locked global diagnostic variables
    ThreadVariables singleThreadDiagnostics( 0, 0, 0, mNextCohortID );

    mModelGrid.ApplyFunctionToAllCells( [&]( GridCell & gcl ) {

        RunWithinCellStockEcology( gcl );
        RunWithinCellCohortEcology( gcl, singleThreadDiagnostics );

    } );
    // Update the variable tracking cohort unique IDs
    mNextCohortID = singleThreadDiagnostics.mNextCohortID;

    // Take the results from the thread local variables and apply to the global diagnostic variables
    mGlobalDiagnosticVariables["NumberOfCohortsExtinct"] = singleThreadDiagnostics.mExtinctions - singleThreadDiagnostics.mCombinations;
    mGlobalDiagnosticVariables["NumberOfCohortsProduced"] = singleThreadDiagnostics.mProductions;
    mGlobalDiagnosticVariables["NumberOfCohortsInModel"] = mGlobalDiagnosticVariables["NumberOfCohortsInModel"] + singleThreadDiagnostics.mProductions - singleThreadDiagnostics.mExtinctions;
    mGlobalDiagnosticVariables["NumberOfCohortsCombined"] = singleThreadDiagnostics.mCombinations;
}



void Madingley::RunWithinCellsInParallel( unsigned cellCounter, std::vector<int> TerrestrialGridcellIndices ) {
    // Instantiate a class to hold thread locked global diagnostic variables

    #ifdef _OPENMP
    //std::cout<<"Running RunWithinCellsInParallel..."<<endl;
    double startTimeTest = omp_get_wtime( );
    #endif

    list<ThreadVariables> partialsDiagnostics;
    unsigned gridCellIndexS;

    #pragma omp parallel num_threads(omp_get_num_procs()) shared(partialsDiagnostics)
    {
        ThreadVariables singleThreadDiagnostics( 0, 0, 0, mNextCohortID );

        #pragma omp for schedule(dynamic)
        for( unsigned gridCellIndex = 0; gridCellIndex < Parameters::Get( )->GetNumberOfGridCells( ); gridCellIndex++ )
        {   
            RunWithinCellStockEcology( mModelGrid.GetACell( gridCellIndex) );
            RunWithinCellCohortEcology( mModelGrid.GetACell( gridCellIndex ), singleThreadDiagnostics ); 
        }
        partialsDiagnostics.push_back(singleThreadDiagnostics);
    }//end parallel

    ThreadVariables globalDiagnostics( 0, 0, 0, mNextCohortID);
    for (list<ThreadVariables>::iterator it=partialsDiagnostics.begin(); it != partialsDiagnostics.end(); it++)
    {
        ThreadVariables tmp=*it;
        globalDiagnostics.mProductions+=tmp.mProductions;
        globalDiagnostics.mExtinctions+=tmp.mExtinctions;
        globalDiagnostics.mCombinations+=tmp.mCombinations;
    }

    // Update the variable tracking cohort unique IDs
    mNextCohortID = globalDiagnostics.mNextCohortID;

    // Take the results from the thread local variables and apply to the global diagnostic variables
    mGlobalDiagnosticVariables["NumberOfCohortsExtinct"] = globalDiagnostics.mExtinctions - globalDiagnostics.mCombinations;
    mGlobalDiagnosticVariables["NumberOfCohortsProduced"] = globalDiagnostics.mProductions;
    mGlobalDiagnosticVariables["NumberOfCohortsInModel"] = mGlobalDiagnosticVariables["NumberOfCohortsInModel"] + globalDiagnostics.mProductions - globalDiagnostics.mExtinctions;
    mGlobalDiagnosticVariables["NumberOfCohortsCombined"] = globalDiagnostics.mCombinations;

    #ifdef _OPENMP
    double endTimeTest = omp_get_wtime( );
    //std::cout << "RunWithinCellsInParallel( ) took: " << endTimeTest - startTimeTest << endl;
    #endif

}

void Madingley::PrintToRConsole( unsigned timeStep, int SimulationInMonths_print, double timer1, double timer2, double env_var ){

    if( timeStep == 0 ){
		
        std::cout << "Running time step " << timeStep + 1 << " / " << SimulationInMonths_print << " (months)" <<
                     ", n cohorts: " << mGlobalDiagnosticVariables["NumberOfCohortsInModel"] << std::endl;
					 
    }else if( ( timeStep + 1 ) < SimulationInMonths_print ){
		
        std::cout << "Running time step " << timeStep + 1 << " / " << SimulationInMonths_print << " (months)" <<
                     ", n cohorts: " << mGlobalDiagnosticVariables["NumberOfCohortsInModel"] <<
                     ", time step took: " << timer1+timer2 << " (sec)" << std::endl; 
					 
    }else{
		
        std::cout << "Running time step " << timeStep + 1 << " / " << SimulationInMonths_print << " (months)" <<
                ", n cohorts: " << mGlobalDiagnosticVariables["NumberOfCohortsInModel"] <<
                ", time step took: " << timer1+timer2 << " (sec)" << std::endl; 
				
    }
	
}

void Madingley::RunWithinCellStockEcology( GridCell& gcl ) {

    if ( !gcl.IsMarine( ) ) {
        // Create a local instance of the stock ecology class
        EcologyStock MadingleyEcologyStock;
        // Get the list of functional group indices for autotroph stocks
        std::vector<int> AutotrophStockFunctionalGroups = mParams.mStockFunctionalGroupDefinitions.GetFunctionalGroupIndex( "Heterotroph/Autotroph", "Autotroph", false );
        // Loop over autotroph functional groups
        for( unsigned FunctionalGroup: AutotrophStockFunctionalGroups ) {
            for( auto& ActingStock: gcl.mStocks[FunctionalGroup] ) {

                // Run stock ecology
                MadingleyEcologyStock.RunWithinCellEcology( gcl, ActingStock, mCurrentTimeStep, mCurrentMonth, mParams );
            }
        }
    }

}

void Madingley::RunWithinCellCohortEcology( GridCell& gcl, ThreadVariables& partial ) {
    // Local instances of classes
    // Initialize ecology for stocks and cohorts - needed fresh every timestep?
    
    // get functional group indices
    std::vector<int> FunctionalGroupIndicesToEat = mParams.mCohortFunctionalGroupDefinitions.GetFunctionalGroupIndex( "Heterotroph/Autotroph", "heterotroph", false );
    int maxFunctionalGroupIndicesToEat = FunctionalGroupIndicesToEat[FunctionalGroupIndicesToEat.size()-1]+1;

    // predation speed up bins
    std::vector<double> Bins;
    for(int i = 0; i < 15; i++) Bins.push_back(pow(10,(i-4)));

    if ( !gcl.IsMarine( ) ) {

        // track cohort juviline and adult age in months
        gcl.ApplyFunctionToAllCohorts( [&]( Cohort* c ) {
            c->mAgeMonths = c->mAgeMonths + 1;
            if(c->mIsAdult == 0) {
                c->mTimeStepsJuviline++;
            }else{
                c->mTimeStepsAdult++;
            }
        } );

        EcologyCohort mEcologyCohort;
        mEcologyCohort.InitialiseEating( gcl, mParams );
        Activity CohortActivity;

        gcl.ApplyFunctionToAllCohortsWithStaticRandomness( [&]( Cohort* c ) {

            if( gcl.mCohorts[c->mFunctionalGroupIndex].size( ) != 0 && c->mCohortAbundance > Parameters::Get( )->GetExtinctionThreshold( ) ) {
                CohortActivity.AssignProportionTimeActive( gcl, c, mCurrentTimeStep, mCurrentMonth, mParams );
    
                mEcologyCohort.RunWithinCellEcology( gcl, c, mCurrentTimeStep, partial, mCurrentMonth, mParams ); // Run ecology
                
                mEcologyCohort.UpdateEcology( gcl, c, mCurrentTimeStep ); // Update the properties of the acting cohort
                
                Cohort::ResetMassFluxes( );
                
                assert( c->mIndividualBodyMass >= 0.0 && "Biomass < 0 for this cohort" ); // Check if mass is still >= 0
            }
            if( gcl.mCohorts[c->mFunctionalGroupIndex].size( ) > 0 ) assert( c->mIndividualBodyMass >= 0.0 && "Biomass < 0 for this cohort" ); // Check if mass is still >= 0

        }, mCurrentTimeStep );
        

        for( auto c: GridCell::mNewCohorts ) {
            gcl.InsertCohort( c );
            if( c->mDestinationCell != &gcl ) std::cout << "whut? wrong cell?" << std::endl;
        }
        partial.mProductions += GridCell::mNewCohorts.size( );
        GridCell::mNewCohorts.clear( );

        RunExtinction( gcl, partial );

        // Merge cohorts, if necessary
        if( gcl.GetNumberOfCohorts( ) > Parameters::Get( )->GetMaximumNumberOfCohorts( ) ) {
            mCohortMerger.ResetRandom( );
            partial.mCombinations += mCohortMerger.MergeToReachThresholdFast( gcl, mParams );

            //Run extinction a second time to remove those cohorts that have been set to zero abundance when merging
            RunExtinction( gcl, partial );
        }

    }
}

void Madingley::RunExtinction( GridCell& gcl, ThreadVariables& partial ) {

    // Loop over cohorts and remove any whose abundance is below the extinction threshold
    std::vector<Cohort*> CohortsToRemove;
    gcl.ApplyFunctionToAllCohorts( [&]( Cohort* c ) {
        if( c->mCohortAbundance - Parameters::Get( )->GetExtinctionThreshold( ) <= 0 || c->mIndividualBodyMass <= 0 ) {
            CohortsToRemove.push_back( c );
            partial.mExtinctions += 1;
        }
    } );
    
    //std::cout << " cohorts extinct: "<< CohortsToRemove.size() << std::endl;

    // Code to add the biomass to the biomass pool and dispose of the cohort
    for( auto c: CohortsToRemove ) {

        // Add biomass of the extinct cohort to the organic matter pool
        double deadMatter = ( c->mIndividualBodyMass + c->mIndividualReproductivePotentialMass ) * c->mCohortAbundance;
        if( deadMatter < 0 ) std::cout << "Dead " << deadMatter << std::endl;
        //Environment::Get( "Organic Pool", c->GetCurrentCell( ) ) += deadMatter;
        gcl.mGridcellOrganicPool += deadMatter;
        assert( gcl.mGridcellOrganicPool >= 0 && "Organic pool < 0" );

        // Remove the extinct cohort from the list of cohorts
        gcl.RemoveCohort( c );
    }
    for( auto c: CohortsToRemove ) {delete(c);}
}

std::vector<double> Madingley::RunCrossGridCellEcology( unsigned& dispersals ) {

    //std::cout << "Running dispersal between grid cells" << std::endl;

    // Loop through each grid cell, and run dispersal for each.
    // In the original model a new dispersal object is made every timestep - this resets the random number generators
    mDispersalSet->ResetRandoms( );
    mModelGrid.ApplyFunctionToAllCells( [&]( GridCell & c ) {
        mDispersalSet->RunCrossGridCellEcologicalProcess( c, mModelGrid, mParams, mCurrentMonth );
    } );

    int cellCounter = 0;
    std::vector<double> DispersalVector(Parameters::Get( )->GetNumberOfGridCells( ));
    mModelGrid.ApplyFunctionToAllCells( [&]( GridCell & gridCell ) {
        DispersalVector[cellCounter] = 0;
        gridCell.ApplyFunctionToAllCohorts( [&]( Cohort* c ) {
            if( c->IsMoving( ) ) {
                //std::cout << "cohort dispersed" << std::endl;
                DispersalVector[cellCounter]+=1;
            }
        } );
        cellCounter++;
    } );

    // Apply the changes from dispersal
    mDispersalSet->UpdateCrossGridCellEcology( dispersals );
 
    return DispersalVector;
}

void Madingley::SetUpGlobalDiagnosticsList( ) {
    // Add global diagnostic variables
    mGlobalDiagnosticVariables["NumberOfCohortsExtinct"] = 0.0;
    mGlobalDiagnosticVariables["NumberOfCohortsProduced"] = 0.0;
    mGlobalDiagnosticVariables["NumberOfCohortsCombined"] = 0.0;
    mGlobalDiagnosticVariables["NumberOfCohortsInModel"] = 0.0;
    mGlobalDiagnosticVariables["NumberOfStocksInModel"] = 0.0;
    mGlobalDiagnosticVariables["ExtAdultMass"] = 999999.0; //#
    mGlobalDiagnosticVariables["NumberOfCohortForcedToExtinction"] = 0.0; //#
}

std::vector<double> Madingley::CalculateOutputBiomass( unsigned step, unsigned n_fg ) {
    std::vector<double> Current_Biomass(n_fg, 0.0);
    mModelGrid.ApplyFunctionToAllCells( [&]( GridCell & gridCell ) {
        gridCell.ApplyFunctionToAllCohorts( [&]( Cohort* c ) {
            double cohortBiomass = ( c->mIndividualBodyMass + c->mIndividualReproductivePotentialMass ) * c->mCohortAbundance / 1000.;
            Current_Biomass[c->mFunctionalGroupIndex] += cohortBiomass;
        } );
    } );
    
    return Current_Biomass;
}

void Madingley::CalculateTrophicIndex( ) {
    // Calculate trophic index based on cohort consumption
    mModelGrid.ApplyFunctionToAllCells( [&]( GridCell & gridCell ) {
        gridCell.ApplyFunctionToAllCohorts( [&]( Cohort* c ) {
            c->mConsumed_Total = c->mConsumed_Autotroph + c->mConsumed_Herbivore + c->mConsumed_Omnivore + c->mConsumed_Carnivore;
            if( c->mConsumed_Total > 0.0 ) {
                c->mTrophicIndex = 1 + 
                c->mConsumed_Autotroph/c->mConsumed_Total + 
                c->mConsumed_Herbivore/c->mConsumed_Total * 2 + 
                c->mConsumed_Omnivore/c->mConsumed_Total * 2.5 + 
                c->mConsumed_Carnivore/c->mConsumed_Total * 3; 
            }        
            //std::cout << c->mTrophicIndex << std::endl;
        } );
    } );

}

void Madingley::ResetCohortConsumptionVectors( Grid& mModelGrid ) {
    mModelGrid.ApplyFunctionToAllCells( [&]( GridCell & gridCell ) {
        gridCell.ApplyFunctionToAllCohorts( [&]( Cohort* c ) {
            c->mConsumed_Autotroph = 0.0;
            c->mConsumed_Herbivore = 0.0;
            c->mConsumed_Omnivore  = 0.0;
            c->mConsumed_Carnivore = 0.0;   
            
            std::vector<float> allocFLOAT(11,0.0f);
            c->mConsumed_HerbivoreBIN_ecto = allocFLOAT;
            c->mConsumed_HerbivoreBIN_endo = allocFLOAT;
            c->mConsumed_OmnivoreBIN_ecto = allocFLOAT;
            c->mConsumed_OmnivoreBIN_endo = allocFLOAT;
            c->mConsumed_CarnivoreBIN_ecto = allocFLOAT;
            c->mConsumed_CarnivoreBIN_endo = allocFLOAT;
        } );
    } );
}
