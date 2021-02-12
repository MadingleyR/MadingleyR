#ifndef LAYER3D
#define	LAYER3D

#include "Types.h"
#include "Layer.h"

class Layer3D : public Layer {
public:
    Layer3D( unsigned sizeX, unsigned sizeY );
    ~Layer3D( );

    double& operator[]( int index ) {
        return mData[ mTimeIndex ][ index ];
    }

    void SetTime( unsigned );

    Types::DoubleMatrix mData;
    unsigned mTimeIndex;
};

#endif
