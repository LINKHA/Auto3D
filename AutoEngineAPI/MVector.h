#ifndef M_VECTOR_H_ 
#define M_VECTOR_H_

#include "MMath.h"



USING_MATH
AUTO_API_BEGIN

struct MVector2 : public Vector2
{
public:
	float x, y;
	void Set(float inX, float inY) { x = inX; y = inY; }
	float* GetPtr() { return &x; }
	const float* GetPtr()const { return &x; }

	static inline float Dot(const Vector2& lhs, const Vector2& rhs)				{ return MATH::Dot(lhs, rhs); }
	
	static inline float Magnitude(const Vector2& vec)							{ return MATH::sqrt(Dot(vec, vec)); }
	
	static inline Vector2 & Normalize(const Vector2& vec)						{ return MATH::Normalize(vec); }
	
	static inline Vector2 Lerp(const Vector2& from, const Vector2& to, float t) { return MATH::Lerp( from ,to , t); }
	
	static inline Vector2 Min(const Vector2& lhs, const Vector2& rhs)			{ return MATH::Min(lhs, rhs); }
	
	static inline Vector2 Max(const Vector2& lhs, const Vector2& rhs)			{ return MATH::Max(lhs, rhs); }
	
	static inline bool IsNormalized(const Vector2& vec)							{ return MATH::IsNormalized(vec); }
	
	static inline Vector2 Abs(const Vector2& v)									{ return MATH::Abs(v); }
	
	static inline float Angle(const Vector2& lhs, const Vector2& rhs)			{ return MATH::Angle(lhs, rhs); }
	
};

struct MVector3	: public Vector3
{
public:
	
};

struct MVector4 : public Vector4
{
public:
	
};

AUTO_API_END
#endif // !#endif // 