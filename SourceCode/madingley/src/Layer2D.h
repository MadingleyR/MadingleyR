#ifndef LAYER2D
#define	LAYER2D

#include "Types.h"
#include "Layer.h"

class Layer2D : public Layer {
public:
    Layer2D( unsigned );
    ~Layer2D( );

    double& operator[]( int i ) {
        return mData[ i ];
    }

    void SetTime( unsigned );

    Types::DoubleVector mData;
};

#endif
