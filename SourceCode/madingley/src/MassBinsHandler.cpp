#include "MassBinsHandler.h"
#include "InputParameters.h"

MassBinsHandler::MassBinsHandler( ) {
    mNumMassBins = 50;
}

void MassBinsHandler::SetUpMassBins( std::string massBinsFile ) {

    ConfigurationDirectory = InputParameters::Get( )->GetConfigurationDirectory();
    massBinsFile = ConfigurationDirectory + massBinsFile;

    std::ifstream massFile( massBinsFile.c_str( ) );
    std::string title;
    if( massFile.is_open( ) ) {
        std::getline( massFile, title );
        float f;
        while( !massFile.eof( ) ) {
            massFile>>f;
            if( !massFile.eof( ) )mMassBins.push_back( f );
        }
        // Sort the array of mass bins
        std::sort( mMassBins.begin( ), mMassBins.end( ) );
        massFile.close( );
    } else {
        std::cout << "Problem with Mass Bins file!! " << massBinsFile << std::endl;
        exit( 1 );
    }
}
