#pragma once
#include <cstdlib>
#include <cmath>
#include <limits>
#include "AutoSTL.h"
#include "AutoString.h"

namespace Auto3D {

#undef M_PI
static const float M_PI = 3.14159265358979323846264338327950288f;
static const float M_HALF_PI = M_PI * 0.5f;
static const int M_MIN_INT = 0x80000000;
static const int M_MAX_INT = 0x7fffffff;
static const unsigned M_MIN_UNSIGNED = 0x00000000;
static const unsigned M_MAX_UNSIGNED = 0xffffffff;

static const float M_EPSILON = 0.000001f;
static const float M_MAX_FLOAT = 3.402823466e+38f;
static const float M_LARGE_EPSILON = 0.00005f;
static const float M_MIN_NEARCLIP = 0.01f;
static const float M_MAX_FOV = 160.0f;
static const float M_LARGE_VALUE = 100000000.0f;
static const float M_INFINITY = (float)HUGE_VAL;
static const float M_DEGTORAD = M_PI / 180.0f;
static const float M_DEGTORAD_2 = M_PI / 360.0f;    // M_DEGTORAD / 2.f
static const float M_RADTODEG = 1.0f / M_DEGTORAD;

/// Intersection test result.
enum Intersection
{
	OUTSIDE,
	INTERSECTS,
	INSIDE
};

/// Swap two values.
template<class T> inline void Swap(T& first, T& second)
{
	T temp = first;
	first = second;
	second = temp;
}

/// Check whether two floating point values are equal within accuracy.
template <class T>
inline bool Equals(T lhs, T rhs) { return lhs + std::numeric_limits<T>::epsilon() >= rhs && lhs - std::numeric_limits<T>::epsilon() <= rhs; }

/// Linear interpolation between two values.
template <class T, class U>
inline T Lerp(T lhs, T rhs, U t) { return lhs * (1.0 - t) + rhs * t; }

/// Inverse linear interpolation between two values.
template <class T>
inline T InverseLerp(T lhs, T rhs, T x) { return (x - lhs) / (rhs - lhs); }

/// Return the smaller of two values.
template <class T, class U>
inline T Min(T lhs, U rhs) { return lhs < rhs ? lhs : rhs; }

/// Return the larger of two values.
template <class T, class U>
inline T Max(T lhs, U rhs) { return lhs > rhs ? lhs : rhs; }

/// Return absolute value of a value
template <class T>
inline T Abs(T value) { return value >= 0.0 ? value : -value; }

/// Return the sign of a float (-1, 0 or 1.)
template <class T>
inline T Sign(T value) { return value > 0.0 ? 1.0 : (value < 0.0 ? -1.0 : 0.0); }

/// Return a representation of the specified floating-point value as a single format bit layout.
inline unsigned FloatToRawIntBits(float value)
{
	unsigned u = *((unsigned*)&value);
	return u;
}

/// Check whether a floating point value is NaN.
template <class T> inline bool IsNaN(T value) { return std::isnan(value); }

/// Clamp a number to a range.
template <class T>
inline T Clamp(T value, T min, T max)
{
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}

/// Smoothly damp between values.
template <class T>
inline T SmoothStep(T lhs, T rhs, T t)
{
	t = Clamp((t - lhs) / (rhs - lhs), T(0.0), T(1.0)); // Saturate t
	return t * t * (3.0 - 2.0 * t);
}

/// Return sine of an angle in degrees.
template <class T> inline T Sin(T angle) { return sin(angle * M_DEGTORAD); }

/// Return cosine of an angle in degrees.
template <class T> inline T Cos(T angle) { return cos(angle * M_DEGTORAD); }

/// Return tangent of an angle in degrees.
template <class T> inline T Tan(T angle) { return tan(angle * M_DEGTORAD); }

/// Return arc sine in degrees.
template <class T> inline T Asin(T x) { return M_RADTODEG * asin(Clamp(x, T(-1.0), T(1.0))); }

/// Return arc cosine in degrees.
template <class T> inline T Acos(T x) { return M_RADTODEG * acos(Clamp(x, T(-1.0), T(1.0))); }

/// Return arc tangent in degrees.
template <class T> inline T Atan(T x) { return M_RADTODEG * atan(x); }

/// Return arc tangent of y/x in degrees.
template <class T> inline T Atan2(T y, T x) { return M_RADTODEG * atan2(y, x); }

/// Return X in power Y.
template <class T> inline T Pow(T x, T y) { return pow(x, y); }

/// Return natural logarithm of X.
template <class T> inline T Ln(T x) { return log(x); }

/// Return square root of X.
template <class T> inline T Sqrt(T x) { return sqrt(x); }

/// Return floating-point remainder of X/Y.
template <class T> inline T Mod(T x, T y) { return fmod(x, y); }

/// Return fractional part of passed value in range [0, 1).
template <class T> inline T Fract(T value) { return value - floor(value); }

/// Round value down.
template <class T> inline T Floor(T x) { return floor(x); }

/// Round value down. Returns integer value.
template <class T> inline int FloorToInt(T x) { return static_cast<int>(floor(x)); }

/// Round value to nearest integer.
template <class T> inline T Round(T x) { return round(x); }

/// Round value to nearest integer.
template <class T> inline int RoundToInt(T x) { return static_cast<int>(round(x)); }

/// Round value up.
template <class T> inline T Ceil(T x) { return ceil(x); }

/// Round value up.
template <class T> inline int CeilToInt(T x) { return static_cast<int>(ceil(x)); }

/// Check whether an unsigned integer is a power of two.
inline bool IsPowerOfTwo(unsigned value)
{
	return !(value & (value - 1));
}

/// Round up to next power of two.
inline unsigned NextPowerOfTwo(unsigned value)
{
	// http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
	--value;
	value |= value >> 1u;
	value |= value >> 2u;
	value |= value >> 4u;
	value |= value >> 8u;
	value |= value >> 16u;
	return ++value;
}

/// Return log base two or the MSB position of the given value.
inline unsigned LogBaseTwo(unsigned value)
{
	// http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogObvious
	unsigned ret = 0;
	while (value >>= 1)     // Unroll for more speed...
		++ret;
	return ret;
}

/// Count the number of set bits in a mask.
inline unsigned CountSetBits(unsigned value)
{
	// Brian Kernighan's method
	unsigned count = 0;
	for (count = 0; value; count++)
		value &= value - 1;
	return count;
}

/// Update a hash with the given 8-bit value using the SDBM algorithm.
inline unsigned SDBMHash(unsigned hash, unsigned char c) { return c + (hash << 6u) + (hash << 16u) - hash; }

/// Convert float to half float. From https://gist.github.com/martinkallman/5049614
inline unsigned short FloatToHalf(float value)
{
	unsigned inu = FloatToRawIntBits(value);
	unsigned t1 = inu & 0x7fffffffu;         // Non-sign bits
	unsigned t2 = inu & 0x80000000u;         // Sign bit
	unsigned t3 = inu & 0x7f800000u;         // Exponent

	t1 >>= 13;                              // Align mantissa on MSB
	t2 >>= 16;                              // Shift sign bit into position

	t1 -= 0x1c000;                          // Adjust bias

	t1 = (t3 < 0x38800000) ? 0 : t1;        // Flush-to-zero
	t1 = (t3 > 0x47000000) ? 0x7bff : t1;   // Clamp-to-max
	t1 = (t3 == 0 ? 0 : t1);                // Denormals-as-zero

	t1 |= t2;                               // Re-insert sign bit

	return (unsigned short)t1;
}

/// Convert half float to float. From https://gist.github.com/martinkallman/5049614
inline float HalfToFloat(unsigned short value)
{
	unsigned t1 = value & 0x7fffu;           // Non-sign bits
	unsigned t2 = value & 0x8000u;           // Sign bit
	unsigned t3 = value & 0x7c00u;           // Exponent

	t1 <<= 13;                              // Align mantissa on MSB
	t2 <<= 16;                              // Shift sign bit into position

	t1 += 0x38000000;                       // Adjust bias

	t1 = (t3 == 0 ? 0 : t1);                // Denormals-as-zero

	t1 |= t2;                               // Re-insert sign bit

	float out;
	*((unsigned*)&out) = t1;
	return out;
}

static inline bool CompareApproximately(float f0, float f1, float epsilon = M_EPSILON)
{
	float dist = f0 - f1;
	dist = Abs(dist);
	return dist < epsilon;
}

}

