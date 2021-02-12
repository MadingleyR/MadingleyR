#include "Layer3D.h"

Layer3D::Layer3D( unsigned sizeX, unsigned sizeY ): mTimeIndex( 0 ) {
    mData.resize( sizeX );
    for( unsigned xIndex = 0; xIndex < sizeX; ++xIndex ) {
        mData[ xIndex ].resize( sizeY );
    }
}

Layer3D::~Layer3D( ) {
    for( unsigned index = 0; index < mData.size( ); ++index ) {
        mData[ index ].clear( );
    }
}

void Layer3D::SetTime( unsigned index ) {
    mTimeIndex = index;
}

