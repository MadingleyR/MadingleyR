#ifndef HANPP_H
#define HANPP_H

#include "GridCell.h"
#include "Constants.h"

/** \file HANPP.h
 * \brief the HANPP header file
 */

/** \brief Removes autotroph matter appropriated by humans from a grid cell's autotroph stocks
\remarks   Assumes that autotroph matter is appropriated evenly from different stocks in proportion to their biomass */
class HumanAutotrophMatterAppropriation {
public:
    //----------------------------------------------------------------------------------------------
    //Methods
    //----------------------------------------------------------------------------------------------

    /** \brief Constructor for human appropriation of autotroph matter */
    HumanAutotrophMatterAppropriation( );
    //----------------------------------------------------------------------------------------------

    /** \brief Remove human appropriated matter from the grid cell autotroph stocks
    @param gcl The current grid cell 
    @param humanNPPExtraction The type of NPP extraction to apply: 'no' = no removal; 'hanpp' = appropriated NPP estimate from input map; or proportion of total NPP 
    @param gridCellStocks The stocks in the current grid cell 
    @param actingStock The position of the acting stock in the jagged array of grid cell stocks 
    @param currentTimestep The current model time step */
    double RemoveHumanAppropriatedMatter(double , GridCell& , Stock& , unsigned , unsigned, bool );
    //----------------------------------------------------------------------------------------------
    /** \brief Instance of the class to perform general functions */
private:
    UtilityFunctions mUtilities;
};
#endif
