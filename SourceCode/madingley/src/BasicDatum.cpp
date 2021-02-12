#include "BasicDatum.h"

#include "Constants.h"
#include "Parameters.h"
#include "TimeStep.h"

BasicDatum::BasicDatum( const std::string& name, const std::string& timeUnit, const std::string& dataUnit ) {
    mName = name;
    mTimeUnit = timeUnit;
    mDataUnit = dataUnit;

    unsigned size = Parameters::Get( )->GetLengthOfSimulationInMonths( );
    
    if( mTimeUnit == Constants::cAnnualTimeUnitName )
        size = Parameters::Get( )->GetLengthOfSimulationInYears( );

    mData = new float[ size ]( );
}

BasicDatum::~BasicDatum( ) {
    delete[ ] mData;
}

std::string BasicDatum::GetName( ) const {
    return mName;
}

std::string BasicDatum::GetTimeUnit( ) const {
    return mTimeUnit;
}

std::string BasicDatum::GetDataUnit( ) const {
    return mDataUnit;
}

float* BasicDatum::GetData( ) const {
    return mData;
}

void BasicDatum::SetData( const float& data ) {
    mData[ TimeStep::Get( )->Get( mTimeUnit ) ] = data;
}

void BasicDatum::AddData( const float& data ) {
    mData[ TimeStep::Get( )->Get( mTimeUnit ) ] += data;
}