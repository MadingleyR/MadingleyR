#include "DataLayer3DwithTime.h"

DataLayer3DwithTime::DataLayer3DwithTime( const std::string& name, const Types::VariableVector variableVector, const Types::VariablePointer longitudeVariable, const Types::VariablePointer latitudeVariable, const Types::VariablePointer depthVariable, const Types::VariablePointer timeVariable ) {
    mName = name;
    mVariableVector = variableVector;
    mLongitudeVariable = longitudeVariable;
    mLatitudeVariable = latitudeVariable;
    mDepthVariable = depthVariable;
    mTimeVariable = timeVariable;
}

DataLayer3DwithTime::~DataLayer3DwithTime( ) {
    delete mDepthVariable;
    delete mTimeVariable;
}

float DataLayer3DwithTime::GetDataAtCellIndex( const unsigned cellIndex ) const {
    // FIX
    return 1.0;
}

Types::VariablePointer DataLayer3DwithTime::GetDepthVariable( ) const {
    return mDepthVariable;
}

Types::VariablePointer DataLayer3DwithTime::GetTimeVariable( ) const {
    return mTimeVariable;
}