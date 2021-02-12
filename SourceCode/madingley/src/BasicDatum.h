#ifndef BASICDATUM
#define	BASICDATUM

#include "Types.h"

class BasicDatum {
public:
    BasicDatum( const std::string&, const std::string&, const std::string& );
    ~BasicDatum( );
    
    std::string GetName( ) const;
    std::string GetTimeUnit( ) const;
    std::string GetDataUnit( ) const;
    float* GetData( ) const;
    
    void SetData( const float& );
    void AddData( const float& );
    
private:
    std::string mName;
    std::string mTimeUnit;
    std::string mDataUnit;
    float* mData;
};

#endif

