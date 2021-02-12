#include "FileWriter.h"
#include "Constants.h"
#include "Date.h"
#include "Convertor.h"
#include "Parameters.h"
#include "DataRecorder.h"
#include "BasicDatum.h"
#include "GridDatum.h"
#include "InputParameters.h"

FileWriter::~FileWriter( ) {
}

FileWriter::FileWriter( ) {
    InitialiseOutputDirectory( );
    WriteInputFiles( );
}

bool FileWriter::WriteFiles( ) {
    bool completedSuccessfully = true;
    return completedSuccessfully;
}

std::string& FileWriter::GetOutputDirectory( ) {
    return mOutputDirectory;
}

#if __linux__
void FileWriter::InitialiseOutputDirectory( ) {
    // mOutputDirectory = Constants::cOutputBaseDirectory;
    // mkdir( mOutputDirectory.c_str( ), Constants::cOutputFolderPermissions );
    // mOutputDirectory.append( Date::GetDateAndTimeString( ) );
    // mOutputDirectory.append( "_" );
    // mOutputDirectory.append( Parameters::Get( )->GetSpatialSelection( ) );
    // if(OutputFolderName!="none") mOutputDirectory.append( OutputFolderName );
    // mkdir( mOutputDirectory.c_str( ), Constants::cOutputFolderPermissions );
    // mOutputDirectory.append( Convertor::Get( )->ToString( Constants::cFolderDelimiter ) );

    OutputFolderName = InputParameters::Get( )->GetOutputFolderName();

    mOutputDirectory = OutputFolderName;
    mkdir( mOutputDirectory.c_str( ), Constants::cOutputFolderPermissions );

}
#elif __APPLE__
void FileWriter::InitialiseOutputDirectory( ) {
    // mOutputDirectory = Constants::cOutputBaseDirectory;
    // mkdir( mOutputDirectory.c_str( ), Constants::cOutputFolderPermissions );
    // mOutputDirectory.append( Date::GetDateAndTimeString( ) );
    // mOutputDirectory.append( "_" );
    // mOutputDirectory.append( Parameters::Get( )->GetSpatialSelection( ) );
    // if(OutputFolderName!="none") mOutputDirectory.append( OutputFolderName );
    // mkdir( mOutputDirectory.c_str( ), Constants::cOutputFolderPermissions );
    // mOutputDirectory.append( Convertor::Get( )->ToString( Constants::cFolderDelimiter ) );

    OutputFolderName = InputParameters::Get( )->GetOutputFolderName();

    mOutputDirectory = OutputFolderName;
    mkdir( mOutputDirectory.c_str( ), Constants::cOutputFolderPermissions );

}
#elif _WIN32
void FileWriter::InitialiseOutputDirectory( ) {
    // folder created from R
}
#endif


void FileWriter::WriteInputFiles( ) {
    Types::StringVector inputFilePaths = DataRecorder::Get( )->GetInputFilePaths( );

    for( unsigned stringIndex = 0; stringIndex < inputFilePaths.size( ); ++stringIndex ) {

        std::ifstream sourceFileStream( inputFilePaths[ stringIndex ].c_str( ), std::ios::in );

        std::string outputFilePath = mOutputDirectory;
        Types::StringVector inputFilePathComponents = Convertor::Get( )->StringToWords( inputFilePaths[ stringIndex ], Constants::cFolderDelimiter );

        std::string fileName = inputFilePathComponents[ inputFilePathComponents.size( ) - 1 ];
        outputFilePath.append( fileName );

        std::ofstream destinationFileStream( outputFilePath.c_str( ), std::ios::out );

        destinationFileStream << sourceFileStream.rdbuf( );

        sourceFileStream.close( );
        destinationFileStream.close( );
    }
}
