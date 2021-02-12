#include "DataCoords.h"

DataCoords::DataCoords( const float& longitude, const float& latitude, const float& depth ) {
    Initialise( longitude, latitude, depth );
}

DataCoords::DataCoords( const float& longitude, const float& latitude ) {
    Initialise( longitude, latitude, 0 );
}

DataCoords::~DataCoords( ) {

}

void DataCoords::Initialise( const float& longitude, const float& latitude, const float& depth ) {
    mLongitude = longitude;
    mLatitude = latitude;
    mDepth = depth;
}

float DataCoords::GetLongitude( ) const {
    return mLongitude;
}

float DataCoords::GetLatitude( ) const {
    return mLatitude;
}

float DataCoords::GetDepth( ) const {
    return mDepth;
}

void DataCoords::SetLongitude( const float& longitude ) {
    mLongitude = longitude;
}

void DataCoords::SetLatitude( const float& latitude ) {
    mLatitude = latitude;
}

void DataCoords::SetDepth( const float& depth ) {
    mDepth = depth;
}