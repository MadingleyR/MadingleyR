#ifndef DATACOORDS
#define	DATACOORDS

class DataCoords {
public:
    DataCoords( const float&, const float&, const float& );
    DataCoords( const float&, const float& );
    ~DataCoords( );

    float GetLongitude( ) const;
    float GetLatitude( ) const;
    float GetDepth( ) const;

    void SetLongitude( const float& );
    void SetLatitude( const float& );
    void SetDepth( const float& );

private:
    void Initialise( const float&, const float&, const float& );
    
    float mLongitude;
    float mLatitude;
    float mDepth;
};

#endif
