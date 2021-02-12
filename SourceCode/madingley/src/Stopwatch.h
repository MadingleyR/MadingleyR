#ifndef STOPWATCH
#define STOPWATCH

#include <chrono>

/** \brief Timer to track time elapsed */
class Stopwatch {
public:
    /** \brief Constructor - watch defaults to stopped */
    Stopwatch( );

    /** \brief Start the stopwatch */
    void Start( );

    /** \brief Stop the stopwatch */
    void Stop( );

    /** \brief Get the non-cumulative elapsed time of a stopwatch run in milliseconds */
    double GetElapsedTimeMillis( );

    /** \brief Get the non-cumulative elapsed time of a stopwatch run in seconds */
    double GetElapsedTimeSecs( );

    /** \brief Get the cumulative elapsed time of a stopwatch run in seconds */
    double AccumulatedTime( );

private:
    /** \brief The start time of a given stopwatch run */
    std::chrono::high_resolution_clock::time_point mStartTime;
    /** \brief The stop time of a given stopwatch run */
    std::chrono::high_resolution_clock::time_point mStopTime;
    /** \brief Whether the stopwatch is running */
    bool mIsRunning;
    /** \brief Time accumlated between last start and stop */
    std::chrono::duration< double > mInterval;
    /** \brief Time accumlated since this stopwatch was created */
    std::chrono::duration< double > mAccumulatedTime;

};
#endif
