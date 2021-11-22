#ifndef MADINGLEY
#define	MADINGLEY

#include "MadingleyInitialisation.h"
#include "FunctionalGroupDefinitions.h"
#include "Stopwatch.h"
#include "CohortMerger.h"
#include "Grid.h"
#include "GridCell.h"
#include "DispersalSet.h"
#include "EcologyStock.h"
#include "EcologyCohort.h"
#include "Activity.h"
#include "ThreadVariables.h"
#include "TimeStep.h"
#include "Maths.h"
#include "Parameters.h"
#include "DataRecorder.h"
#include "Types.h"
#include "WriteModelState.h"
//#include "Environment.h"
//#include "LoadCSVEnvironment.h"


/** \brief The ecosystem model */
class Madingley {
public:
    /** \brief   Initializes the ecosystem model
    @param initialisationFileName The name of the file with model parameters
    @param OutputPath Where the output will be stored */
    Madingley( );
    /** \brief  Run the global ecosystem model */
    void Run( );

    // get write flags
    int TimestepWritingBinnedCohortStatistics;
    int TimestepWritingFullCohortProperties;
    int TimestepWritingPreyBinnedFoodwebConnections; // slows down code, tracking in EatingCarnivore.cpp
    int TimestepWritingStockProperties;

    // get additional input flags
    int Xmin;
    int Xmax;
    int Ymin;
    int Ymax;
    int TimeStep_Months_Calc_FoodWeb;
    std::vector <std::vector<double>> eGV_nst; 
    std::string TypeOfRun;
    int WriteGridProperties; //(yes=1/no=0)

    int UseNonDefaultModelParameters;

private:
    /** \brief  Run processes for cells*/
    void RunWithinCells( unsigned cellCounter, std::vector<int> TerrestrialGridcellIndices );

    /** \brief   Run ecological processes for stocks in a specified grid cell
    @param gcl The current cell */
    void RunWithinCellStockEcology( GridCell& );
    /** \brief   Run ecological processes for cohorts in a specified grid cell
    @param gcl Reference to the current grid cell
    @param partial Track some global variables pertaining to cohort numbers etc. */
    void RunWithinCellCohortEcology( GridCell&, ThreadVariables& );
    /** \brief Carries out extinction on cohorts that have an abundance below a defined extinction threshold */
    void RunExtinction( GridCell&, ThreadVariables& );
    /** \Megafauna extinction, runs within cohort ecology */ 
    void RandomExtinction_v1( double, double, int );



    /** \brief  Run in parallel processes for cells*/
    void RunWithinCellsInParallel( unsigned cellCounter, std::vector<int> TerrestrialGridcellIndices );



    /** \brief Run ecological processes that operate across grid cells */
    std::vector<double>  RunCrossGridCellEcology( unsigned& );
    void SetUpGlobalDiagnosticsList( );
    
    //# changed. returns biomass diagnostics
    std::vector<double> CalculateOutputBiomass( unsigned, unsigned );

    /** \brief An instance of ModelGrid to hold the grid to be used in this model */
    Grid mModelGrid;
    GridCell mGridCell;
    MadingleyInitialisation mParams;
    /** \brief An instance of the merging class */
    CohortMerger mCohortMerger;
    /** \brief Instance of the class to perform general functions */
    UtilityFunctions mUtilities;
    /** \brief An instance of StopWatch to time individual time steps */
    Stopwatch mEcologyTimer;
    Stopwatch mIniTimer;
    Stopwatch mDispersalTimer;
    Stopwatch mOutputTimer;
    Stopwatch mMergeTimer;
    Types::DispersalSetPointer mDispersalSet; //FIX - Does this need to be a pointer?
    /** \brief A list of global diagnostics for this model run */
    Types::DoubleMap mGlobalDiagnosticVariables;
    Types::StringVector mStockLeafStrategy;
    Types::StringVector mCohortNutritionSource;
    Types::StringVector mCohortThermoregulation;
    Types::StringVector mCohortReproductiveStrategy;

    /** A variable to increment for the purposes of giving each cohort a unique ID */
    long long mNextCohortID;
    /** \brief The current time step */
    unsigned mCurrentTimeStep;
    /** \brief The current month: 1=Jan; 2=Feb; 3=Mar etc. */
    unsigned mCurrentMonth;
    /** \brief Variable to track the number of cohorts that have dispersed. Doesn't need to be thread-local because all threads have converged prior to running cross-grid-cell processes */
    unsigned mDispersals;

    //#
    bool RunParallel;
    std::string mOutputDirectory;

    void CalculateTrophicIndex( );

    void ResetCohortConsumptionVectors( Grid& );

    void PrintToRConsole( unsigned, int, double, double, double );

};

#endif
