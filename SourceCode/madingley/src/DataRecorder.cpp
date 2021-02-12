#include "DataRecorder.h"
#include "Constants.h"
#include "BasicDatum.h"
#include "GridDatum.h"
#include "Convertor.h"
#include "Parameters.h"

Types::DataRecorderPointer DataRecorder::mThis = NULL;

Types::DataRecorderPointer DataRecorder::Get( ) {
    if( mThis == NULL ) {
        mThis = new DataRecorder( );
    }
    return mThis;
}

DataRecorder::~DataRecorder( ) {
    for( Types::BasicDatumMap::iterator iter = mBasicDatumMap.begin( ); iter != mBasicDatumMap.end( ); ++iter ) {
        delete iter->second;
    }
    for( Types::GridDatumMap::iterator iter = mGridDatumMap.begin( ); iter != mGridDatumMap.end( ); ++iter ) {
        delete iter->second;
    }
    if( mThis != NULL ) {
        delete mThis;
    }
}

DataRecorder::DataRecorder( ) {

}

bool DataRecorder::Initialise( const Types::StringMatrix& rawOutputParameterData ) {
    bool success = false;
    if( rawOutputParameterData.size( ) > 0 ) {
        if( rawOutputParameterData[ 0 ].size( ) == Constants::eDataUnit + 1 ) {
            for( unsigned rowIndex = 0; rowIndex < rawOutputParameterData.size( ); ++rowIndex ) {
                std::string name = rawOutputParameterData[ rowIndex ][ Constants::eDatumName ];
                std::string type = Convertor::Get( )->ToLowercase( rawOutputParameterData[ rowIndex ][ Constants::eDatumType ] );
                std::string timeUnit = Convertor::Get( )->ToLowercase( rawOutputParameterData[ rowIndex ][ Constants::eTimeUnit ] );
                std::string dataUnit = Convertor::Get( )->ToLowercase( rawOutputParameterData[ rowIndex ][ Constants::eDataUnit ] );

                Types::StringVector datumMetadata;

                datumMetadata.push_back( name );
                datumMetadata.push_back( type );
                datumMetadata.push_back( timeUnit );
                datumMetadata.push_back( dataUnit );

                if( type == Constants::cBasicDatumTypeName ) {
                    mBasicOutputMetadata.push_back( datumMetadata );
                } else if( type == Constants::cGridDatumTypeName ) {
                    mGridOutputMetadata.push_back( datumMetadata );

                }
            }
            success = true;
        }
    }
    return success;
}

void DataRecorder::SetDataOn( const std::string& name, const float& data ) {

    Types::BasicDatumPointer basicDatum = GetBasicDatum( name );

    if( basicDatum != NULL ) {
        basicDatum->SetData( data );
    }
}

void DataRecorder::AddDataTo( const std::string& name, const float& data ) {

    Types::BasicDatumPointer basicDatum = GetBasicDatum( name );

    if( basicDatum != NULL ) {
        basicDatum->AddData( data );
    }
}

void DataRecorder::SetDataOn( const std::string& name, const unsigned& cellIndex, const float& data ) {

    Types::GridDatumPointer gridDatum = GetGridDatum( name );

    if( gridDatum != NULL )
        gridDatum->SetData( cellIndex, data );
}

Types::BasicDatumMap DataRecorder::GetBasicDatumMap( ) const {
    return mBasicDatumMap;
}

Types::GridDatumMap DataRecorder::GetGridDatumMap( ) const {
    return mGridDatumMap;
}

Types::StringVector DataRecorder::GetInputFilePaths( ) const {
    return mInputFilePaths;
}

void DataRecorder::AddInputFilePath( const std::string& inputFilePath ) {
    mInputFilePaths.push_back( inputFilePath );
}

Types::BasicDatumPointer DataRecorder::GetBasicDatum( const std::string& name ) {

    Types::BasicDatumPointer basicDatum = NULL;

    Types::BasicDatumMap::iterator iter = mBasicDatumMap.find( name );

    if( iter != mBasicDatumMap.end( ) ) {
        basicDatum = iter->second;
    } else {
        for( unsigned datumIndex = 0; datumIndex < mBasicOutputMetadata.size( ); ++datumIndex ) {

            std::string basicDatumName = mBasicOutputMetadata[ datumIndex ][ Constants::eDatumName ];

            if( Convertor::Get( )->ToLowercase( basicDatumName ) == Convertor::Get( )->ToLowercase( name ) ) {

                basicDatum = new BasicDatum( basicDatumName, mBasicOutputMetadata[ datumIndex ][ Constants::eTimeUnit ], mBasicOutputMetadata[ datumIndex ][ Constants::eDataUnit ] );
                mBasicDatumMap.insert( std::pair< std::string, Types::BasicDatumPointer >( basicDatumName, basicDatum ) );
                break;
            }
        }
    }
    return basicDatum;
}

Types::GridDatumPointer DataRecorder::GetGridDatum( const std::string& name ) {

    Types::GridDatumPointer gridDatum = NULL;

    Types::GridDatumMap::iterator iter = mGridDatumMap.find( name );

    if( iter != mGridDatumMap.end( ) ) {
        gridDatum = iter->second;
    } else {
        for( unsigned datumIndex = 0; datumIndex < mGridOutputMetadata.size( ); ++datumIndex ) {

            std::string gridDatumName = mGridOutputMetadata[ datumIndex ][ Constants::eDatumName ];

            if( Convertor::Get( )->ToLowercase( gridDatumName ) == Convertor::Get( )->ToLowercase( name ) ) {

                gridDatum = new GridDatum( gridDatumName, mGridOutputMetadata[ datumIndex ][ Constants::eTimeUnit ], mGridOutputMetadata[ datumIndex ][ Constants::eDataUnit ] );
                mGridDatumMap.insert( std::pair< std::string, Types::GridDatumPointer >( gridDatumName, gridDatum ) );
                break;
            }
        }
    }
    return gridDatum;
}