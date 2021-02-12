#include "GridDatum.h"
#include "Constants.h"
#include "Types.h"
#include "Parameters.h"
#include "TimeStep.h"

GridDatum::GridDatum( const std::string& name, const std::string& timeUnit, const std::string& dataUnit ) {
    mName = name;
    mTimeUnit = timeUnit;
    mDataUnit = dataUnit;
    
    unsigned size = Parameters::Get( )->GetSizeOfMonthlyGridDatum( );

    if( mTimeUnit == Constants::cAnnualTimeUnitName )
        size = Parameters::Get( )->GetSizeOfAnnualGridDatum( );

    mData = new float[ size ]( );
}

GridDatum::~GridDatum( ) {
    delete[ ] mData;
}

std::string GridDatum::GetName( ) const {
    return mName;
}

std::string GridDatum::GetTimeUnit( ) const {
    return mTimeUnit;
}

std::string GridDatum::GetDataUnit( ) const {
    return mDataUnit;
}

float* GridDatum::GetData( ) const {
    return mData;
}

void GridDatum::SetData( const unsigned& cellIndex, const float& data ) {

    unsigned dataIndex = cellIndex + ( TimeStep::Get( )->Get( mTimeUnit ) * Parameters::Get( )->GetNumberOfGridCells( ) );
    
    mData[ dataIndex ] = data;
}