#ifndef GRIDDATUM
#define	GRIDDATUM

#include "Types.h"

class GridDatum {
public:
    GridDatum( const std::string&, const std::string&, const std::string& );
    ~GridDatum( );
    
    std::string GetName( ) const;
    std::string GetTimeUnit( ) const;
    std::string GetDataUnit( ) const;
    float* GetData( ) const;
    
    void SetData( const unsigned&, const float& );
    
private:
    std::string mName;
    std::string mTimeUnit;
    std::string mDataUnit;
    float* mData;
};

#endif

