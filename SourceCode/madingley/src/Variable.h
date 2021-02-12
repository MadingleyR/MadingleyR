#ifndef VARIABLE
#define	VARIABLE

#include "Types.h"
#include "Constants.h"

class Variable {
public:
    Variable( const std::string&, const Types::UnsignedVector&, const unsigned&, float*, const bool );
    ~Variable( );
    
    std::string GetName( ) const;
    Types::UnsignedVector GetDimensions( ) const;
    unsigned GetDimension( const unsigned& ) const;
    unsigned GetSize( ) const;
    
    float* GetData( ) const;
    float GetDataAtIndex( const int& ) const;
    float GetMaximumDataDifference( ) const;
    
    bool IsDefault( ) const;
    
    void SetData( float*, const unsigned& );
    void SetDataAtIndex( const int&, const float& );
    void SetMaximumDataDifference( const float& );
    
protected:
    std::string mName;
    Types::UnsignedVector mDimensions;
    unsigned mSize;
    float* mData;
    float mMaximumDataDifference;
    bool mIsDefault;
};

#endif

