#ifndef CONSTANTS
#define CONSTANTS

#include <string>

namespace Constants {
    
    enum eParametersMetadata {
        eParameterName,
        eParameterValue
    };

    enum eOutputControlParametersMetadata {
        eDatumName,
        eDatumType,
        eTimeUnit,
        eDataUnit
    };

    enum eEnvironmentalDataLayersMetadata {
        eInternalName,
        eFilePath,
        eDefaultVariableName
    };

    enum eDataLayerTypes {
        eDataLayer2D, // Spatial: land-sea mask
        eDataLayer2DwithTime, // Two-dimensional with time, e.g. SST.
        eDataLayer3D, // Three-dimensional, e.g. ocean temperature.
        eDataLayer3DwithTime // Three-dimensional with time.
    };

    enum eVariableTypes {
        eLongitude,
        eLatitude,
        eTime,
        eDepth,
        eOther
    };

    const std::string cLongitudeVariableNames[ ] = { "lon", "long", "longitude", "x" };
    const std::string cLatitudeVariableNames[ ] = { "lat", "lats", "latitude", "y" };
    const std::string cDepthVariableNames[ ] = { "dep", "depth", "z" };
    const std::string cTimeVariableNames[ ] = { "time", "month", "t" };

    const std::string cBasicDatumTypeName = "basic";
    const std::string cGridDatumTypeName = "grid";

    //const std::string cConfigurationDirectory = "./input/Model setup/";
    const std::string cInputParametersFileName = "SimulationControlParameters.csv";
    const std::string cInputDataFileName = "EnvironmentalDataLayers.csv";
    const std::string cOutputVariablesFileName = "OutputControlParameters.csv";

    const std::string cCohortDefinitionsFileName = "CohortFunctionalGroupDefinitions.csv";
    const std::string cStockDefinitionsFileName = "StockFunctionalGroupDefinitions.csv";
    const std::string cMassBinDefinitionsFileName = "MassBinDefinitions.csv";

    const std::string cOutputBaseDirectory = "./output/";
    const std::string cDataSetNameFormat = "%Y-%m-%d_%H-%M-%S";
    const std::string cCompleteDateFormat = "%c";
    const std::string cUnitsString = "units";
    const std::string cTimeVariableUnits = "months";

    ///////////////////////////////////////////////////////////////////////////
    const std::string cAnnualBasicOutputsFileName = "AnnualBasicOutputs.nc";
    const std::string cAnnualBasicOutputsFileName_State = "AnnualBasicOutputs_State.nc";
    const std::string cMonthlyBasicOutputsFileName = "MonthlyBasicOutputs.nc";
    const std::string cMonthlyBasicOutputsFileName_State = "MonthlyBasicOutputs_State.nc";
    const std::string cAnnualGridOutputsFileName = "AnnualGridOutputs.nc";
    const std::string cAnnualGridOutputsFileName_State = "AnnualGridOutputs.nc";
    const std::string cMonthlyGridOutputsFileName = "MonthlyGridOutputs.nc";
    const std::string cMonthlyGridOutputsFileName_State = "MonthlyGridOutputs_State.nc";
    const std::string cMonthlyTimeUnitName = "month";
    const std::string cAnnualTimeUnitName = "year";
    const std::string cLongitudeVariableUnit = "degrees east";
    const std::string cLatitudeVariableUnit = "degrees north";
    ///////////////////////////////////////////////////////////////////////////

    const int cMissingValue = -9999;
    const int cOutputFolderPermissions = 0775;
    const int cDateTimeBufferSize = 25;

    const char cDataDelimiterValue = ',';
    const char cTextFileCommentCharacter = '#';
    const char cFolderDelimiter = '/';
    const char cWhiteSpaceCharacter = ' ';
}

#endif
