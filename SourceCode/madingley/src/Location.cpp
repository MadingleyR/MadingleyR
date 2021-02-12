#include "Location.h"

Location::Location( ) {
    mLongitudeIndex = 0;
    mLatitudeIndex = 0;
}

Location::Location( const Location& location ) {
    mLongitudeIndex = location.mLongitudeIndex;
    mLatitudeIndex = location.mLatitudeIndex;
}

void Location::SetIndices( unsigned latitudeIndex, unsigned longitudeIndex ) {
    mLatitudeIndex = latitudeIndex;
    mLongitudeIndex = longitudeIndex;
}


