#include "DataLayer2DwithTime.h"

#include "Parameters.h"
#include "TimeStep.h"
#include "Maths.h"

DataLayer2DwithTime::DataLayer2DwithTime( const std::string& name, const Types::VariableVector variableVector, Types::VariablePointer longitudeVariable, Types::VariablePointer latitudeVariable, Types::VariablePointer timeVariable ) {
    mName = name;
    mVariableVector = variableVector;
    mLongitudeVariable = longitudeVariable;
    mLatitudeVariable = latitudeVariable;
    mTimeVariable = timeVariable;
}

DataLayer2DwithTime::~DataLayer2DwithTime( ) {
    delete mTimeVariable;
}

float DataLayer2DwithTime::GetDataAtCellIndex( const unsigned cellIndex ) const {
    // FIX: The way the month index is calculated here is due to the use of an annual climatology
    unsigned monthIndex = Maths::Get( )->Mod( TimeStep::Get( )->Get( Constants::cMonthlyTimeUnitName ), 12 );
    unsigned dataIndex = cellIndex + ( monthIndex * Parameters::Get( )->GetNumberOfGridCells( ) );

    return mVariableVector[ 0 ]->GetDataAtIndex( dataIndex );
}

Types::VariablePointer DataLayer2DwithTime::GetTimeVariable( ) const {
    return mTimeVariable;
}