#include "Processor.h"

#include "Variable.h"
#include "Parameters.h"
#include "Maths.h"
#include "Convertor.h"

Types::ProcessorPointer Processor::mThis = NULL;

Types::ProcessorPointer Processor::Get( ) {
    if( mThis == NULL ) {
        mThis = new Processor( );
    }
    return mThis;
}

Processor::~Processor( ) {
    if( mThis != NULL ) {
        delete mThis;
    }
}

Processor::Processor( ) {

}

int Processor::Indices2DToIndex( const int& x, const int& y, const unsigned& xMax ) const {
    if( x != Constants::cMissingValue && y != Constants::cMissingValue )
        return ( y * xMax ) + x;
    else
        return Constants::cMissingValue;
}

int Processor::Indices3DToIndex( const int& x, const int& y, const int& t, const unsigned& xMax, const unsigned& yMax ) const {
    if( x != Constants::cMissingValue && y != Constants::cMissingValue && t != Constants::cMissingValue )
        return ( t * xMax * yMax ) + ( y * xMax ) + x;
    else
        return Constants::cMissingValue;
}

int Processor::Indices4DToIndex( const int& x, const int& y, const int& z, const int& t, const unsigned& xMax, const unsigned& yMax, const unsigned& zMax ) const {
    if( x != Constants::cMissingValue && y != Constants::cMissingValue && z != Constants::cMissingValue && t != Constants::cMissingValue )
        return ( t * xMax * yMax * zMax ) + ( z * xMax * yMax ) + ( y * xMax ) + x;
    else
        return Constants::cMissingValue;
}

int Processor::FindVariableIndexOfValue( const Types::VariablePointer variable, const double& value ) const {

    double lowestAbsoluteDifference = variable->GetMaximumDataDifference( );
    int closestIndex = Constants::cMissingValue;

    bool increasingVariable = variable->GetDataAtIndex( 0 ) < variable->GetDataAtIndex( variable->GetSize( ) - 1 );
    bool indexHasBeenSet = false;

    for( unsigned variableIndex = 0; variableIndex < variable->GetSize( ); ++variableIndex ) {
        double vectorValue = variable->GetDataAtIndex( variableIndex );
        double absoluteDifference = Maths::Get( )->Abs( value - vectorValue );

        if( increasingVariable == true ) {
            if( absoluteDifference <= lowestAbsoluteDifference ) {
                lowestAbsoluteDifference = absoluteDifference;
                closestIndex = variableIndex;
                indexHasBeenSet = true;
            } else if( indexHasBeenSet == true ) {
                break;
            }
        } else {
            if( absoluteDifference < lowestAbsoluteDifference ) {
                lowestAbsoluteDifference = absoluteDifference;
                closestIndex = variableIndex;
                indexHasBeenSet = true;
            } else if( indexHasBeenSet == true ) {
                break;
            }
        }
    }

    if( closestIndex == Constants::cMissingValue ) {
        std::cout << "Value \"" << value << "\" was not found in variable \"" << variable->GetName( ) << "\"." << std::endl;
    }

    return closestIndex;
}

int Processor::CalculateVariableIndexOfValue( const Types::VariablePointer variable, const double& value ) const {

    unsigned variableSize = variable->GetSize( );

    int calculatedIndex = Maths::Get( )->Round( ( variableSize - 1 ) * ( ( value - variable->GetDataAtIndex( 0 ) ) / ( variable->GetDataAtIndex( variableSize - 1 ) - variable->GetDataAtIndex( 0 ) ) ) );

    if( calculatedIndex > ( int )variableSize - 1 ) {
        calculatedIndex = variableSize - 1;
    } else if( calculatedIndex < 0 ) {
        calculatedIndex = 0;
    }

    return calculatedIndex;
}

int Processor::CalculateArrayIndexOfValue( const float* array, const unsigned& arraySize, const double& value ) const {
    
    int calculatedIndex = Maths::Get( )->Round( ( arraySize - 1 ) * ( ( value - array[ 0 ] ) / ( array[ arraySize - 1 ] - array[ 0 ] ) ) );
    
    if( calculatedIndex > ( int )arraySize - 1 ) {
        calculatedIndex = arraySize - 1;
    } else if( calculatedIndex < 0 ) {
        calculatedIndex = 0;
    }

    return calculatedIndex;
}