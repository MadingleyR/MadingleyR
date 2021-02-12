#ifndef CONVERTOR
#define	CONVERTOR

#include "Types.h"

#include <string>
#include <sstream>
#include <iostream>
//#include <stdlib.h>
#include <iomanip>

class Convertor {
public:
    ~Convertor( );
    static Types::ConvertorPointer Get( );

    template< class T >
    const std::string ToString( const T& input ) const {
        std::stringstream stringStream;
        stringStream << input;

        return stringStream.str( );
    }
    
    double StringToNumber( const std::string& ) const;

    const Types::StringVector StringToWords( const std::string&, const char ) const;
    const std::string DoubleToPrecisionString( const double&, const unsigned& ) const;

    std::string ToLowercase( const std::string ) const;
    std::string RemoveWhiteSpace( const std::string ) const;

private:
    Convertor( );

    static Types::ConvertorPointer mThis;
};

#endif

