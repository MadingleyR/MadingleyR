#ifndef INPUTDATUM
#define	INPUTDATUM

#include "Types.h"

class InputDatum {
public:
    InputDatum( const std::string&, const std::string&, const Types::UnsignedVector&, const unsigned&, float*, const bool );
    ~InputDatum( );
    
    std::string GetName( ) const;
    Types::VariablePointer GetVariable( const unsigned& ) const;
    unsigned GetNumberOfVariables( ) const;

    void AddVariable( const std::string&, const Types::UnsignedVector&, const unsigned&, float*, const bool );

private:
    Types::VariableVector mVariableVector;
    std::string mName;
};

#endif

