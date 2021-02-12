#ifndef PARAMETERS
#define PARAMETERS

#include "Types.h"

class Parameters {
public:

    ~Parameters( );
    static Types::ParametersPointer Get( );

    bool Initialise( const Types::StringMatrix& );

    // User defined parameters
    std::string GetRootDataDirectory( ) const;
    std::string GetTimeStepUnits( ) const;
    unsigned GetLengthOfSimulationInYears( ) const;
    int GetUserMinimumLatitude( ) const;
    int GetUserMaximumLatitude( ) const;
    int GetUserMinimumLongitude( ) const;
    int GetUserMaximumLongitude( ) const;
    float GetGridCellSize( ) const;
    float GetExtinctionThreshold( ) const;
    unsigned GetMaximumNumberOfCohorts( ) const;
    float GetPlanktonSizeThreshold( ) const;
    bool GetDrawRandomly( ) const;

    std::string GetHumanNPPScenarioType( ) const;
    double GetHumanNPPExtractionScale( ) const;
    double GetHumanNPPScenarioDuration( ) const;
    unsigned GetBurninSteps( ) const;
    unsigned GetImpactSteps( ) const;
    unsigned GetRecoverySteps( ) const;
    
    
    //############################################
    unsigned GetRunParallel( ) const;
    unsigned GetThreadNumber( ) const;

    std::string GetSpatialSelection( ) const;

    //# require model ini preferences
    unsigned GetApplyModelSpinup( ) const;
    std::string GetCohortCSVLocation( ) const;
    std::string GetStockCSVLocation( ) const;
    std::string GetPoolCSVLocation( ) const;
    unsigned GetRestartedFromTimeStep( ) const;


    //# csv outputs
    unsigned GetWriteCohortSpecifics( ) const;
    unsigned GetWriteCohortConsumption( ) const;
    unsigned GetWriteConsumptionSummary( ) const;

    unsigned GetWriteStateInModelTime( ) const;
    unsigned GetWriteStateOverwrite( ) const;
    double GetWriteStateInterval( ) const;
    std::string GetWriteStateIntervalUnit( ) const;
    //############################################
    
    void SetRootDataDirectory( const std::string& );
    void SetTimeStepUnits( const std::string& );
    void SetLengthOfSimulationInMonths( const unsigned& );
    void SetUserMinimumLongitude( const int& );
    void SetUserMaximumLongitude( const int& );
    void SetUserMinimumLatitude( const int& );
    void SetUserMaximumLatitude( const int& );
    void SetGridCellSize( const float& );
    void SetExtinctionThreshold( const float& );
    void SetMaximumNumberOfCohorts( const unsigned& );
    void SetPlanktonSizeThreshold( const float& );
    void SetDrawRandomly( const std::string& );

    void SetHumanNPPScenarioType(const std::string& humanNPPScenarioType);
    void SetHumanNPPExtractionScale(const double& humanNPPExtractionScale );
    void SetHumanNPPScenarioDuration(const double & humanNPPScenarioDuration);
    void SetBurninSteps(const unsigned& burninSteps);
    void SetImpactSteps(const unsigned& impactSteps);
    void SetRecoverySteps(const unsigned& recoverySteps);
    
    //############################################
    void SetRunParallel(const unsigned& runParallel);
    void SetThreadNumber(const unsigned& threadNumber);

    void SetSpatialSelection( const std::string& );

    //# require model ini preferences
    void SetApplyModelSpinup( const unsigned& applyModelSpinup);
    void SetCohortCSVLocation( const std::string& cohortCSVLocation);
    void SetStockCSVLocation( const std::string& stockCSVLocation);
    void SetPoolCSVLocation( const std::string& poolCSVLocation);
    void SetRestartedFromTimeStep( const unsigned& applyModelSpinup);

    //# csv outputs
    void SetWriteCohortSpecifics( const unsigned& writeCohortSpecifics);
    void SetWriteCohortConsumption( const unsigned& writeCohortConsumption); 
    void SetWriteConsumptionSummary( const unsigned& writeConsumptionSummary); 

    void SetWriteStateInModelTime( const unsigned& writeStateInModelTime); 
    void SetWriteStateOverwrite( const unsigned& writeStateOverwrite); 
    void SetWriteStateInterval( const std::string& writeStateInterval); 
    void SetWriteStateIntervalUnit( const std::string& writeStateIntervalUnit); 
    //############################################
       
