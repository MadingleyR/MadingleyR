#ifndef DATALAYER2DWITHTIME
#define	DATALAYER2DWITHTIME

#include "DataLayer.h"

class DataLayer2DwithTime: public DataLayer {
public:
    DataLayer2DwithTime( const std::string&, const Types::VariableVector, const Types::VariablePointer, const Types::VariablePointer, const Types::VariablePointer );
    ~DataLayer2DwithTime( );

    float GetDataAtCellIndex( const unsigned ) const;

    Types::VariablePointer GetTimeVariable( ) const;

private:
    Types::VariablePointer mTimeVariable;
};

#endif

