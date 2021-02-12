#ifndef STOCK
#define STOCK

#include "FunctionalGroupDefinitions.h"
#include "GridCell.h"

/** \brief  Individual stocks */
class Stock {
public:
    /** \brief Constructor for stock class. Assigns stock starting properties
    @param StockDefinitions Definitions of Stock Properties
    @param Functional Group The functional group index of the stock being generated
    @param Environment The cell environment
    @param Success Whether the stock should be present in this cell */
    Stock( FunctionalGroupDefinitions&, const unsigned, GridCell&, bool& );

    /** \brief The total biomass of the stock */
    double mTotalBiomass;
    /** \brief The index of the functional group that the stock belongs to */
    unsigned mFunctionalGroupIndex;

private:
    /** \brief The mean body mass of an individual in this stock */
    double mIndividualBodyMass;

    GridCell* mCell;
};
#endif
