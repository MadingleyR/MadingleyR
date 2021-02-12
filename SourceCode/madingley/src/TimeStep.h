#ifndef TIMESTEP
#define	TIMESTEP

#include "Types.h"

class TimeStep {
public:
    ~TimeStep( );
    static Types::TimeStepPointer Get( );
    
    unsigned Get( const std::string& );
    void SetMonthly( const unsigned& );
    void SetAnnual( const unsigned& );
    
private:
    TimeStep( );
    
    static Types::TimeStepPointer mThis;
    
    unsigned mMonthlyTimeStep;
    unsigned mAnnualTimeStep;
};

#endif

