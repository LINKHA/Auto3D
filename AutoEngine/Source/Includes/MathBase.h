#pragma once
#include <memory>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Auto3D {
static const float MATH_DEG = 0.01745329251994329576923690768489f;			//One degree equals several radians
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


template<typename _Ty>
static inline _Ty max(_Ty v1, _Ty v2) { return v1 > v2 ? v1 : v2; }

template<>
static inline float max(float v1, float v2) { return v1 > v2 ? v1 : v2; }

template<typename _Ty>
static inline _Ty min(_Ty v1, _Ty v2) { return v1 < v2 ? v1 : v2; }

template<>
static inline float min(float v1, float v2) { return v1 < v2 ? v1 : v2; }

template<typename _Ty>
static inline _Ty abs(const _Ty &r)
{
	return std::abs(r);
}

template<typename _Ty>
static inline  _Ty sqrt(const _Ty& _Ty)
{
	return std::sqrt(_Ty);
}

template<typename _Ty>
static inline _Ty cos(_Ty const& theta)
{
	return std::cos(theta);
}

template<typename _Ty>
static inline _Ty acos(_Ty const& theta)
{
	return std::acos(theta);
}

template<typename _Ty>
static inline _Ty sin(_Ty const& theta)
{
	return std::sin(theta);
}

template<typename _Ty>
static inline _Ty asin(_Ty const& theta)
{
	return std::asin(theta);
}

template<typename _Ty>
static inline _Ty tan(_Ty const& theta)
{
	return std::tan(theta);
}

template<typename _Ty>
static inline _Ty atan(_Ty const& theta)
{
	return std::atan(theta);
}

template<typename _Ty>
static inline void sincos(_Ty const& u, _Ty& s, _Ty& c)
{
	s = sin(u);
	c = cos(u);
}

template<typename _Ty>
static inline  _Ty pow(_Ty const & x, _Ty const& y)
{
	return std::pow(x, y);
}


template<typename _Ty,typename E>
static inline _Ty lerp(_Ty const  & a, _Ty const  & b, E x)
{
	return a + (b - a)*x;
}

template<typename _Ty>
static inline _Ty clamp(_Ty const& x, _Ty const& lo, _Ty const& hi)
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
template<typename _Ty>
static inline _Ty radians(const _Ty& x) { return x * _Ty(MATH_DEG); }

static inline bool CompareApproximately(float f0, float f1, float epsilon = MATH_EPSILON)
{
	float dist = f0 - f1;
	dist = abs(dist);
	return dist < epsilon;
}

}

