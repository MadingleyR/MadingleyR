#include "FileReader.h"
#include "Convertor.h"
#include "Constants.h"
#include "InputData.h"
#include "DataLayerSet.h"
#include "DataRecorder.h"
#include "Parameters.h"
#include "InputParameters.h"
#include "DataCoords.h"

FileReader::FileReader( ) {
}

FileReader::~FileReader( ) {
}

bool FileReader::ReadFiles( ) {

    bool success = false;
    success = ReadInputParameters( );
    //if( success == true ) success = SetUpOutputVariables( );
    return success;
}

bool FileReader::ReadTextFile( const std::string& filePath ) {

    bool success = false;

    ClearMetadata( );

    //std::cout << "Reading text file \"" << filePath << "\"..." << std::endl;
    std::ifstream fileStream( filePath.c_str( ), std::ios::in );

    if( fileStream.is_open( ) ) {
        std::string readLine;
        unsigned lineCount = 0;

        while( std::getline( fileStream, readLine ) ) {
            if( lineCount > 0 && readLine[ 0 ] != Constants::cTextFileCommentCharacter ) {
                mMetadata.push_back( Convertor::Get( )->StringToWords( readLine, Constants::cDataDelimiterValue ) );
            } else if( lineCount == 0 ) {
                mMetadataHeadings = Convertor::Get( )->StringToWords( readLine, Constants::cDataDelimiterValue );
            }
            ++lineCount;
        }
        success = true;
        fileStream.close( );
        DataRecorder::Get( )->AddInputFilePath( filePath );
        
        std::cout << "Yeah!! reading: " << filePath << std::endl;
    } else {
        std::cout << "File path \"" << filePath << "\" is invalid." << std::endl;

    }

    return success;
}

bool FileReader::ReadInputParameters( ) {

    //std::cout << "Going to read:  " << InputParameters::Get( )->GetConfigurationDirectory( ) + Constants::cInputParametersFileName << std::endl;

    bool success = true; //ReadTextFile( InputParameters::Get( )->GetConfigurationDirectory( ) + Constants::cInputParametersFileName );
    if( success == true ) success = Parameters::Get( )->Initialise( mMetadata );
    return success;
}

bool FileReader::SetUpOutputVariables( ) {

    bool success = ReadTextFile( InputParameters::Get( )->GetConfigurationDirectory( ) + Constants::cOutputVariablesFileName );
    if( success == true ) success = DataRecorder::Get( )->Initialise( mMetadata );
    return success;
}

void FileReader::ClearMetadata( ) {
    
    for( unsigned rowIndex = 0; rowIndex < mMetadata.size( ); ++rowIndex ) mMetadata[ rowIndex ].clear( );
    mMetadata.clear( );
    mMetadataHeadings.clear( );
}
