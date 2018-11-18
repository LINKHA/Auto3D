#pragma once
#include <memory>
#include <math.h>
#include <string.h>
#include "Auto.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Auto3D {
static const float MATH_DEG = 0.0174532925f;			//One degree equals several radians
static const float MATH_RAD = 57.29577951f;				//One Radian equals several angles
static const float MATH_FLOAT_SMALL = 1.0e-37f;		    //Infinite approach and 0
static const float MATH_TOLERANCE = 2e-37f;
static const float MATH_EPSILON = 0.00001f;				//A tiny floating point value (Read Only).
static const float MATH_E = 2.71828182845904523536f	;	//e
static const float MATH_PI = 3.14159265358979323846f;	//pi
static const float MATH_PI_2f = 1.57079632679489661923f;//pi/2
static const float MATH_PI_4f = 0.785398163397448309616f;//pi/4
static const float MATH_DEGTORAD = MATH_PI / 180.0f;
static const float MATH_DEGTORAD_2f = MATH_PI / 360.0f;    // MATH_DEGTORAD / 2.f
static const float MATH_RADTODEG = 1.0f / MATH_DEGTORAD;

static const int MATH_MIN_INT = 0x80000000;
static const int MATH_MAX_INT = 0x7fffffff;
static const unsigned MATH_MIN_UNSIGNED = 0x00000000;
static const unsigned MATH_MAX_UNSIGNED = 0xffffffff;


template<typename T>
static inline T max(T v1, T v2) { return v1 > v2 ? v1 : v2; }

template<>
static inline float max(float v1, float v2) { return v1 > v2 ? v1 : v2; }

template<typename T>
static inline T min(T v1, T v2) { return v1 < v2 ? v1 : v2; }

template<>
static inline float min(float v1, float v2) { return v1 < v2 ? v1 : v2; }

template<typename T>
static inline T abs(const T &r)
{
	return std::abs(r);
}

template<typename T>
static inline  T sqrt(const T& T)
{
	return std::sqrt(T);
}

template<typename T>
static inline T cos(T const& theta)
{
	return std::cos(theta);
}

template<typename T>
static inline T acos(T const& theta)
{
	return std::acos(theta);
}

template<typename T>
static inline T sin(T const& theta)
{
	return std::sin(theta);
}

template<typename T>
static inline T asin(T const& theta)
{
	return std::asin(theta);
}

template<typename T>
static inline T tan(T const& theta)
{
	return std::tan(theta);
}

template<typename T>
static inline T atan(T const& theta)
{
	return std::atan(theta);
}

template<typename T>
static inline void sincos(T const& u, T& s, T& c)
{
	s = sin(u);
	c = cos(u);
}

template<typename T>
static inline  T pow(T const & x, T const& y)
{
	return std::pow(x, y);
}


template<typename T,typename E>
static inline T lerp(T const  & a, T const  & b, E x)
{
	return a + (b - a)*x;
}

template<typename T>
static inline T clamp(T const& x, T const& lo, T const& hi)
{
	return  ((x < lo) ? lo : ((x > hi) ? hi : x));
}

static inline float clamp01(float const&x)
{
	return (x > 1 ? 1 : (0 > x) ? 0 : x);
}

static inline float clamp11(float const&x)
{
	return (x > 1 ? 1 : (-1 > x) ? -1 : x);
}

static inline bool CompareApproximately(float f0, float f1, float epsilon = MATH_EPSILON)
{
	float dist = f0 - f1;
	dist = abs(dist);
	return dist < epsilon;
}

}

