#include "Stopwatch.h"

Stopwatch::Stopwatch( ) {
    mIsRunning = false;
}

void Stopwatch::Start( ) {
    // Set the start time for the stopwatch run
    mStartTime = std::chrono::high_resolution_clock::now( );
    // Set the stopwatch as being running
    mIsRunning = true;
}

void Stopwatch::Stop( ) {
    // Set the stop time for the stopwatch run
    mStopTime = std::chrono::high_resolution_clock::now( );
    // Set the stopwatch as being not running
    mIsRunning = false;
    // Calculate the time elapsed during this stopwatch run
    mInterval = ( mStopTime - mStartTime );
    // Update the time accumulated by this stopwatch instance
    mAccumulatedTime += mInterval;
}

double Stopwatch::GetElapsedTimeMillis( ) {
    //set the units for output - default is second so ratio of 1000 get milliseconds
    std::chrono::duration<double, std::ratio < 1, 1000 >> elapsed;

    // If the stopwatch is running, then calculate time since the stopwatch started, otherwise use the time elapsed during the last stopwatch run

    if( mIsRunning )
        elapsed = std::chrono::high_resolution_clock::now( ) - mStartTime;
    else
        elapsed = mInterval;

    return elapsed.count( );
}

double Stopwatch::GetElapsedTimeSecs( ) {
    //set the units for output = default is second
    std::chrono::duration< double > elapsed;
    if( mIsRunning )
        elapsed = std::chrono::high_resolution_clock::now( ) - mStartTime;
    else
        elapsed = mStopTime - mStartTime;

    return elapsed.count( );

}

double Stopwatch::AccumulatedTime( ) {
    //units for output - default is second
    return mAccumulatedTime.count( );

}