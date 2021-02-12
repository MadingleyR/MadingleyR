#include "DataLayer.h"

#include "Convertor.h"

std::string DataLayer::GetName( ) {
    return mName;
}

Types::VariablePointer DataLayer::GetDefaultVariable( ) const {
    return mVariableVector[ 0 ];
}

Types::VariablePointer DataLayer::GetVariable( const std::string& name ) const {

    Types::VariablePointer variablePointer = NULL;

    for( unsigned index = 0; index < mVariableVector.size( ); ++index ) {
        if( Convertor::Get( )->ToLowercase( mVariableVector[ index ]->GetName( ) ) == Convertor::Get( )->ToLowercase( name ) ) {
            variablePointer = mVariableVector[ index ];
            break;
        }
    }

    if( variablePointer == NULL ) {
        std::cout << "ERROR> Variable with name \"" << name << "\" is not found." << std::endl;
    }

    return variablePointer;
}

Types::VariablePointer DataLayer::GetLongitudeVariable( ) const {
    return mLongitudeVariable;
}

Types::VariablePointer DataLayer::GetLatitudeVariable( ) const {
    return mLatitudeVariable;
}