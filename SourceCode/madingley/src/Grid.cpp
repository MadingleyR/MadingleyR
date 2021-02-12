#include "Grid.h"

Grid::Grid( ) {
    mNumGrids = 0;
}

void Grid::SetUpGrid( ) {
    // Add one to the counter of the number of grids. If there is more than one model grid, exit the program with a debug crash.
    mNumGrids = mNumGrids + 1;
    assert( mNumGrids < 2 && "You have initialised more than one grid on which to apply models. At present, this is not supported" );
    //std::cout << "Initialising grid cell environment" << std::endl;

    // Instantiate a grid of grid cells
    for( unsigned index = 0; index < Parameters::Get( )->GetNumberOfGridCells( ); ++index ) {
        // Create the grid cell at the specified position
        mCells[ index ].SetCellCoords( index );
    }
	
    //std::cout<< "done" << std::endl;
}

GridCell* Grid::GetNewCell( GridCell* gridCell, const int& v, const int& u ) {
    GridCell* newCell = NULL;
    if( gridCell->GetLatitudeIndex( ) + v >= 0 && gridCell->GetLatitudeIndex( ) + v < Parameters::Get( )->GetLengthUserLatitudeArray( ) ) {
        int lnc = gridCell->GetLongitudeIndex( ) + u;
        while( lnc < 0 )lnc += Parameters::Get( )->GetLengthUserLongitudeArray( );
        while( lnc >= Parameters::Get( )->GetLengthUserLongitudeArray( ) )lnc -= Parameters::Get( )->GetLengthUserLongitudeArray( );
        long index = lnc + Parameters::Get( )->GetLengthUserLongitudeArray( ) * ( gridCell->GetLatitudeIndex( ) + v );
        if( mCells.count( index ) != 0 ) newCell = &( mCells[ index ] );
    }
    return newCell;
}

Location Grid::GetNewLocation( const Location& location, const int& v, const int& u ) {
    Location newLocation = location;
    if( location.mLatitudeIndex + v >= 0 && location.mLongitudeIndex + v < Parameters::Get( )->GetLengthUserLatitudeArray( ) ) {
        int lnc = location.mLongitudeIndex + u;
        while( lnc < 0 )lnc += Parameters::Get( )->GetLengthUserLongitudeArray( );
        while( lnc >= Parameters::Get( )->GetLengthUserLongitudeArray( ) )lnc -= Parameters::Get( )->GetLengthUserLongitudeArray( );
        long index = lnc + Parameters::Get( )->GetLengthUserLongitudeArray( ) * ( location.mLatitudeIndex + v );
        if( mCells.count( index ) != 0 ) {
            newLocation.SetIndices( mCells[ index ].GetLatitudeIndex( ), mCells[ index ].GetLongitudeIndex( ) );
        }
    }
    return newLocation;
}

GridCell& Grid::GetACell( const Location& location ) {
    long index = location.mLongitudeIndex + Parameters::Get( )->GetLengthUserLongitudeArray( ) * ( location.mLatitudeIndex );
    return mCells[ index ];
}

GridCell& Grid::GetACell( const unsigned index ) {
    return mCells[ index ];
}
