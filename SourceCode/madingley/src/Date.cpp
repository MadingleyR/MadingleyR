#include "Date.h"
#include "Constants.h"

Date::Date( ) {

}

Date::~Date( ) {

}

std::string Date::GetDateAndTimeString( const std::string format ) {
    char dateTimeChar[ Constants::cDateTimeBufferSize ];
    timeval timeNow;

    gettimeofday( &timeNow, NULL );
    time_t rawtime = static_cast < time_t >( timeNow.tv_sec );

    struct tm *timeinfo;
    timeinfo = localtime( &rawtime );

    strftime( dateTimeChar, Constants::cDateTimeBufferSize, format.c_str( ), timeinfo );
    std::string dateTime( dateTimeChar );

    return dateTime;
}