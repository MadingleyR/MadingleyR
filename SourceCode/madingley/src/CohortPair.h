#ifndef COHORTPAIR
#define	COHORTPAIR

#include "CohortMerger.h"

class CohortPair {
public:
    CohortPair( );
    CohortPair( Cohort*, Cohort*, double );

    Cohort* mCohortA;
    Cohort* mCohortB;
    double mDistance;

    struct Comparator {
        bool operator()( const CohortPair& u, const CohortPair& v ) {
            return ( u.mDistance < v.mDistance );
        }
    };
};
#endif
