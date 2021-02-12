#include "TimeStep.h"
#include "Maths.h"

Types::TimeStepPointer TimeStep::mThis = NULL;

TimeStep::TimeStep( ) {
    mMonthlyTimeStep = 0;
    mAnnualTimeStep = 0;
}

TimeStep::~TimeStep( ) {

}

Types::TimeStepPointer TimeStep::Get( ) {
    if( mThis == NULL ) {
        mThis = new TimeStep( );
    }
    return mThis;
}

unsigned TimeStep::Get( const std::string& timeUnit ) {
    if( timeUnit == Constants::cMonthlyTimeUnitName )
        return mMonthlyTimeStep;
    else
        return mAnnualTimeStep;
}

void TimeStep::SetMonthly( const unsigned& monthlyTimeStep ) {
    mMonthlyTimeStep = monthlyTimeStep;

    if( monthlyTimeStep != 0 && Maths::Get( )->Mod( mMonthlyTimeStep, 12 ) == 0 )
        mAnnualTimeStep += 1;
}

void TimeStep::SetAnnual( const unsigned& annualTimeStep ) {
    mAnnualTimeStep = annualTimeStep;
    mMonthlyTimeStep = mAnnualTimeStep * 12;
}
