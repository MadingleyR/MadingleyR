#include "DataLayerProcessor.h"
#include "DataLayer2D.h"
#include "DataLayer2DwithTime.h"
#include "DataLayer3D.h"
#include "DataLayer3DwithTime.h"
#include "InputData.h"
#include "InputDatum.h"
#include "Convertor.h"
#include "Processor.h"
#include "Maths.h"
#include "Parameters.h"

DataLayerProcessor::DataLayerProcessor( ) {

}

DataLayerProcessor::~DataLayerProcessor( ) {

}

Types::DataLayerMap DataLayerProcessor::ConvertReadDataIntoLayers( const Types::InputDataPointer inputData ) const {

    Types::DataLayerMap dataLayerMap;

    for( unsigned index = 0; index < inputData->GetNumberOfInputDatums( ); ++index ) {
        Types::InputDatumPointer inputDatum = inputData->GetInputDatum( index );

        std::cout << "Processing read data for datum \"" << inputDatum->GetName( ) << "\"..." << std::endl;

        Types::DataLayerPointer dataLayer = MakeDataLayer( inputDatum );
        dataLayerMap.insert( std::pair< std::string, Types::DataLayerPointer >( inputDatum->GetName( ), dataLayer ) );
    }

    return dataLayerMap;
}

Types::DataLayerPointer DataLayerProcessor::MakeDataLayer( const Types::InputDatumPointer datum ) const {

    Types::DataLayerPointer concreteDataLayer = NULL;

    Types::VariablePointer longitudeVariable = NULL;
    Types::VariablePointer latitudeVariable = NULL;
    Types::VariablePointer depthVariable = NULL;
    Types::VariablePointer timeVariable = NULL;
    Types::VariableVector variableVector;

    for( unsigned variableIndex = 0; variableIndex < datum->GetNumberOfVariables( ); ++variableIndex ) {

        Types::VariablePointer variable = datum->GetVariable( variableIndex );

        unsigned variableType = GetVariableType( variable );

        switch( variableType ) {
            case Constants::eLongitude:
                longitudeVariable = variable;
                longitudeVariable->SetMaximumDataDifference( FindLargestVectorValueDifference( longitudeVariable ) );
                break;

            case Constants::eLatitude:
                latitudeVariable = variable;
                latitudeVariable->SetMaximumDataDifference( FindLargestVectorValueDifference( latitudeVariable ) );
                break;

            case Constants::eDepth:
                depthVariable = variable;
                depthVariable->SetMaximumDataDifference( FindLargestVectorValueDifference( depthVariable ) );
                break;

            case Constants::eTime:
                timeVariable = variable;
                timeVariable->SetMaximumDataDifference( FindLargestVectorValueDifference( timeVariable ) );
                break;

            default:
                if( variable->IsDefault( ) == true ) {
                    variableVector.insert( variableVector.begin( ), variable );
                } else {
                    variableVector.push_back( variable );
                }
                break;
        }
    }

    if( variableVector.size( ) > 0 ) {
        if( longitudeVariable != NULL || latitudeVariable != NULL ) {

            if( depthVariable != NULL && timeVariable != NULL ) {
                // Spatial data with depth and time
                for( unsigned variableIndex = 0; variableIndex < variableVector.size( ); ++variableIndex ) {
                    unsigned userIndex = 0;
                    float* truncatedData = new float[ Parameters::Get( )->GetLengthUserLongitudeArray( ) * Parameters::Get( )->GetLengthUserLatitudeArray( ) * depthVariable->GetSize( ) * timeVariable->GetSize( ) ];

                    for( unsigned timeIndex = 0; timeIndex < timeVariable->GetSize( ); ++timeIndex ) {
                        for( unsigned depthIndex = 0; depthIndex < depthVariable->GetSize( ); ++depthIndex ) {
                            for( unsigned latitudeIndex = 0; latitudeIndex < Parameters::Get( )->GetLengthDataLatitudeArray( ); ++latitudeIndex ) {
                                for( unsigned longitudeIndex = 0; longitudeIndex < Parameters::Get( )->GetLengthDataLongitudeArray( ); ++longitudeIndex ) {

                                    unsigned dataIndex = Processor::Get( )->Indices4DToIndex( longitudeIndex, latitudeIndex, depthIndex, timeIndex, Parameters::Get( )->GetLengthDataLongitudeArray( ), Parameters::Get( )->GetLengthDataLatitudeArray( ), depthVariable->GetSize( ) );
                                    float data = variableVector[ 0 ]->GetDataAtIndex( dataIndex );

                                    float dataLatitude = Parameters::Get( )->GetDataLatitudeAtIndex( latitudeIndex );
                                    float dataLongitude = Parameters::Get( )->GetDataLongitudeAtIndex( longitudeIndex );

                                    if( dataLongitude >= Parameters::Get( )->GetUserLongitudeAtIndex( 0 ) && dataLongitude <= Parameters::Get( )->GetUserLongitudeAtIndex( Parameters::Get( )->GetLengthUserLongitudeArray( ) - 1 ) && dataLatitude >= Parameters::Get( )->GetUserLatitudeAtIndex( 0 ) && dataLatitude <= Parameters::Get( )->GetUserLatitudeAtIndex( Parameters::Get( )->GetLengthUserLatitudeArray( ) - 1 ) ) {
                                        truncatedData[ userIndex ] = data;
                                        userIndex += 1;
                                    }
                                }
                            }
                        }
                    }

                    variableVector[ variableIndex ]->SetData( truncatedData, userIndex + 1 );
                }
                concreteDataLayer = new DataLayer3DwithTime( datum->GetName( ), variableVector, longitudeVariable, latitudeVariable, depthVariable, timeVariable );

            } else if( depthVariable != NULL ) {
                // Spatial data with depth
                for( unsigned variableIndex = 0; variableIndex < variableVector.size( ); ++variableIndex ) {
                    unsigned userIndex = 0;
                    float* truncatedData = new float[ Parameters::Get( )->GetLengthUserLongitudeArray( ) * Parameters::Get( )->GetLengthUserLatitudeArray( ) * depthVariable->GetSize( ) ];

                    for( unsigned depthIndex = 0; depthIndex < depthVariable->GetSize( ); ++depthIndex ) {
                        for( unsigned latitudeIndex = 0; latitudeIndex < Parameters::Get( )->GetLengthDataLatitudeArray( ); ++latitudeIndex ) {
                            for( unsigned longitudeIndex = 0; longitudeIndex < Parameters::Get( )->GetLengthDataLongitudeArray( ); ++longitudeIndex ) {

                                unsigned dataIndex = Processor::Get( )->Indices3DToIndex( longitudeIndex, latitudeIndex, depthIndex, Parameters::Get( )->GetLengthDataLongitudeArray( ), Parameters::Get( )->GetLengthDataLatitudeArray( ) );
                                float data = variableVector[ 0 ]->GetDataAtIndex( dataIndex );

                                float dataLatitude = Parameters::Get( )->GetDataLatitudeAtIndex( latitudeIndex );
                                float dataLongitude = Parameters::Get( )->GetDataLongitudeAtIndex( longitudeIndex );

                                if( dataLongitude >= Parameters::Get( )->GetUserLongitudeAtIndex( 0 ) && dataLongitude <= Parameters::Get( )->GetUserLongitudeAtIndex( Parameters::Get( )->GetLengthUserLongitudeArray( ) - 1 ) && dataLatitude >= Parameters::Get( )->GetUserLatitudeAtIndex( 0 ) && dataLatitude <= Parameters::Get( )->GetUserLatitudeAtIndex( Parameters::Get( )->GetLengthUserLatitudeArray( ) - 1 ) ) {
                                    truncatedData[ userIndex ] = data;
                                    userIndex += 1;
                                }
                            }
                        }
                    }

                    variableVector[ variableIndex ]->SetData( truncatedData, userIndex + 1 );
                }
                concreteDataLayer = new DataLayer3D( datum->GetName( ), variableVector, longitudeVariable, latitudeVariable, depthVariable );

            } else if( timeVariable != NULL ) {
                // Spatial data with time
                for( unsigned variableIndex = 0; variableIndex < variableVector.size( ); ++variableIndex ) {
                    unsigned userIndex = 0;
                    float* truncatedData = new float[ Parameters::Get( )->GetLengthUserLongitudeArray( ) * Parameters::Get( )->GetLengthUserLatitudeArray( ) * timeVariable->GetSize( ) ];

                    for( unsigned timeIndex = 0; timeIndex < timeVariable->GetSize( ); ++timeIndex ) {
                        for( unsigned latitudeIndex = 0; latitudeIndex < Parameters::Get( )->GetLengthDataLatitudeArray( ); ++latitudeIndex ) {
                            for( unsigned longitudeIndex = 0; longitudeIndex < Parameters::Get( )->GetLengthDataLongitudeArray( ); ++longitudeIndex ) {

                                unsigned dataIndex = Processor::Get( )->Indices3DToIndex( longitudeIndex, latitudeIndex, timeIndex, Parameters::Get( )->GetLengthDataLongitudeArray( ), Parameters::Get( )->GetLengthDataLatitudeArray( ) );
                                float data = variableVector[ 0 ]->GetDataAtIndex( dataIndex );

                                float dataLatitude = Parameters::Get( )->GetDataLatitudeAtIndex( latitudeIndex );
                                float dataLongitude = Parameters::Get( )->GetDataLongitudeAtIndex( longitudeIndex );

                                if( dataLongitude >= Parameters::Get( )->GetUserLongitudeAtIndex( 0 ) && dataLongitude <= Parameters::Get( )->GetUserLongitudeAtIndex( Parameters::Get( )->GetLengthUserLongitudeArray( ) - 1 ) && dataLatitude >= Parameters::Get( )->GetUserLatitudeAtIndex( 0 ) && dataLatitude <= Parameters::Get( )->GetUserLatitudeAtIndex( Parameters::Get( )->GetLengthUserLatitudeArray( ) - 1 ) ) {
                                    truncatedData[ userIndex ] = data;
                                    userIndex += 1;
                                }
                            }
                        }
                    }

                    variableVector[ variableIndex ]->SetData( truncatedData, userIndex + 1 );
                }
                concreteDataLayer = new DataLayer2DwithTime( datum->GetName( ), variableVector, longitudeVariable, latitudeVariable, timeVariable );

            } else {
                // Spatial data
                for( unsigned variableIndex = 0; variableIndex < variableVector.size( ); ++variableIndex ) {
                    unsigned userIndex = 0;
                    float* truncatedData = new float[ Parameters::Get( )->GetLengthUserLongitudeArray( ) * Parameters::Get( )->GetLengthUserLatitudeArray( ) ];

                    for( unsigned latitudeIndex = 0; latitudeIndex < Parameters::Get( )->GetLengthDataLatitudeArray( ); ++latitudeIndex ) {
                        for( unsigned longitudeIndex = 0; longitudeIndex < Parameters::Get( )->GetLengthDataLongitudeArray( ); ++longitudeIndex ) {

                            unsigned dataIndex = Processor::Get( )->Indices2DToIndex( longitudeIndex, latitudeIndex, Parameters::Get( )->GetLengthDataLongitudeArray( ) );
                            float data = variableVector[ 0 ]->GetDataAtIndex( dataIndex );

                            float dataLatitude = Parameters::Get( )->GetDataLatitudeAtIndex( latitudeIndex );
                            float dataLongitude = Parameters::Get( )->GetDataLongitudeAtIndex( longitudeIndex );

                            if( dataLongitude >= Parameters::Get( )->GetUserLongitudeAtIndex( 0 ) && dataLongitude <= Parameters::Get( )->GetUserLongitudeAtIndex( Parameters::Get( )->GetLengthUserLongitudeArray( ) - 1 ) && dataLatitude >= Parameters::Get( )->GetUserLatitudeAtIndex( 0 ) && dataLatitude <= Parameters::Get( )->GetUserLatitudeAtIndex( Parameters::Get( )->GetLengthUserLatitudeArray( ) - 1 ) ) {
                                truncatedData[ userIndex ] = data;
                                userIndex += 1;
                            }
                        }
                    }

                    variableVector[ variableIndex ]->SetData( truncatedData, userIndex + 1 );
                }
                concreteDataLayer = new DataLayer2D( datum->GetName( ), variableVector, longitudeVariable, latitudeVariable );
            }

            longitudeVariable->SetData( Parameters::Get( )->GetUserLongitudeArray( ), Parameters::Get( )->GetLengthUserLongitudeArray( ) );
            latitudeVariable->SetData( Parameters::Get( )->GetUserLatitudeArray( ), Parameters::Get( )->GetLengthUserLatitudeArray( ) );

        } else {
            if( longitudeVariable == NULL && latitudeVariable == NULL ) {
                std::cout << "ERROR> Attempted creation of datum \"" << datum->GetName( ) << "\" failed with no longitude and latitude set." << std::endl;
            } else if( longitudeVariable == NULL ) {
                std::cout << "ERROR> Attempted creation of datum \"" << datum->GetName( ) << "\" failed with no longitude set." << std::endl;
            } else {
                std::cout << "ERROR> Attempted creation of datum \"" << datum->GetName( ) << "\" failed with no latitude set." << std::endl;
            }
        }
    } else {
        std::cout << "ERROR> Attempted creation of datum \"" << datum->GetName( ) << "\" failed with no variables set." << std::endl;
    }

    return concreteDataLayer;
}

