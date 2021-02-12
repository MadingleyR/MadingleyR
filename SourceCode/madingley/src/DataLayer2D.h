#ifndef DATALAYER2D
#define	DATALAYER2D

#include "DataLayer.h"

class DataLayer2D: public DataLayer {
public:
    DataLayer2D( const std::string&, const Types::VariableVector&, const Types::VariablePointer, const Types::VariablePointer );
    ~DataLayer2D( );

    float GetDataAtCellIndex( const unsigned ) const;

private:
};

#endif

