#ifndef FILEWRITER
#define	FILEWRITER

#include "Types.h"

class FileWriter {
public:

    std::string OutputFolderName;


    FileWriter( );
    ~FileWriter( );
    
    bool WriteFiles( );
    
    std::string& GetOutputDirectory( );
    
private:
    void InitialiseOutputDirectory( );
    void WriteInputFiles( );

    std::string mOutputDirectory;
};

#endif

