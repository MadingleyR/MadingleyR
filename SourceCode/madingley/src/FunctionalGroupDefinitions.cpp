#include "FunctionalGroupDefinitions.h"
#include "InputParameters.h"


FunctionalGroupDefinitions::FunctionalGroupDefinitions( ) {

}

FunctionalGroupDefinitions::FunctionalGroupDefinitions( std::string fileName ) {
    //std::cout << "Reading \"" << ConfigurationDirectory << fileName << "\" functional group definitions." << std::endl;

    ConfigurationDirectory = InputParameters::Get( )->GetConfigurationDirectory();

    fileName = ConfigurationDirectory + fileName;
    std::ifstream infile( fileName.c_str( ) );

    if( infile.is_open( ) ) {
        std::string l;
        Types::StringVector header, category;
        getline( infile, l );
        //trim off newline character
        char lastCharHeader = l.back();
        if (lastCharHeader == ' ') { l.pop_back( );
        } else if (lastCharHeader == '\t') { l.pop_back( );
        } else if (lastCharHeader == '\n') { l.pop_back( );
        } else if (lastCharHeader == '\v') { l.pop_back( );
        } else if (lastCharHeader == '\f') { l.pop_back( );
        } else if (lastCharHeader == '\r') { l.pop_back( );
        }
        char nc = l.back();
        std::istringstream s( l );
        //split out the comma-separated and underscore separated header
        while( s.good( ) ) {
            std::string tmp;
            getline( s, tmp, ',' );
            std::transform( tmp.begin( ), tmp.end( ), tmp.begin( ), ::tolower );
            //split out the header category (definition.property or note)
            std::istringstream splt( tmp );
            std::string dp, op;
            getline( splt, dp, '_' );
            category.push_back( dp );
            getline( splt, op, '_' );
            header.push_back( op );
        }
        int count = 0;
        //retrieve the lines defining each functional group
        while( infile.good( ) ) {
            std::string l, data;
            getline( infile, l );
            if( infile.good( ) ) {
                char lastChar = l.back();
                if (lastChar == ' ') { l.pop_back( );
                } else if (lastChar == '\t') { l.pop_back( );
                } else if (lastChar == '\n') { l.pop_back( );
                } else if (lastChar == '\v') { l.pop_back( );
                } else if (lastChar == '\f') { l.pop_back( );
                } else if (lastChar == '\r') { l.pop_back( );
                }
                mAllFunctinoalGroupsIndex.push_back( count );
                if( l.length( ) > 1 ) {
                    std::istringstream s( l );
                    //step through the columns for this functional group
                    for( unsigned i = 0; i < header.size( ); i++ ) {
                        getline( s, data, ',' );
                        std::transform( data.begin( ), data.end( ), data.begin( ), ::tolower );

                        if( category[i] == "definition" ) {
                            //for each trait, store the value for a given functional group
                            //indexed by functional group number
                            mTraitLookupFromIndex[ header[ i ] ].push_back( data );
                            //for a given trait, store the functional group number
                            //which has a given value for that trait
                            mIndexLookupFromTrait[ header[ i ] ][ data ].push_back( count );
                        }
                        //Otherwise get the value for the given property
                        //for this functional group
                        if( category[i] == "property" ) {
                            mFunctionalGroupProperties[ header[ i ] ].push_back( atof( data.c_str( ) ) );
                        }
                    }
                }
                count++;
            }
        }
    } else {
        std::cout << "Something wrong with functional group definitions file " << fileName << std::endl;
    }
    infile.close( );

}

double FunctionalGroupDefinitions::GetBiologicalPropertyOneFunctionalGroup( std::string propertyName, int functionalGroup ) {
    std::transform( propertyName.begin( ), propertyName.end( ), propertyName.begin( ), ::tolower );
    return mFunctionalGroupProperties[ propertyName ][ functionalGroup ];
}

Types::DoubleVector FunctionalGroupDefinitions::GetBiologicalPropertyAllFunctionalGroups( std::string propertyName ) {
    std::transform( propertyName.begin( ), propertyName.end( ), propertyName.begin( ), ::tolower );
    return mFunctionalGroupProperties[ propertyName ];
}

