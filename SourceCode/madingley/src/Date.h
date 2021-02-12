#ifndef DATE
#define	DATE

#include <sys/time.h>   // For struct timeval
#include "Types.h"
#include "Constants.h"

class Date {
public:
    Date( );
    ~Date( );

    static std::string GetDateAndTimeString( const std::string format = Constants::cDataSetNameFormat );
};

#endif

