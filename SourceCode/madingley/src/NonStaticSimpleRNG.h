
#ifndef NONSTATICSIMPLERNG_H
#define	NONSTATICSIMPLERNG_H

#include <cmath>
#include <assert.h>
#include <chrono>

/// <summary>
/// SimpleRNG is a simple random number generator based on 
/// George Marsaglia's MWC (multiply with carry) generator.
/// Although it is very simple, it passes Marsaglia's DIEHARD
/// series of random number generator tests.
/// 
/// Written by John D. Cook 
/// http://www.johndcook.com
/// </summary>

class NonStaticSimpleRNG {
public:
    /// <summary>
    /// Constructor the random number generator: sets default values for the integers for George Marsaglia's MWC algorithm
    /// </summary>
    NonStaticSimpleRNG( );

    void Reset( );
    // The random generator seed can be set three ways:
    // 1) specifying two non-zero unsigned integers
    // 2) specifying one non-zero unsigned integer and taking a default value for the second
    // 3) setting the seed from the system time

    /// <summary>
    /// Set the seed of the random number generator using the specified integers
    /// </summary>
    /// <param name="u">An integer</param>
    /// <param name="v">An integer</param>
    void SetSeed( unsigned u, unsigned v );

    /// <summary>
    /// Set the seed of the random number generator using the specified integer
    /// </summary>
    /// <param name="u">An integer</param>
    void SetSeed( unsigned u );

    /// <summary>
    /// Set the seed of the random number generator based on the system time
    /// </summary>

    void SetSeedFromSystemTime( );

    /// <summary>
    /// A random draw from a uniform distribution between 0 and 1
    /// </summary>
    /// <returns>A random draw from a uniform distribution between 0 and 1</returns>
    /// <remarks>This will not return either 0 or 1</remarks>
    double GetUniform( );

    /// <summary>
    /// Get a random unsigned integer using uses George Marsaglia's MWC algorithm
    /// </summary>
    /// <returns>A random unsigned integer using uses George Marsaglia's MWC algorithm</returns>
    /// <remarks>See http://www.bobwheeler.com/statistics/Password/MarsagliaPost.txt </remarks>
    unsigned GetUint( );

    /// <summary>
    /// A random draw from a normal distribution with mean 0 and standard deviation 1
    /// </summary>
    /// <returns>A random draw from a normal distribution with mean 0 and standard deviation 1</returns>
    double GetNormal( );

    /// <summary>
    /// A random draw from a normal distribution
    /// </summary>
    /// <param name="mean">The mean of the normal distribution</param>
    /// <param name="standardDeviation">The standard deviation of the normal distribution</param>
    /// <returns>A random draw from a normal distribution</returns>
    double GetNormal( double mean, double standardDeviation );

    /// <summary>
    /// A random draw from an exponential distribution with mean 1
    /// </summary>
    /// <returns>A random draw from an exponential distribution with mean 1</returns>
    double GetExponential( );

    /// <summary>
    /// A random draw from the exponential distribution
    /// </summary>
    /// <param name="mean">The mean of the exponential distribution</param>
    /// <returns>A random draw from the exponential distribution</returns>
    double GetExponential( double mean );

    /// <summary>
    /// A random draw from the gamma distribution
    /// </summary>
    /// <param name="shape">The shape parameter of the gamma distribution</param>
    /// <param name="scale">The scale parameter of the gamma distribution</param>
    /// <returns>A random draw from the gamma distribution</returns>
    double GetGamma( double shape, double scale );

    /// <summary>
    /// A random draw from the chi-square distribution
    /// </summary>
    /// <param name="degreesOfFreedom">The degrees of freedom</param>
    /// <returns>A random draw from the chi-square distribution</returns>
    /// <remarks>A chi squared distribution with n degrees of freedom is a gamma distribution with shape n/2 and scale 2</remarks>
    double GetChiSquare( double degreesOfFreedom );

    /// <summary>
    /// A random draw from the inverse-gamma distribution
    /// </summary>
    /// <param name="shape">The shape parameter of the inverse-gamma distribution</param>
    /// <param name="scale">The scale parameter of the inverse-gamma distribution</param>
    /// <returns>A random draw from the inverse-gamma distribution</returns>
    double GetInverseGamma( double shape, double scale );

    /// <summary>
    /// A random draw from the Weibull distribution
    /// </summary>
    /// <param name="shape">The shape parameter of the Weibull distribution</param>
    /// <param name="scale">The scale parameter of the Weibull distribution</param>
    /// <returns>A random draw from the Weibull distribution</returns>
    double GetWeibull( double shape, double scale );

    /// <summary>
    /// A random draw from the Cauchy distribution
    /// </summary>
    /// <param name="median">The median of the Cauchy distribution</param>
    /// <param name="scale">The scale parameter of the Cauchy distribution</param>
    /// <returns>A random draw from the Cauchy distribution</returns>
    double GetCauchy( double median, double scale );

    /// <summary>
    /// A random draw from the Laplace distribution
    /// </summary>
    /// <param name="mean">The mean of the Laplace distribution</param>
    /// <param name="scale">The scale parameter of the Laplace distribution</param>
    /// <returns>A random draw from the Laplace distribution</returns>
    /// <remarks>The Laplace distribution is also known as the double exponential distribution</remarks>
    double GetLaplace( double mean, double scale );

    /// <summary>
    /// A random draw from a lognormal distribution
    /// </summary>
    /// <param name="mu">Mean of the lognormal distribution</param>
    /// <param name="sigma">Standard deviation of the lognormal distribution</param>
    /// <returns>A random draw from a lognormal distribution</returns>
    double GetLogNormal( double mu, double sigma );

    /// <summary>
    /// A random draw from the beta distribution
    /// </summary>
    /// <param name="a">Beta distribution 'a' parameter</param>
    /// <param name="b">Beta distribution 'b' parameter</param>
    /// <returns>A random draw from the beta distribution</returns>
    double GetBeta( double a, double b );

private:
    /// <summary>
    /// Integer for George Marsaglia's MWC algorithm 
    /// </summary>
    unsigned m_w;
    /// <summary>
    /// Integer for George Marsaglia's MWC algorithm
    /// </summary>
    unsigned m_z;
    double PI;
    double Temp2PI;
};
#endif
