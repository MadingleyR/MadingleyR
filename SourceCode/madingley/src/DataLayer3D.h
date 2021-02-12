#ifndef DATALAYER3D
#define	DATALAYER3D

#include "DataLayer.h"

class DataLayer3D: public DataLayer {
public:
    DataLayer3D( const std::string&, const Types::VariableVector, const Types::VariablePointer, const Types::VariablePointer, const Types::VariablePointer );
    ~DataLayer3D( );
    
    float GetDataAtCellIndex( const unsigned ) const;
    
    Types::VariablePointer GetDepthVariable( ) const;

private:
    Types::VariablePointer mDepthVariable;
};

#endif

