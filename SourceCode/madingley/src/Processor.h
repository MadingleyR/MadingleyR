#ifndef PROCESSOR
#define	PROCESSOR

#include "Types.h"

class Processor {
public:
    ~Processor( );
    static Types::ProcessorPointer Get( );

    int Indices2DToIndex( const int&, const int&, const unsigned& ) const;
    int Indices3DToIndex( const int&, const int&, const int&, const unsigned&, const unsigned& ) const;
    int Indices4DToIndex( const int&, const int&, const int&, const int&, const unsigned&, const unsigned&, const unsigned& ) const;

    int FindVariableIndexOfValue( const Types::VariablePointer, const double& ) const;
    int CalculateVariableIndexOfValue( const Types::VariablePointer, const double& ) const;
    int CalculateArrayIndexOfValue( const float*, const unsigned&, const double& ) const;
    
    template< class T, unsigned N >
    unsigned SizeOfArray( const T( &array )[ N ] ) const {
        // N has same value as count:
        unsigned count = sizeof ( array ) / sizeof ( array[ 0 ] );

        return count;
    }
    
private:
    Processor( );
    
    static Types::ProcessorPointer mThis;
};

#endif