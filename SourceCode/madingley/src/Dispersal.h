#ifndef DISPERSAL
#define DISPERSAL

#include "Grid.h"
#include "Cohort.h"
#include "NonStaticSimpleRNG.h"

/** \brief Base Class for implementations of the ecological process of dispersal */
class Dispersal {
public:
    Dispersal( );

    /** \brief Run the dispersal implementation */
    virtual void Run( Grid&, Cohort*, const unsigned& );

    void ResetRandom( );

    void NewCell( Grid&, double&, double&, double&, double&, Cohort* );

    /** \brief Include Utility class */
    UtilityFunctions mUtilities;
    /** \brief An instance of the simple random number generator class */
    NonStaticSimpleRNG mRandomNumberA;
    NonStaticSimpleRNG mRandomNumberB;
};

#endif
