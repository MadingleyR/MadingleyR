#include "Maths.h"

//#include "Parameters.h"

#include <math.h>
#include <cmath> 

Types::MathsPointer Maths::mThis = NULL;

Types::MathsPointer Maths::Get( ) {
    if( mThis == NULL ) {
        mThis = new Maths( );
    }
    return mThis;
}

Maths::~Maths( ) {
    if( mThis != NULL ) {
        delete mThis;
    }
}

Maths::Maths( ) {
    mPi = acos( -1. );
}

long double Maths::Pi( ) const {
    return mPi;
}

int Maths::Floor( const double& value ) const {
    return static_cast < int >( ::floor( value ) );
}

int Maths::Ceil( const double& value ) const {
    return static_cast < int >( ::ceil( value ) );
}

int Maths::Round( const double& value ) const {
    return static_cast < int >( ::floor( value + 0.5 ) );
}

double Maths::Abs( const double& value ) const {
    return std::abs( value );
}

double Maths::Min( const double& a, const double& b ) const {
    return ( a < b ) ? a : b;
}

double Maths::Max( const double& a, const double& b ) const {
    return ( a > b ) ? a : b;
}

double Maths::Log( const double& value ) const {
    return log( value );
}

double Maths::Log10( const double& value ) const {
    return log10( value );
}

double Maths::Exp( const double& exponent ) const {
    return exp( exponent );
}

double Maths::Pow( const double& base, const double& exponent ) const {
    return pow( base, exponent );
}

double Maths::NthRoot( const double& root, const double& degree ) const {
    return pow( root, 1.0 / degree );
}

unsigned Maths::Mod( const unsigned& dividend, const unsigned& divisor ) const {
    return dividend % divisor;
}