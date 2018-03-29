#ifndef VECTOR_2_H_
#define VECTOR_2_H_

#include "AUMathBase.h"
#include "LogAssert.h"

//MATH_BEGIN
MATH_BEGIN
struct Vector2
{
public:
	Vector2() : x(0.0), y(0.0) {}
	Vector2(const Vector2& vec) { x = vec.x; y = vec.y; }
	Vector2(float xPos, float yPos) : x(xPos), y(yPos) {}

	void Set(float inX, float inY) { x = inX; y = inY; }
	float* GetPtr() { return &x; }
	const float* GetPtr()const { return &x; }
	// operators
	Vector2     operator-()const;							
	inline Vector2 operator+(const Vector2& rhs) const		{ return Vector2(x + rhs.x, y + rhs.y); }
	Vector2     operator-(const Vector2& rhs) const;		
	Vector2     operator*(const float scale) const;			
	Vector2     operator*(const Vector2& rhs) const;		
	Vector2     operator/(const float scale) const;		

	Vector2&    operator/=(const float scale);				
	Vector2&	operator=(const Vector2& rhs);
	Vector2&    operator+=(const Vector2& rhs);				
	Vector2&    operator-=(const Vector2& rhs);				
	Vector2&    operator*=(const float scale);				
	Vector2&    operator*=(const Vector2& rhs);				

	const float& operator[] (int i)const;
	float&		operator[] (int i);

	bool        operator==(const Vector2& rhs) const;		
	bool        operator!=(const Vector2& rhs) const;		
	bool        operator<(const Vector2& rhs) const;		

	float x;
	float y;

	static const Vector2	zero;
	static const Vector2	xAxis;
	static const Vector2	yAxis;
};

inline float Dot(const Vector2& lhs, const Vector2& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }

inline float Magnitude(const Vector2& vec) { return sqrt(Dot(vec, vec)); }

inline Vector2 & Normalize(const Vector2& vec) { return vec / Magnitude(vec); }

inline Vector2 Lerp(const Vector2& from, const Vector2& to, float t) { return lerp(from, to, t); }

inline Vector2 Min(const Vector2& lhs, const Vector2& rhs) { return Vector2(min(lhs.x, rhs.x), min(lhs.y, rhs.y)); }

inline Vector2 Max(const Vector2& lhs, const Vector2& rhs) { return Vector2(max(lhs.x, rhs.x), max(lhs.y, rhs.y)); }

inline bool IsNormalized(const Vector2& vec){ return Magnitude(vec) == 1.0f; }

inline Vector2 Abs(const Vector2& v) { return Vector2(abs(v.x), abs(v.y)); }

inline float Angle(const Vector2& lhs, const Vector2& rhs) { return acos(min(1.0f, max(-1.0f, Dot(lhs, rhs) / (Magnitude(lhs) * Magnitude(rhs))))); }

MATH_END

#endif // VECTOR_2_H_
