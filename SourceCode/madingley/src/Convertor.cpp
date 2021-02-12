#include "Convertor.h"
#include "Constants.h"

#include <algorithm>

Types::ConvertorPointer Convertor::mThis = NULL;

Types::ConvertorPointer Convertor::Get( ) {
    if( mThis == NULL ) {
        mThis = new Convertor( );
    }
    return mThis;
}

Convertor::~Convertor( ) {
    if( mThis != NULL ) {
        delete mThis;
    }
}

Convertor::Convertor( ) {

}

double Convertor::StringToNumber( const std::string& string ) const {

    double number = strtod( string.c_str( ), NULL );

    return number;
}

const Types::StringVector Convertor::StringToWords( const std::string& inputString, const char wordTerminationCharacter ) const {

    std::stringstream stringStream( inputString );

    std::string word = "";
    Types::StringVector wordList;

    while( std::getline( stringStream, word, wordTerminationCharacter ) ) {
        wordList.push_back( word );
    }

    return wordList;
}

const std::string Convertor::DoubleToPrecisionString( const double& value, const unsigned& decimals ) const {

    std::ostringstream outputStringStream;
    outputStringStream << std::fixed << std::setprecision( decimals ) << value;

    return outputStringStream.str( );
}

std::string Convertor::ToLowercase( const std::string inString ) const {
    std::string outString;

    std::transform( inString.begin( ), inString.end( ), std::back_inserter( outString ), tolower );

    return outString;
}

std::string Convertor::RemoveWhiteSpace( const std::string inString ) const {
    std::string outString = inString;

    outString.erase( remove( outString.begin( ), outString.end( ), Constants::cWhiteSpaceCharacter ), outString.end( ) );

    return outString;
}