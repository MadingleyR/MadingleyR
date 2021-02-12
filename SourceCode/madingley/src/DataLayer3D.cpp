#include "DataLayer3D.h"

DataLayer3D::DataLayer3D( const std::string& name, const Types::VariableVector variableVector, const Types::VariablePointer longitudeVariable, const Types::VariablePointer latitudeVariable, const Types::VariablePointer depthVariable ) {
    mName = name;
    mVariableVector = variableVector;
    mLongitudeVariable = longitudeVariable;
    mLatitudeVariable = latitudeVariable;
    mDepthVariable = depthVariable;
}

DataLayer3D::~DataLayer3D( ) {
    delete mDepthVariable;
}

float DataLayer3D::GetDataAtCellIndex( const unsigned cellIndex ) const {
    // FIX
    return 1.0;
}

Types::VariablePointer DataLayer3D::GetDepthVariable( ) const {
    return mDepthVariable;
}