unsigned DataLayerProcessor::GetVariableType( const Types::VariablePointer variable ) const {

    std::string variableName = Convertor::Get( )->ToLowercase( variable->GetName( ) );

    unsigned variableType = Constants::eOther;
    bool isFound = false;

    for( unsigned longitudeNameIndex = 0; longitudeNameIndex < Processor::Get( )->SizeOfArray( Constants::cLongitudeVariableNames ); ++longitudeNameIndex ) {
        if( variableName == Constants::cLongitudeVariableNames[ longitudeNameIndex ] ) {
            variableType = Constants::eLongitude;
            isFound = true;
            break;
        }
    }

    if( isFound == false ) {
        for( unsigned latitudeNameIndex = 0; latitudeNameIndex < Processor::Get( )->SizeOfArray( Constants::cLatitudeVariableNames ); ++latitudeNameIndex ) {
            if( variableName == Constants::cLatitudeVariableNames[ latitudeNameIndex ] ) {
                variableType = Constants::eLatitude;
                isFound = true;
                break;
            }
        }
    }

    if( isFound == false ) {
        for( unsigned timeNameIndex = 0; timeNameIndex < Processor::Get( )->SizeOfArray( Constants::cTimeVariableNames ); ++timeNameIndex ) {
            if( variableName == Constants::cTimeVariableNames[ timeNameIndex ] ) {
                variableType = Constants::eTime;
                isFound = true;
                break;
            }
        }
    }

    if( isFound == false ) {
        for( unsigned depthNameIndex = 0; depthNameIndex < Processor::Get( )->SizeOfArray( Constants::cDepthVariableNames ); ++depthNameIndex ) {
            if( variableName == Constants::cDepthVariableNames[ depthNameIndex ] ) {
                variableType = Constants::eDepth;
                isFound = true;
                break;
            }
        }
    }

    return variableType;
}

float DataLayerProcessor::FindLargestVectorValueDifference( const Types::VariablePointer variable ) const {

    float largestDifference = 0;

    for( unsigned variableIndex = 0; variableIndex < variable->GetSize( ) - 1; ++variableIndex ) {
        float difference = Maths::Get( )->Abs( variable->GetDataAtIndex( variableIndex + 1 ) - variable->GetDataAtIndex( variableIndex ) );

        if( difference > largestDifference ) {
            largestDifference = difference;
        }
    }

    return largestDifference;
}

bool DataLayerProcessor::DoesTheGeographicVariableNeedAdjustment( const Types::VariablePointer geographicVariable ) const {

    bool needsAdjustment = false;

    float dataAtBeginning = geographicVariable->GetDataAtIndex( 0 );
    float dataAtEnd = geographicVariable->GetDataAtIndex( geographicVariable->GetSize( ) - 1 );
    float dataToCheck = Constants::cMissingValue;

    if( dataAtBeginning < dataAtEnd ) {
        dataToCheck = dataAtBeginning;
    } else {
        dataToCheck = dataAtEnd;
    }

    if( dataToCheck >= 0 ) {
        needsAdjustment = true;
    }

    return needsAdjustment;
}