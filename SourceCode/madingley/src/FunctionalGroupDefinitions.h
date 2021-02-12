#ifndef FUNCTIONALGROUPDEFINITIONS
#define FUNCTIONALGROUPDEFINITIONS

#include "Constants.h"
#include "Types.h"
#include <iostream>

/** \brief Reads in and performs look-ups on functional group definitions
    @remark Mass bins values currently defined as middle of each mass bins</remarks> */
class FunctionalGroupDefinitions {
public:

    std::string ConfigurationDirectory;


    FunctionalGroupDefinitions( );

    /** \brief Constructor for the functional group definitions: reads in the specified functional group definition file, 
    Constructs lookup tables, mass ranges and initial cohort numbers in each functional group
    @param fileName The name of the functional group definition file to be read in */
    FunctionalGroupDefinitions( std::string );

    /** \brief Return the value of a biological parameter for a given parameter and functional group
    @param propertyName The name of the biological parameter
    @param functionalGroup Functional group index
    @return The value of the biological parameter for the specified functional group */

    double GetBiologicalPropertyOneFunctionalGroup( std::string, int );

    /** \brief Get values of a functional group property for all functional groups
    @param propertyName The name of the property to get values for
    @return The values of a functional group property for all functional groups */
    Types::DoubleVector GetBiologicalPropertyAllFunctionalGroups( std::string );

    /** \brief  Retrieves the values for all traits defined in the model
    @return String array of traits defined for the model */
    Types::StringVector GetTraits( );

    /** \brief    Retrieves the trait values for all traits defined in the model
    @param Trait The trait for which trait values are to be found
    @returns String array of trait values for the specifiec trait */
    Types::StringVector GetUniqueTraitValues( std::string trait );

    /** \brief Returns a string of Trait Names associated with the specified search trait and functional group index value
    @param searchTrait The name of the trait to get values for
    @param functionalGroupIndex The functional group index to return the trait value for
    @return The value of the specified trait for the specified functional group */
    std::string GetTraitNames( std::string searchTrait, int functionalGroupIndex );

    /** \brief Get the values of a set of specified traits for a specified functional group
    @param searchTraits A vector of trait names to get values for
    @param functionalGroupIndex The functional group index to return trait values for
    @returns A vector of values of the specified traits for a specified functional group */
    Types::StringVector GetTraitNames( Types::StringVector searchTraits, int functionalGroupIndex );

    /** \brief Get the functional group indices that have specified values of specified traits
    @param searchTraits Vector of trait names to search for
    @param searchTraitValues Vector of trait values to search for
    @param intersection Whether the intersection of the indices for the traits should be returned, otherwise return the union of the indices
    @return A vector of functional group indices with the specified values of the specified traits */
    Types::IntegerVector GetFunctionalGroupIndex( Types::StringVector searchTraits, Types::StringVector searchTraitValues, bool intersection );

    /** \brief Function to return the integer index values for functional groups corresponding to given trait and trait value pair combinations.
    Overloaded to accept a single string rather than an array in the traits to search and the trait values - both must be single strings
    @param searchTraits String of Trait names to search for trait values within
    @param searchTraitValues String of string Trait Values to find functional group indices for
    @param intersection Boolean statement indicating if you want the intersection of the indices. Only valid if more than one Trait and Trait Value pair.
    //        /// True means give intersection. False means give the union of indices
    @return Int array containing functional group indices corresponding to the given search conditions */
    Types::IntegerVector GetFunctionalGroupIndex( std::string, std::string, bool );

    /** \brief Returns number of functional groups 
       @returns>Number of functional groups*/
    unsigned GetNumberOfFunctionalGroups( );

    /** \brief A lookup device: sorted dictionary keyed by Functional Trait and valued by a sorted dictionary itself keyed by Unique Functional Trait Values and valued by an integer array of functional group indices corresponding to each functional trait value */
    Types::Integer2DVectorMap mIndexLookupFromTrait;
    /** \brief A sorted list of all of the properties of functional groups and their values */
    Types::DoubleVectorMap mFunctionalGroupProperties;
    /** \brief Dictionary to allow traits of functional groups to be looked up based on the functional group index*/
    Types::StringVectorMap mTraitLookupFromIndex;
    /** \brief A list of the indices of all functional groups in the model*/
    Types::IntegerVector mAllFunctinoalGroupsIndex;
};

#endif