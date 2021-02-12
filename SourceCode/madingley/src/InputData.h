#ifndef INPUTDATA
#define	INPUTDATA

#include "Types.h"

class InputData {
public:
    InputData( );
    ~InputData( );

    void AddVariableToDatum( const std::string&, const std::string&, const Types::UnsignedVector&, const unsigned&, float*, const bool );
    Types::InputDatumPointer GetInputDatum( const std::string&, const bool isInternalSearch = false );
    Types::InputDatumPointer GetInputDatum( const unsigned );
    
    unsigned GetNumberOfInputDatums( );

private:
    Types::InputDatumVector mInputDatumVector;
};

#endif