    // Calculated parameters
    unsigned GetNumberOfGridCells( ) const;
    unsigned GetLengthOfSimulationInMonths( ) const;
    unsigned GetLengthDataLongitudeArray( ) const;
    unsigned GetLengthDataLatitudeArray( ) const;
    unsigned GetDataIndexOfUserMinimumLongitude( ) const;
    unsigned GetDataIndexOfUserMaximumLongitude( ) const;
    unsigned GetDataIndexOfUserMinimumLatitude( ) const;
    unsigned GetDataIndexOfUserMaximumLatitude( ) const;
    unsigned GetLengthUserLongitudeArray( ) const;
    unsigned GetLengthUserLatitudeArray( ) const;

    unsigned GetSizeOfAnnualGridDatum( ) const;
    unsigned GetSizeOfMonthlyGridDatum( ) const;

    float GetDataLongitudeAtIndex( const unsigned& ) const;
    float GetDataLatitudeAtIndex( const unsigned& ) const;
    float GetUserLongitudeAtIndex( const unsigned& ) const;
    float GetUserLatitudeAtIndex( const unsigned& ) const;

    unsigned* GetMonthlyTimeStepArray( ) const;
    unsigned* GetAnnualTimeStepArray( ) const;
    //float* GetDataLongitudeArray( ) const;
    //float* GetDataLatitudeArray( ) const;
    float* GetUserLongitudeArray( ) const;
    float* GetUserLatitudeArray( ) const;
    
    int GetCellIndexFromDataIndices( const unsigned&, const unsigned& ) const;
    Types::DataCoordsPointer GetDataCoordsFromCellIndex( const unsigned& ) const;
    Types::DataIndicesPointer GetDataIndicesFromCellIndex( const unsigned& ) const;

    int GetBin( std::vector<double>, double );

private:
    Parameters( );
    void CalculateParameters( );

    static Types::ParametersPointer mThis;

    // User defined parameters
    std::string mRootDataDirectory;
    std::string mTimeStepUnits;
    unsigned mLengthOfSimulationInYears;
    unsigned mRestartedFromTimeStep;

    int mUserMinimumLongitude;
    int mUserMaximumLongitude;
    int mUserMinimumLatitude;
    int mUserMaximumLatitude;
    float mGridCellSize;
    float mExtinctionThreshold;
    unsigned mMaximumNumberOfCohorts;
    float mPlanktonSizeThreshold;
    bool mDrawRandomly;

    std::string mHumanNPPScenarioType;
    double mHumanNPPExtractionScale;
    double mHumanNPPScenarioDuration;
    unsigned mBurninSteps;
    unsigned mImpactSteps;
    unsigned mRecoverySteps;
    
    //############################################
    unsigned mRunParallel;
    unsigned mThreadNumber;
    std::string mSpatialSelection;

    //# require model ini preferences
    unsigned mApplyModelSpinup;
    std::string mCohortCSVLocation;
    std::string mStockCSVLocation;
    std::string mPoolCSVLocation;

    //############################################
    unsigned mTimeStepStartExtinction;
    unsigned mStartBodyMass;
    unsigned mEndBodyMass;
    unsigned mStepBodyMass;
    unsigned mSelectCarnivores;
    unsigned mSelectOmnivores;
    unsigned mSelectHerbivores;
    //############################################

    //# csv outputs
    unsigned mWriteCohortSpecifics;
    unsigned mWriteCohortConsumption;
    unsigned mWriteConsumptionSummary;

    unsigned mWriteStateInModelTime;
    unsigned mWriteStateOverwrite;
    double mWriteStateInterval;
    std::string mWriteStateIntervalUnit;
    //############################################
    
    // Calculated parameters
    unsigned mLengthOfSimulationInMonths;
    unsigned mNumberOfGridCells;
    unsigned mLengthDataLongitudeArray;
    unsigned mLengthDataLatitudeArray;
    unsigned mLengthUserLongitudeArray;
    unsigned mLengthUserLatitudeArray;
    unsigned mDataIndexOfUserMinimumLongitude;
    unsigned mDataIndexOfUserMaximumLongitude;
    unsigned mDataIndexOfUserMinimumLatitude;
    unsigned mDataIndexOfUserMaximumLatitude;
    unsigned mSizeOfMonthlyGridDatum;
    unsigned mSizeOfAnnualGridDatum;
    unsigned* mMonthlyTimeStepArray;
    unsigned* mAnnualTimeStepArray;
    float* mDataLongitudeArray;
    float* mDataLatitudeArray;
    float* mUserLongitudeArray;
    float* mUserLatitudeArray;

    Types::CoordsIndicesVector mCoordsIndicesLookup;
};

#endif

