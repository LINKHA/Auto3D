#ifndef VECTOR_4_H
#define VECTOR_4_H

#include "AUMathBase.h"
#include "LogAssert.h"

MATH_BEGIN
struct Vector4
{
public:
	float x, y, z, w;

	Vector4(): x(0.0), y(0.0), z(0.0), w(0.0) {}
	Vector4(const Vector4& vec): x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}
	Vector4(float xPos, float yPos , float zPos, float wPos) :x(xPos),y(yPos),z(zPos),w(wPos) {}

	void Set(float inX, float inY, float inZ, float inW) { x = inX; y = inY; z = inZ; w = inW; }
	void Set(const float* array) { x = array[0]; y = array[1]; z = array[2]; w = array[3]; }

	float* GetPtr() { return &x; }
	const float* GetPtr() const { return &x; }

	Vector4		operator-()const;
	Vector4		operator-(const Vector4& rhs)const;
	Vector4		operator+(const Vector4& rhs)const;
	Vector4		operator*(const Vector4& rhs)const;
	Vector4		operator*(const float scale)const;
	Vector4     operator/(const float scale) const;			
	

	Vector4&	operator=(const Vector4& rhs);
	Vector4&    operator+=(const Vector4& rhs);				
	Vector4&    operator-=(const Vector4& rhs);				
	Vector4&    operator*=(const float scale);				
	Vector4&    operator*=(const Vector4& rhs);				
	Vector4&    operator/=(const float scale);	

	const float& operator[] (int i)const;
	float& operator[] (int i);
	

	bool operator==(const Vector4& rhs)const;
	bool operator!=(const Vector4& rhs)const;
	bool operator<(const Vector4& rhs)const;


	
};

inline float Dot(const Vector4& lhs, const Vector4& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w; }

inline Vector4 Lerp(const Vector4& from, const Vector4& to, float t) { return lerp(from, to, t); }

MATH_END

#endif //VECTOR_4_H 