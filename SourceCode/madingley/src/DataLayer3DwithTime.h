#ifndef DATALAYER3DWITHTIME
#define	DATALAYER3DWITHTIME

#include "DataLayer.h"

class DataLayer3DwithTime: public DataLayer {
public:
    DataLayer3DwithTime( const std::string&, const Types::VariableVector, const Types::VariablePointer, const Types::VariablePointer, const Types::VariablePointer, const Types::VariablePointer );
    ~DataLayer3DwithTime( );

    float GetDataAtCellIndex( const unsigned ) const;

    Types::VariablePointer GetDepthVariable( ) const;
    Types::VariablePointer GetTimeVariable( ) const;

private:
    Types::VariablePointer mDepthVariable;
    Types::VariablePointer mTimeVariable;
};

#endif

