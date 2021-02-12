#ifndef GRIDCELL
#define GRIDCELL

#include "UtilityFunctions.h"
#include "Stock.h"
#include "Cohort.h"
//#include "Environment.h"
#include "Parameters.h"
#include "DataIndices.h"
#include "Convertor.h"
#include "ClimateVariablesCalculator.h"

#include <chrono>       // std::chrono::system_clock
#include "NonStaticSimpleRNG.h"

class GridCell {
public:
    
    GridCell( );

    void SetCellCoords( unsigned );

    double GetRealm( );
    bool IsMarine( );

    void InsertCohort( Cohort* );
    void RemoveCohort( Cohort* );
    void MoveCohort( Cohort* );
    void CheckGone( Cohort* );
    unsigned GetIndex( ) const;
    unsigned GetLatitudeIndex( ) const;
    unsigned GetLongitudeIndex( ) const;
    double GetCellArea( ) const;
    double GetCellHeight( ) const;
    double GetCellWidth( ) const;
    void SetCohortSize( unsigned );

    double mGridcellOrganicPool;
    double mGridcellRespiratoryCO2Pool;
    
    //---Environmental set layers--------------------
    void SetGridcellRealmClassification( double );
    void SetGridcellAvailableWaterCapacity( double );
    void SetGridcellLandMask( int );
    void SetGridcellHanpp( double );
    void SetGridcellDiurnalTemperatureRange( std::vector<double> );
    void SetGridcellNearSurfaceTemp( std::vector<double> );
    void SetGridcellGroundFrostFrequency( std::vector<double> );
    void SetGridcellPrecipitation( std::vector<double> );
    void SetGridcellTerrestrialNPP( std::vector<double> );
    
    void Set_Ecto_max( double );
    void Set_Endo_C_max( double );
    void Set_Endo_H_max( double );
    void Set_Endo_O_max( double );
    
    void SetNPPSeasonalityAndBreeding( std::vector<double> ); 
    void SetAVGSDTemp( int, std::vector<double> );
    void SetFrostandFire( int, double, std::vector<double>, std::vector<double>, std::vector<double> );
    //-----------------------------------------------

    //---Environmental get layers--------------------
    double GetGridcellRealmClassification( ) const;
    int GetGridcellLandMask( ) const;
    double GetGridcellAvailableWaterCapacity( ) const;
    double GetGridcellHanpp( ) const;
    double GetGridcellDiurnalTemperatureRange( int ) const;
    double GetGridcellNearSurfaceTemp( int ) const;
    double GetGridcellGroundFrostFrequency( int ) const;
    double GetGridcellPrecipitation( int ) const;
    double GetGridcellTerrestrialNPP( int ) const;

    double Get_Ecto_max( ) const;
    double Get_Endo_C_max( ) const;
    double Get_Endo_H_max( ) const;
    double Get_Endo_O_max( ) const;
    
    double GetGridcellTotalPrecip( ) const;
    double GetGridcellSeasonality( int ) const;
    double GetGridcellBreedingSeason( int ) const;
    double GetGridcellAnnualTemperature( ) const;
    double GetGridcellSDTemperature( ) const;
    double GetGridcellExpTDevWeight( int ) const;
    double GetGridcellAET( int ) const;
    double GetGridcellTotalAET( ) const;
    double GetGridcellFractionMonthFrost( int ) const;
    double GetGridcellFractionYearFrost( ) const;
    double GetGridcellFractionYearFire( ) const;
    //-----------------------------------------------
      
    /** \brief Gets the number of cohorts in this grid cell */
    unsigned GetNumberOfCohorts( );

    static std::vector<Cohort*> mNewCohorts;
    #pragma omp threadprivate(mNewCohorts)

    template <typename F>
    void ApplyFunctionToAllCohorts( F f ) {
        for( int index = 0; index < mCohorts.size( ); index++ ) {
            // Work through the list of cohorts
            for( Cohort* c : mCohorts[ index ] ) {
                f( c );
            }
        }
    }

    template <typename F>
    void ApplyFunctionToAllCohortsWithStaticRandomness( F f, int CurrentTimeStep ) {
        std::vector<unsigned > RandomCohortOrder;
        std::vector<std::pair<int, int> > indexedList;
        unsigned TotalCohorts = 0;
        
        for( int functionalTypeIndex = 0; functionalTypeIndex < mCohorts.size( ); functionalTypeIndex++ ) {
            // Work through the list of cohorts and create a list of pairs so as to be able to lookup cohorts easily
            for( int cohortNum = 0; cohortNum < mCohorts[ functionalTypeIndex ].size( ); cohortNum++ ) {
                indexedList.push_back( std::make_pair( functionalTypeIndex, cohortNum ) );
                TotalCohorts++;
            }
        }

        //despite the name of this utility, it actually returns a random list, but with a given fixed seed
        RandomCohortOrder = mUtilities.NonRandomlyOrderedCohorts( TotalCohorts, CurrentTimeStep );

        for( int i = 0; i < RandomCohortOrder.size( ); i++ ) {
            Cohort* c = mCohorts[indexedList[RandomCohortOrder[i]].first][indexedList[RandomCohortOrder[i]].second];
            f( c );
        }
    }

    template <typename F>
    void ApplyFunctionToAllStocks( F f ) {
        for( int index = 0; index < mStocks.size( ); index++ ) {
            // Work through the list of cohorts
            for( Stock& s : mStocks[ index ] ) {
                f( s );
            }
        }
    }

    vector< vector<Cohort*> > mCohorts;

    Types::StocksMap mStocks;



private:
    void RandomizeCohorts( );

    NonStaticSimpleRNG mRandomNumber;

    UtilityFunctions mUtilities;
    unsigned mIndex;
    unsigned mLatitudeIndex;
    unsigned mLongitudeIndex;
    double mCellArea;
    double mCellHeightKm;
    double mCellWidthKm;
    
    // Environmental layers (direct import)
    double mGridcellRealmClassification;
    double mGridcellAvailableWaterCapacity;
    int mGridcellLandMask;
    double mGridcellHanpp;
    std::vector<double> mGridcellDiurnalTemperatureRange;
    std::vector<double> mGridcellNearSurfaceTemp;
    std::vector<double> mGridcellGroundFrostFrequency;
    std::vector<double> mGridcellPrecipitation;
    std::vector<double> mGridcellTerrestrialNPP;
    
    // Environmental layers (deduced from imports)
    double mGridcellTotalPrecip;
    std::vector<double> mGridcellSeasonality;
    std::vector<double> mGridcellBreedingSeason;
    double mGridcellAnnualTemperature;
    double mGridcellSDTemperature;
    std::vector<double> mGridcellExpTDevWeight;
    std::vector<double> mGridcellAET;
    double mGridcellTotalAET;
    std::vector<double> mGridcellFractionMonthFrost;
    double mGridcellFractionYearFrost;
    double mGridcellFractionYearFire;
    
    double m_Ecto_max;
    double m_Endo_C_max;
    double m_Endo_H_max;
    double m_Endo_O_max;

};
#endif
