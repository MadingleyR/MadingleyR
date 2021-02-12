#ifndef THREADVARIABLES
#define	THREADVARIABLES

class ThreadVariables {
public:
    /** \brief Hold some variables that need protection in original multi-threaded code */
    ThreadVariables( int extinctions, int productions, int combinations, long long nextCohortID ) : mExtinctions( extinctions ), mProductions( productions ), mCombinations( combinations ), mNextCohortID( nextCohortID ) {
        ;
    }
    
    /** \brief Thread-locked variable to track the cohort ID to assign to newly produced cohorts */
    long long mNextCohortID;
    /** \brief Thread-local variable to track the extinction of cohorts */
    long mExtinctions;
    /** \brief Thread-local variable to track the production of cohorts */
    long mProductions;
    /** \brief Variable to track the number of cohorts combined */
    long mCombinations;
};
#endif

