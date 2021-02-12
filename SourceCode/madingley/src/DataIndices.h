#ifndef DATAINDICES
#define	DATAINDICES

#include "Constants.h"

class DataIndices {
public:
    DataIndices( const unsigned&, const unsigned&, const unsigned& );
    DataIndices( const unsigned&, const unsigned& );
    ~DataIndices( );

    unsigned GetX( ) const;
    unsigned GetY( ) const;
    unsigned GetZ( ) const;

    void SetX( const unsigned& );
    void SetY( const unsigned& );
    void SetZ( const unsigned& );
private:
    void Initialise( const unsigned&, const unsigned&, const unsigned& );

    unsigned mX;
    unsigned mY;
    unsigned mZ;
};

#endif
