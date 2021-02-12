#include "NonStaticSimpleRNG.h"

NonStaticSimpleRNG::NonStaticSimpleRNG( ) {
    // These values are not magical, just the default values Marsaglia used.
    // Any pair of unsigned integers should be fine.
    m_w = 521288629;
    m_z = 362436069;

    PI = acos( -1. );
    Temp2PI = 2.0 * PI;
}

void NonStaticSimpleRNG::Reset( ) {
    // These values are not magical, just the default values Marsaglia used.
    // Any pair of unsigned integers should be fine.
    m_w = 521288629;
    m_z = 362436069;
}

void NonStaticSimpleRNG::SetSeed( unsigned u, unsigned v ) {
    if( u != 0 ) m_w = u;
    if( v != 0 ) m_z = v;
}

void NonStaticSimpleRNG::SetSeed( unsigned u ) {
    m_w = u;
}

void NonStaticSimpleRNG::SetSeedFromSystemTime( ) {
    //System.DateTime dt = System.DateTime.Now;
    //long x = dt.ToFileTime();
    //SetSeed((uint)(x >> 16), (uint)(x % 4294967296));
    unsigned seed = std::chrono::system_clock::now( ).time_since_epoch( ).count( );

    SetSeed( seed >> 16, seed % 4294967296 );
}

double NonStaticSimpleRNG::GetUniform( ) {
    // 0 <= u < 2^32
    unsigned u = GetUint( );
    // The magic number below is 1/(2^32 + 2).
    // The result is strictly between 0 and 1.
    return ( u + 1.0 ) * 2.328306435454494e-10;
}

unsigned NonStaticSimpleRNG::GetUint( ) {
    m_z = 36969 * ( m_z & 65535 ) + ( m_z >> 16 );
    m_w = 18000 * ( m_w & 65535 ) + ( m_w >> 16 );
    return (m_z << 16 ) +m_w;
}

double NonStaticSimpleRNG::GetNormal( ) {
    // Use Box-Muller algorithm
    double u1 = GetUniform( );
    double u2 = GetUniform( );
    double r = sqrt( -2.0 * log( u1 ) );
    //double theta = 2.0 * Math.PI * u2;
    //return r * Math.Sin(theta);
    return r * sin( Temp2PI * u2 );
}

double NonStaticSimpleRNG::GetNormal( double mean, double standardDeviation ) {
    //if (standardDeviation <= 0.0)
    //{
    //string msg = string.Format("Shape must be positive. Received {0}.", standardDeviation);
    //throw new ArgumentOutOfRangeException(msg);
    assert( standardDeviation > 0. );
    //}
    return mean + standardDeviation * GetNormal( );
}

double NonStaticSimpleRNG::GetExponential( ) {
    return -log( GetUniform( ) );
}

double NonStaticSimpleRNG::GetExponential( double mean ) {
    //if (mean <= 0.0)
    //{
    //    string msg = string.Format("Mean must be positive. Received {0}.", mean);
    //    throw new ArgumentOutOfRangeException(msg);
    //}
    assert( mean > 0 );
    return mean * GetExponential( );
}

double NonStaticSimpleRNG::GetGamma( double shape, double scale ) {
    // Implementation based on "A Simple Method for Generating Gamma Variables"
    // by George Marsaglia and Wai Wan Tsang.  ACM Transactions on Mathematical Software
    // Vol 26, No 3, September 2000, pages 363-372.

    double d, c, x, xsquared, v, u;
    assert( shape > 0 );
    if( shape >= 1.0 ) {
        d = shape - 1.0 / 3.0;
        c = 1.0 / sqrt( 9.0 * d );
        for(;; ) {
            do {
                x = GetNormal( );
                v = 1.0 + c * x;
            } while( v <= 0.0 );
            v = v * v * v;
            u = GetUniform( );
            xsquared = x * x;
            if( u < 1.0 - .0331 * xsquared * xsquared || log( u ) < 0.5 * xsquared + d * ( 1.0 - v + log( v ) ) )
                return scale * d * v;
        }
    } else {
        double g = GetGamma( shape + 1.0, 1.0 );
        double w = GetUniform( );
        return scale * g * pow( w, 1.0 / shape );
    }
}

double NonStaticSimpleRNG::GetChiSquare( double degreesOfFreedom ) {
    return GetGamma( 0.5 * degreesOfFreedom, 2.0 );
}

double NonStaticSimpleRNG::GetInverseGamma( double shape, double scale ) {
    // If X is gamma(shape, scale) then
    // 1/Y is inverse gamma(shape, 1/scale)
    return 1.0 / GetGamma( shape, 1.0 / scale );
}

double NonStaticSimpleRNG::GetWeibull( double shape, double scale ) {

    assert( shape > 0. );
    assert( scale > 0 );

    return scale * pow( -log( GetUniform( ) ), 1.0 / shape );
}

double NonStaticSimpleRNG::GetCauchy( double median, double scale ) {
    assert( scale > 0 );


    double p = GetUniform( );

    // Apply inverse of the Cauchy distribution function to a uniform
    return median + scale * tan( PI * ( p - 0.5 ) );
}

double NonStaticSimpleRNG::GetLaplace( double mean, double scale ) {
    double u = GetUniform( );
    return ( u < 0.5 ) ?
            mean + scale * log( 2.0 * u ) :
            mean - scale * log( 2 * ( 1 - u ) );
}

double NonStaticSimpleRNG::GetLogNormal( double mu, double sigma ) {
    return exp( GetNormal( mu, sigma ) );
}

double NonStaticSimpleRNG::GetBeta( double a, double b ) {
    assert( a > 0 );
    assert( b > 0 );

    // There are more efficient methods for generating beta samples.
    // However such methods are a little more efficient and much more complicated.
    // For an explanation of why the following method works, see
    // http://www.johndcook.com/distribution_chart.html#gamma_beta

    double u = GetGamma( a, 1.0 );
    double v = GetGamma( b, 1.0 );
    return u / ( u + v );
}
