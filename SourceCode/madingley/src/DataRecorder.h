#ifndef DATARECORDER
#define	DATARECORDER

#include "Types.h"

class DataRecorder {
public:
    ~DataRecorder( );
    static Types::DataRecorderPointer Get( );

    bool Initialise( const Types::StringMatrix& );
    void SetDataOn( const std::string&, const float& );
    void AddDataTo( const std::string&, const float& );
    
    void SetDataOn( const std::string&, const unsigned&, const float& );
    
    Types::BasicDatumMap GetBasicDatumMap( ) const;
    Types::GridDatumMap GetGridDatumMap( ) const;
    
    Types::StringVector GetInputFilePaths( ) const;
    void AddInputFilePath( const std::string& );

private:
    DataRecorder( );
    static Types::DataRecorderPointer mThis;
    
    Types::BasicDatumPointer GetBasicDatum( const std::string& );
    Types::GridDatumPointer GetGridDatum( const std::string& );
    
    Types::BasicDatumMap mBasicDatumMap;
    Types::GridDatumMap mGridDatumMap;
    
    Types::StringMatrix mBasicOutputMetadata;
    Types::StringMatrix mGridOutputMetadata;
    
    Types::StringVector mInputFilePaths;
};

#endif

