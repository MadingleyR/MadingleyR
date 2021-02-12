#include "DataLayer2D.h"

DataLayer2D::DataLayer2D( const std::string& name, const Types::VariableVector& variableVector, const Types::VariablePointer longitudeVariable, const Types::VariablePointer latitudeVariable ) {
    mName = name;
    mVariableVector = variableVector;
    mLongitudeVariable = longitudeVariable;
    mLatitudeVariable = latitudeVariable;
}

DataLayer2D::~DataLayer2D( ) {
}

float DataLayer2D::GetDataAtCellIndex( const unsigned cellIndex ) const {
    return mVariableVector[ 0 ]->GetDataAtIndex( cellIndex );
}