#ifndef GRID
#define GRID

#include "GridCell.h"
#include "Parameters.h"

/** \brief A class containing the model grid (composed of individual grid cells) along with grid attributes. */
class Grid {
public:
    //empty placeholder constructor just to get started.
    Grid( );
    
    /** \brief Constructor for model grid to construct the grid for the globe */
    void SetUpGrid( );
    
    /** \brief  Get pointer to a viable cell to move to
    @param  gcl Pointer to the focal grid cell
    @param  v latitudinal displacement
    @param  u longitudinal displacement
    @return Pointer to cell that lies at displacement u,v from the current cell
    @remark Currently assumes wrapping in longitude, and a hard upper and lower boundary in latitude */
    GridCell* GetNewCell( GridCell* gridCell, const int& v, const int& u );

    Location GetNewLocation( const Location& L, const int& v, const int& u );

    GridCell& GetACell( const Location& L );
    
    GridCell& GetACell( const unsigned index );

    //Apply any function that operates on a cell to all cells in the collection
    template <typename Function>
    void ApplyFunctionToAllCells( Function f ) {
        for( auto& j : mCells ) {
            f( j.second );
        }
    }

private:
    // Variable to make sure that not more than one grid is instantiated
    unsigned mNumGrids;
    /** \brief Array of grid cells */
    Types::GridCellMap mCells;
};

#endif
