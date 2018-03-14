#ifndef MATH_F_H_
#define MATH_F_H_
#include <cmath>

#define MATH_DEG					0.0174532925f				//One degree equals several radians
#define MATH_RAD					57.29577951f				//One Radian equals several angles
#define MATH_FLOAT_SMALL            1.0e-37f				    //Infinite approach and 0
#define MATH_TOLERANCE              2e-37f
#define MATH_EPSILON                0.000001f					//A tiny floating point value (Read Only).
#define MATH_E						2.71828182845904523536f		//e
#define MATH_PI						3.14159265358979323846f		//pi

struct Mathf
{
public:
	static inline float cos(float const& theta)
	{
		return std::cos(theta);
	}








};
#endif // MATH_F_H_
