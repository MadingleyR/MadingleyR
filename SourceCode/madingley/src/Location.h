#ifndef LOCATION
#define	LOCATION

class Location {
public:
    Location( );
    Location( const Location& );

    bool operator==( const Location& L_in ) {
        return ( mLongitudeIndex == L_in.mLongitudeIndex ) && ( mLatitudeIndex == L_in.mLatitudeIndex );
    }

    void SetIndices( unsigned, unsigned );
    
    unsigned mLongitudeIndex;
    unsigned mLatitudeIndex;
};

#endif

