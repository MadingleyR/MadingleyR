#ifndef FILEREADER
#define	FILEREADER

#include "Types.h"
#include "Constants.h"

class FileReader {
public:
    FileReader( );
    ~FileReader( );

    bool ReadFiles( );

private:
    bool ReadTextFile( const std::string& );
    
    bool ReadInputParameters( );
    bool SetUpOutputVariables( );

    void ClearMetadata( );
    
    std::string mFilePath;
    Types::StringVector mMetadataHeadings;
    Types::StringMatrix mMetadata;
    
    Types::IntegerVector mAllocatedCellIndices;
};

#endif

