#ifndef ECOLOGYSTOCK
#define ECOLOGYSTOCK

#include "AutotrophProcessor.h"
#include "TerrestrialCarbon.h"
#include "HANPP.h"

#include "MadingleyInitialisation.h"
#include "Parameters.h"

/** \brief A class to specify, initialise and run ecological processes pertaining to stocks */
class EcologyStock {
public:

    /** \brief Run ecological processes that operate on stocks within a single grid cell 
    @param gcl The current grid cell 
    @param actingStock The acting stock 
    @param currentTimeStep The current model time step 
    @param currentMonth The current model month 
    @param params Parameters */
    void RunWithinCellEcology( GridCell&, Stock&, unsigned, unsigned, MadingleyInitialisation& );

    /** \brief An instance of the Autotroph Processor for this model */
    AutotrophProcessor mMarineNPPtoAutotrophStock;
    /** \brief An instance of the plant model class */
    TerrestrialCarbon mDynamicPlantModel;
    /** \brief An instance of the class for human appropriation of NPP */
    HumanAutotrophMatterAppropriation mHANPP;
};

#endif
