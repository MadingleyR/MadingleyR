#ifndef MATH
#define	MATH

#include "Types.h"
#include "Constants.h"
#include <typeinfo>

class Maths {
public:
    ~Maths( );
    static Types::MathsPointer Get( );
    
    long double Pi( ) const;

    int Floor( const double& ) const;
    int Ceil( const double& ) const;
    int Round( const double& ) const;

    double Abs( const double& ) const;

    double Min( const double&, const double& ) const;
    double Max( const double&, const double& ) const;

    double Log( const double& ) const;
    double Log10( const double& ) const;
    double Exp( const double& ) const;

    double Pow( const double&, const double& ) const;
    double NthRoot( const double&, const double& ) const;

    unsigned Mod( const unsigned&, const unsigned& ) const;

    template< class T >
    int Sum( const std::vector< T >& values ) const {

        int sum = 0;

        for( unsigned index = 0; index < values.size( ); ++index ) {
            sum += values[ index ];
        }

        return sum;
    }

    template< class T >
    int Product( const std::vector< T >& values ) const {

        int product;

        for( unsigned index = 0; index < values.size( ); ++index ) {
            if( index == 0 ) {
                product = values[ index ];
            } else {
                product *= values[ index ];
            }
        }

        return product;
    }

private:
    Maths( );

    static Types::MathsPointer mThis;
    
    long double mPi;
};

#endif

