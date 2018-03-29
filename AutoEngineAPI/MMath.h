#ifndef M_MATH_H_
#define M_MATH_H_
#include "Auto.h"
#include "Math/AUMathBase.h"
#include "Math/AUMath.h"

USING_MATH
AUTO_API_BEGIN
class MMath
{
public:
	template<typename T> static inline T max(T v1, T v2)					{ return v1 > v2 ? v1 : v2; }
	template<typename T> static inline T min(T v1, T v2)					{ return v1 < v2 ? v1 : v2; }
	template<typename T> static inline T abs(const T &r)					{ return MATH::abs(r); }
	template<typename T> static inline T sqrt(const T& T)					{ return MATH::sqrt(T); }
	template<typename T> static inline T cos(T const& theta)				{ return MATH::cos(theta); }
	template<typename T> static inline T acos(T const& theta)				{ return MATH::acos(theta); }
	template<typename T> static inline T sin(T const& theta)				{ return MATH::sin(theta); }
	template<typename T> static inline T asin(T const& theta)				{ return MATH::asin(theta); }
	template<typename T> static inline T tan(T const& theta)				{ return MATH::tan(theta); }
	template<typename T> static inline T atan(T const& theta)				{ return MATH::atan(theta); }
	template<typename T> static inline void sincos(T const& u, T& s, T& c)	{ s = sin(u); c = cos(u); }
	template<typename T> static inline T pow(T const & x, T const& y)							{ return MATH::pow(x, y); }
	template<typename T> static inline T clamp(T const& x, T const& lo, T const& hi)			{ return  ((x < lo) ? lo : ((x > hi) ? hi : x)); }
	template<typename T, typename E> static inline T lerp(T const  & a, T const  & b, E x)		{ return a + (b - a)*x; }
	static inline float clamp01(float const&x)								{ return (x > 1 ? 1 : (0 > x) ? 0 : x); }
	static inline float clamp11(float const&x)								{ return (x > 1 ? 1 : (-1 > x) ? -1 : x); }
};
AUTO_API_END
#endif // MATH_F_H_
