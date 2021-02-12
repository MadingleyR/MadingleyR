#ifndef TYPES
#define TYPES

#include <vector>       // For std::vector
#include <sstream>      // For std::stringstream
#include <fstream>      // For std::ofstream
#include <algorithm>    // For std::sort
#include <assert.h>     // For assert
#include <map>          // For map
#include <sys/stat.h>   // For mkdir

class BasicDatum;
class Cohort;
class Convertor;
class DataCoords;
class DataIndices;
class DataLayer;
class DataLayerProcessor;
class DataLayerSet;
class DataRecorder;
class DispersalSet;
class Environment;
class FileReader;
class FileWriter;
class GridCell;
class GridDatum;
class Dispersal;
class InputData;
class InputDatum;
class Layer;
class Logger;
class MadingleyInitialisation;
class Maths;
class Parameters;
class InputParameters;
class Processor;
class Stock;
class TimeStep;
class Variable;

namespace Types {
    // Pointers
    typedef BasicDatum* BasicDatumPointer;
    typedef Convertor* ConvertorPointer;
    typedef DataCoords* DataCoordsPointer;
    typedef DataIndices* DataIndicesPointer;
    typedef DataLayer* DataLayerPointer;
    typedef DataLayerProcessor* DataLayerProcessorPointer;
    typedef DataLayerSet* DataLayerSetPointer;
    typedef DataRecorder* DataRecorderPointer;
    typedef DispersalSet* DispersalSetPointer;
    typedef Environment* EnvironmentPointer;
    typedef FileReader* FileReaderPointer;
    typedef FileWriter* FileWriterPointer;
    typedef GridDatum* GridDatumPointer;
    typedef GridCell* GridCellPointer;
    typedef Dispersal* DispersalPointer;
    typedef InputData* InputDataPointer;
    typedef InputDatum* InputDatumPointer;
    typedef Layer* LayerPointer;
    typedef Logger* LoggerPointer;
    typedef Maths* MathsPointer;
    typedef Parameters* ParametersPointer;
    typedef InputParameters* InputParametersPointer;
    typedef Processor* ProcessorPointer;
    typedef TimeStep* TimeStepPointer;
    typedef Variable* VariablePointer;

    // Containers of pointers/objects
    typedef std::map< long, GridCell > GridCellMap;

    typedef std::map< std::string, BasicDatumPointer > BasicDatumMap;
    typedef std::map< std::string, DataLayerPointer > DataLayerMap;
    typedef std::map< std::string, GridDatumPointer > GridDatumMap;
    typedef std::map< std::string, DispersalPointer > DispersalMap;
    typedef std::map< std::string, LayerPointer > LayerMap;

    typedef std::pair< DataCoordsPointer, DataIndicesPointer > CoordsIndicesPair;

    typedef std::vector< BasicDatumPointer > BasicDatumVector;
    typedef std::vector< Cohort > CohortVector;
    typedef std::vector< GridDatumPointer > GridDatumVector;
    typedef std::vector< InputDatumPointer > InputDatumVector;
    typedef std::vector< Stock > StockVector;
    typedef std::vector< VariablePointer > VariableVector;

    // Containers of primitives
    typedef std::map< std::string, double > DoubleMap;
    typedef std::map< std::string, std::string > StringMap;

    typedef std::vector< bool > BooleanVector;
    typedef std::vector< char > CharVector;
    typedef std::vector< unsigned char > UnsignedCharVector;
    typedef std::vector< double > DoubleVector;
    typedef std::vector< float > FloatVector;
    typedef std::vector< int > IntegerVector;
    typedef std::vector< long > LongVector;
    typedef std::vector< short > ShortVector;
    typedef std::vector< unsigned > UnsignedVector;
    typedef std::vector< unsigned short > UnsignedShortVector;
    typedef std::vector< std::string > StringVector;

    // Containers of containers of pointers/objects
    typedef std::map< int, StockVector > StocksMap;

    typedef std::vector< CoordsIndicesPair > CoordsIndicesVector;
    typedef std::vector< CohortVector > Cohort2DVector;
    typedef std::vector< DoubleVector > DoubleMatrix;
    typedef std::vector< StringVector > StringMatrix;
    typedef std::vector< UnsignedVector > UnsignedMatrix;

    // Containers of containers of primitives
    typedef std::map< std::string, DoubleMap > Double2DMap;
    typedef std::map< std::string, DoubleVector > DoubleVectorMap;
    typedef std::map< std::string, IntegerVector > IntegerVectorMap;
    typedef std::map< std::string, StringVector > StringVectorMap;

    // Container of containers of containers of primitives
    typedef std::map< std::string, IntegerVectorMap > Integer2DVectorMap;

    typedef std::vector< DoubleMatrix > Double3DMatrix;
}

#endif
