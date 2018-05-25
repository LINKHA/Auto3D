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

	glm::vec4 ToGLM() { return glm::vec4(x, y, z, w); }
	inline Vector4 operator-()const							{ return Vector4(-x, -y, -z, -w); }
	inline Vector4 operator-(const Vector4& rhs)const		{ return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
	inline Vector4 operator+(const Vector4& rhs)const		{ return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	inline Vector4 operator*(const Vector4& rhs)const		{ return Vector4(x*rhs.x, y*rhs.y, z*rhs.z, w*rhs.w); }
	inline Vector4 operator*(const float scale)const		{ return Vector4(x*scale, y*scale, z*scale, w*scale); }
	inline Vector4 operator/(const float scale) const		{ return Vector4(x / scale, y / scale, z / scale, w / scale); }
	
	inline Vector4&	operator=(const Vector4& rhs)			{ x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; return *this; }
	inline Vector4& operator+=(const Vector4& rhs)			{ x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
	inline Vector4& operator-=(const Vector4& rhs)			{ x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
	inline Vector4& operator*=(const float scale)			{ x *= scale; y *= scale; z *= scale; w *= scale; return *this; }
	inline Vector4& operator*=(const Vector4& rhs)			{ x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w; return *this; }
	inline Vector4& operator/=(const float scale)			{ x /= scale; y /= scale; z /= scale; w /= scale; return *this; }

	const float& operator[] (int i)const					{ DebugAssertIf(i < 0 || i > 3); return (&x)[i]; }
	float& operator[] (int i)								{ DebugAssertIf(i < 0 || i > 3); return (&x)[i]; }
	
	inline bool operator==(const Vector4& rhs)const			{ return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w); }
	inline bool operator!=(const Vector4& rhs)const			{ return (x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w); }
	inline bool operator<(const Vector4& rhs)const			{ if (x < rhs.x) return true; if (x > rhs.x) return false; if (y < rhs.y) return true; if (y > rhs.y) return false; if (z < rhs.z) return true; if (z > rhs.z) return false; if (w < rhs.x) return true; if (w > rhs.w) return false; return false; }

};
inline Vector4 ToAuto(glm::vec4 vec) { return Vector4(vec.x, vec.y, vec.z, vec.w); }

inline float Dot(const Vector4& lhs, const Vector4& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w; }

inline Vector4 Lerp(const Vector4& from, const Vector4& to, float t) { return lerp(from, to, t); }

MATH_END

#endif //VECTOR_4_H 