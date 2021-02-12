#ifndef DATALAYERPROCESSOR
#define	DATALAYERPROCESSOR

#include "Types.h"

class DataLayerProcessor {
public:
    DataLayerProcessor( );
    ~DataLayerProcessor( );
    
    Types::DataLayerMap ConvertReadDataIntoLayers( const Types::InputDataPointer ) const;
        
private:
    Types::DataLayerPointer MakeDataLayer( const Types::InputDatumPointer ) const;
    unsigned GetVariableType( const Types::VariablePointer ) const;
    float FindLargestVectorValueDifference( const Types::VariablePointer ) const;
    bool DoesTheGeographicVariableNeedAdjustment( const Types::VariablePointer ) const;
};

#endif

