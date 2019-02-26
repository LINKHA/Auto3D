#pragma once

#include "../AutoConfig.h"

#include <cstdlib>
#include <cmath>
#include <limits>

namespace Auto3D
{

#undef M_PI
static const float M_PI = 3.14159265358979323846264338327950288f;
static const float M_HALF_PI = M_PI * 0.5f;
static const int M_MIN_INT = 0x80000000;
static const int M_MAX_INT = 0x7fffffff;
static const unsigned M_MIN_UNSIGNED = 0x00000000;
static const unsigned M_MAX_UNSIGNED = 0xffffffff;

static const float M_EPSILON = 0.000001f;
static const float M_MAX_FLOAT = 3.402823466e+38f;
static const float M_INFINITY = (float)HUGE_VAL;
static const float M_DEGTORAD = (float)M_PI / 180.0f;
static const float M_DEGTORAD_2 = (float)M_PI / 360.0f; // M_DEGTORAD / 2.f
static const float M_RADTODEG = 1.0f / M_DEGTORAD;

/// Intersection test result.
enum Intersection
{
    OUTSIDE = 0,
    INTERSECTS,
    INSIDE
};

/// Check whether two floating point values are equal within accuracy.
template<typename _Ty> inline bool Equals(_Ty lhs, _Ty rhs) { return lhs + M_EPSILON >= rhs && lhs - M_EPSILON <= rhs; }
/// Check whether a floating point value is NaN.
template<typename _Ty> inline bool IsNaN(_Ty value) { return value != value; }
/// Linear interpolation between two float values.
template<typename _Ty, class U> inline _Ty Lerp(_Ty lhs, _Ty rhs, U t) { return lhs * (1.0 - t) + rhs * t; }
/// Return the smaller of two floats.
template<typename _Ty, typename U> inline _Ty Min(_Ty lhs, U rhs) { return lhs < rhs ? lhs : rhs; }
/// Return the larger of two floats.
template<typename _Ty, typename U> inline _Ty Max(_Ty lhs, U rhs) { return lhs > rhs ? lhs : rhs; }
/// Return absolute value of a float.
template<typename _Ty> inline _Ty Abs(_Ty value) { return value >= 0 ? value : -value; }
/// Return the sign of a float (-1, 0 or 1.)
template<typename _Ty> inline _Ty Sign(_Ty value) { return value > 0 ? 1 : (value < 0 ? -1 : 0); }

/// Clamp a float to a range.
template<typename _Ty> inline _Ty Clamp(float value, float min, float max)
{
    if (value < min)
        return min;
    else if (value > max)
        return max;
    else
        return value;
}

/// Smoothly damp between values.
template<typename _Ty> inline _Ty SmoothStep(_Ty lhs, _Ty rhs, _Ty t)
{
    t = Clamp((t - lhs) / (rhs - lhs), 0, 1); // Saturate t
    return t * t * (3 - 2 * t);
}

/// Return sine of an angle in degrees.
template<typename _Ty> inline _Ty Sin(_Ty angle) { return sinf(angle * M_DEGTORAD); }
/// Return cosine of an angle in degrees.
template<typename _Ty> inline _Ty Cos(_Ty angle) { return cosf(angle * M_DEGTORAD); }
/// Return tangent of an angle in degrees.
template<typename _Ty> inline _Ty Tan(_Ty angle) { return tanf(angle * M_DEGTORAD); }
/// Return arc sine in degrees.
template<typename _Ty> inline _Ty Asin(_Ty x) { return M_RADTODEG * asinf(Clamp(x, -1, 1)); }
/// Return arc cosine in degrees.
template<typename _Ty> inline _Ty Acos(_Ty x) { return M_RADTODEG * acosf(Clamp(x, -1, 1)); }
/// Return arc tangent in degrees.
template<typename _Ty> inline _Ty Atan(_Ty x) { return M_RADTODEG * atanf(x); }
/// Return arc tangent of y/x in degrees.
template<typename _Ty> inline _Ty Atan2(_Ty y, _Ty x) { return M_RADTODEG * atan2f(y, x); }


/// Clamp an _Tyeger to a range.
template<typename _Ty> inline _Ty Clamp(_Ty value, _Ty min, _Ty max)
{
    if (value < min)
        return min;
    else if (value > max)
        return max;
    else
        return value;
}

/// Check whether an unsigned integer is a power of two.
inline bool IsPowerOfTwo(unsigned value)
{
    if (!value)
        return true;
    while (!(value & 1))
        value >>= 1;
    return value == 1;
}

/// Round up to next power of two.
inline unsigned NextPowerOfTwo(unsigned value)
{
    unsigned ret = 1;
    while (ret < value && ret < 0x80000000)
        ret <<= 1;
    return ret;
}

}
