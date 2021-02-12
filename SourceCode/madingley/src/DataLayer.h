#ifndef DATALAYER
#define	DATALAYER

#include "Types.h"
#include "Variable.h"

class DataLayer {
public:

    virtual ~DataLayer( ) {
        for( unsigned variableIndex = 0; variableIndex < mVariableVector.size( ); ++variableIndex ) {
            delete mVariableVector[ variableIndex ];
        }
        delete mLongitudeVariable;
        delete mLatitudeVariable;
    };

    virtual float GetDataAtCellIndex( const unsigned ) const = 0;

    std::string GetName( );

    Types::VariablePointer GetDefaultVariable( ) const;
    Types::VariablePointer GetVariable( const std::string& ) const;

    Types::VariablePointer GetLongitudeVariable( ) const;
    Types::VariablePointer GetLatitudeVariable( ) const;

protected:
    Types::VariableVector mVariableVector;
    Types::VariablePointer mLongitudeVariable;
    Types::VariablePointer mLatitudeVariable;

    std::string mName;
};

#endif

