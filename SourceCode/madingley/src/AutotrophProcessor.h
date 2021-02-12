#ifndef AUTOTROPHPROCESSOR
#define AUTOTROPHPROCESSOR

#include "GridCell.h"
#include "Stock.h"

/** \brief Class for converting primary productivity estimates to autotroph biomass */
class AutotrophProcessor {
public:
    /** \brief Constructor for the autotroph processor: initialises necessary classes */
    AutotrophProcessor( );
    
    /** \brief Convert NPP estimate into biomass of an autotroph stock
    @param gcl The current grid cell 
    @param actingStock The stock to add biomass to */
    void ConvertNPPToAutotroph( GridCell&, Stock&, unsigned );

    /** \brief Instance of the class to perform general functions */
    UtilityFunctions mUtilities;
    /** \brief Factor to convert phytoplankton biomass from grams carbon to grams wet weight
    @remark Currently derived from Ho et al. (2003) J. Phycol., Dalsgaard and Pauly (1997) and Strickland (1966)*/
    double mPhytoplanktonConversionRatio;
    /** \brief Factor to convert NPP from units per m^2 to units per km^2 */
    double mMsqToKmSqConversion;
};
#endif