Types::StringVector FunctionalGroupDefinitions::GetTraits( ) {
    Types::StringVector traits;

    for( auto var: mTraitLookupFromIndex ) {
        traits.push_back( var.first );
    }

    return traits;
}

Types::StringVector FunctionalGroupDefinitions::GetUniqueTraitValues( std::string trait ) {
    Types::StringVector traitValues;
    std::transform( trait.begin( ), trait.end( ), trait.begin( ), ::tolower );

    Types::IntegerVectorMap temp = mIndexLookupFromTrait[ trait ];

    for( auto var: temp ) {
        traitValues.push_back( var.first );
    }

    return traitValues;
}

std::string FunctionalGroupDefinitions::GetTraitNames( std::string searchTrait, int functionalGroupIndex ) {
    std::transform( searchTrait.begin( ), searchTrait.end( ), searchTrait.begin( ), ::tolower );

    return mTraitLookupFromIndex[ searchTrait ][ functionalGroupIndex ];
}

Types::StringVector FunctionalGroupDefinitions::GetTraitNames( Types::StringVector searchTraits, int functionalGroupIndex ) {
    Types::StringVector traitNames( searchTraits.size( ) );
    //
    for( auto sT: searchTraits ) {
        std::transform( sT.begin( ), sT.end( ), sT.begin( ), ::tolower );
        traitNames.push_back( mTraitLookupFromIndex[ sT ][ functionalGroupIndex ] );
    }
    return traitNames;
}

Types::IntegerVector FunctionalGroupDefinitions::GetFunctionalGroupIndex( Types::StringVector searchTraits, Types::StringVector searchTraitValues, bool intersection ) {
    // Check that the numbers of traits and of trait values specified are equal
    Types::IntegerVector result;
    assert( ( searchTraits.size( ) == searchTraitValues.size( ) ) && "Unequal search string arrays" );
    for( auto sT: searchTraits ) {
        if( mIndexLookupFromTrait.count( sT ) != 0 ) {
            for( auto V: searchTraitValues )
                if( mIndexLookupFromTrait[ sT ].count( V ) != 0 ) {
                    std::copy( mIndexLookupFromTrait[ sT ][ V ].begin( ), mIndexLookupFromTrait[ sT ][ V ].end( ), result.end( ) );
                }
        }
    }
    std::sort( result.begin( ), result.end( ) );
    if( intersection ) std::unique( result.begin( ), result.end( ) );
    return result;
}

Types::IntegerVector FunctionalGroupDefinitions::GetFunctionalGroupIndex( std::string searchTraits, std::string searchTraitValues, bool intersection ) {
    //List to hold the index vectors for each trait trait value pair
    Types::IntegerVector indexList;
    std::transform( searchTraits.begin( ), searchTraits.end( ), searchTraits.begin( ), ::tolower );
    std::transform( searchTraitValues.begin( ), searchTraitValues.end( ), searchTraitValues.begin( ), ::tolower );
    //Sorted dictionary to hold the trait value index list sorted dictionary from the lookup table
    //Types::IntegerVectorMap traitIndexList;

    //Check if the trait name is in the lookup table and if so pull out the <trait value, index vector> sorted dictionary for it
    if( mIndexLookupFromTrait.count( searchTraits ) != 0 ) {
        //Check if the trait value string is found in the lookup table and if found pull out the index vector for it
        //and add it to the List of these for processing - intersection of union
        if( mIndexLookupFromTrait[ searchTraits ].count( searchTraitValues ) != 0 ) {
            return mIndexLookupFromTrait[ searchTraits ][ searchTraitValues ];
        }//If trait value string not found then show error message
        else {
            std::cout << searchTraitValues << std::endl;
            std::cout << "Trait Value to search for not found in lookup tables" << std::endl;
            exit( 1 );
        }
    }//If trait name string not found then show error message
    else {
        std::cout << "Trait to search for not found in lookup tables" << std::endl;
        exit( 1 );
    }
    std::sort( indexList.begin( ), indexList.end( ) );
    if( intersection )std::unique( indexList.begin( ), indexList.end( ) );
    return indexList;
}

unsigned FunctionalGroupDefinitions::GetNumberOfFunctionalGroups( ) {
    return mAllFunctinoalGroupsIndex.size( );
}
