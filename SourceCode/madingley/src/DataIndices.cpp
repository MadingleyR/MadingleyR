#include "DataIndices.h"

DataIndices::DataIndices( const unsigned& x, const unsigned& y, const unsigned& z ) {
    Initialise( x, y, z );
}

DataIndices::DataIndices( const unsigned& x, const unsigned& y ) {
    Initialise( x, y, 0 );
}

DataIndices::~DataIndices( ) {

}

void DataIndices::Initialise( const unsigned& x, const unsigned& y, const unsigned& z ) {
    mX = x;
    mY = y;
    mZ = z;
}

unsigned DataIndices::GetX( ) const {
    return mX;
}

unsigned DataIndices::GetY( ) const {
    return mY;
}

unsigned DataIndices::GetZ( ) const {
    return mZ;
}

void DataIndices::SetX( const unsigned& userX ) {
    mX = userX;
}

void DataIndices::SetY( const unsigned& userY ) {
    mY = userY;
}

void DataIndices::SetZ( const unsigned& z ) {
    mZ = z;
}