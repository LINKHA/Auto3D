#ifndef AU_MATH_BASE_H_
#define AU_MATH_BASE_H_
#include <memory>
#include <string.h>
#include "Auto.h"

#define MATH_DEG					0.0174532925f				//One degree equals several radians
#define MATH_RAD					57.29577951f				//One Radian equals several angles
#define MATH_FLOAT_SMALL            1.0e-37f				    //Infinite approach and 0
#define MATH_TOLERANCE              2e-37f
#define MATH_EPSILON                0.000001f					//A tiny floating point value (Read Only).
#define MATH_E						2.71828182845904523536f		//e
#define MATH_PI						3.14159265358979323846f		//pi
#define MATH_PI_2f					1.57079632679489661923f		//pi/2
#define MATH_PI_4f					0.785398163397448309616f	//pi/4

#define MATH_RANDOM_MINUS1_1()      ((2.0f*((float)rand()/RAND_MAX))-1.0f)      // Returns a random float between -1 and 1.
#define MATH_RANDOM_0_1()           ((float)rand()/RAND_MAX)                    // Returns a random float between 0 and 1.
#define MATH_RANDOM(x,y)			((y-x)*((float)rand()/RAND_MAX)+x)			// Returns a random float between min and max.
#define MATH_CLAMP(x, lo, hi)       ((x < lo) ? lo : ((x > hi) ? hi : x))		// if hi > lo return lo if hi <x reutrn x else reutnr x
#define MATH_CLAMP_0_1(x)			( x > 1 ? 1 :( 0 > x)? 0 : x)				
#define MATH_CLAMP_1_1(x)			( x > 1 ? 1 :( -1 > x)? -1 : x)			

MATH_BEGIN

static inline float abs(const float &r)
{
	return std::abs(r);
}

static inline  float sqrt(const float& r)
{
	return std::sqrt(r);
}

static inline float cos(float const& theta)
{
	return std::cos(theta);
}

static inline float acos(float const& theta)
{
	return std::acos(theta);
}

static inline float sin(float const& theta)
{
	return std::sin(theta);
}

static inline float asin(float const& theta)
{
	return std::asin(theta);
}

static inline float tan(float const& theta)
{
	return std::tan(theta);
}

static inline float atan(float const& theta)
{
	return std::atan(theta);
}

static inline void sincos(float const& u, float& s, float& c)
{
	s = sin(u);
	c = cos(u);
}

static inline  float pow(const float& x, const float& y)
{
	return std::pow(x, y);
}


static inline float lerp(float const& a, float const& b, float x)
{
	return a + x*(b - a);
}

static inline float lerp(float const& a, float const& b, float const& x)
{
	return a + x*(b - a);
}

static inline float clamp(float const& x, float const& lo, float const& hi)
{
	return  ((x < lo) ? lo : ((x > hi) ? hi : x));
}

static inline int clamp(int const& x, int const& lo, int const& hi)
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


//Smooth


MATH_END
#endif // !AU_MATH_BASE_H_
