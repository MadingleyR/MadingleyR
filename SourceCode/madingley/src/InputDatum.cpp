#include "InputDatum.h"
#include "Variable.h"

InputDatum::InputDatum( const std::string& name, const std::string& variableName, const Types::UnsignedVector& variableDimensions, const unsigned& size, float* variableData, const bool isDefault ) {
    mName = name;
    mVariableVector.push_back( new Variable( variableName, variableDimensions, size, variableData, isDefault ) );
}

InputDatum::~InputDatum( ) {
    for( unsigned index = 0; index < mVariableVector.size( ); ++index ) {
        delete mVariableVector[ index ];
    }
}

std::string InputDatum::GetName( ) const {
    return mName;
}

Types::VariablePointer InputDatum::GetVariable( const unsigned& index ) const {
    return mVariableVector[ index ];
}

unsigned InputDatum::GetNumberOfVariables( ) const {
    return mVariableVector.size( );
}

void InputDatum::AddVariable( const std::string& name, const Types::UnsignedVector& dimensions, const unsigned& size, float* data, const bool isDefault ) {
    mVariableVector.push_back( new Variable( name, dimensions, size, data, isDefault ) );
}