#include "InputData.h"
#include "InputDatum.h"
#include "Variable.h"
#include "Constants.h"
#include "Convertor.h"

InputData::InputData( ) {

}

InputData::~InputData( ) {
    for( unsigned index = 0; index < mInputDatumVector.size( ); ++index ) {
        delete mInputDatumVector[ index ];
    }
}

void InputData::AddVariableToDatum( const std::string& name, const std::string& variableName, const Types::UnsignedVector& variableDimensions, 
                                    const unsigned& variableSize, float* variableData, const bool isDefault ) {

    Types::InputDatumPointer datum = GetInputDatum( name, true );

    if( datum == NULL ) {
        mInputDatumVector.push_back( new InputDatum( name, variableName, variableDimensions, variableSize, variableData, isDefault ) );
    } else {
        datum->AddVariable( variableName, variableDimensions, variableSize, variableData, isDefault );
    }

}

Types::InputDatumPointer InputData::GetInputDatum( const std::string& name, const bool isInternalSearch ) {

    Types::InputDatumPointer inputDatum = NULL;

    for( unsigned index = 0; index < mInputDatumVector.size( ); ++index ) {
        if( Convertor::Get( )->ToLowercase( mInputDatumVector[ index ]->GetName( ) ) == Convertor::Get( )->ToLowercase( name ) ) {
            inputDatum = mInputDatumVector[ index ];
            break;
        }
    }

    if( inputDatum == NULL && isInternalSearch == false ) {
        std::cout << "ERROR> Variable with name \"" << name << "\" is not found." << std::endl;
    }

    return inputDatum;
}

Types::InputDatumPointer InputData::GetInputDatum( const unsigned index ) {
    return mInputDatumVector[ index ];
}

unsigned InputData::GetNumberOfInputDatums( ) {
    return mInputDatumVector.size( );
}