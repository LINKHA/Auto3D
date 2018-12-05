#pragma once
#include "MathBase.h"

namespace Auto3D {

struct Vector3
{
public:
	Vector3(): x(0.0), y(0.0), z(0.0) {}
	Vector3(float value) :x(value),y(value),z(value){}
	Vector3(const Vector3& vec){ x = vec.x; y = vec.y; z = vec.z; }
	Vector3(float xPos, float yPos, float zPos) : x(xPos) , y(yPos) , z(zPos) {}

	void Set(float inX, float inY, float inZ) { x = inX; y = inY; z = inZ; }
	void Set(const float* array) { x = array[0]; y = array[1]; z = array[2]; }

	float* GetPtr() { return &x; }
	const float* GetPtr()const { return &x; }

	glm::vec3 ToGLM() { return glm::vec3(x, y, z); }
	inline Vector3 operator-()const							{ return Vector3(-x, -y, -z); };
	inline Vector3 operator+(const Vector3& rhs) const		{ return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	inline Vector3 operator-(const Vector3& rhs) const		{ return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	inline Vector3 operator/(const float scale) const		{ return Vector3(x / scale, y / scale, z / scale); }
	inline Vector3 operator*(const float scale) const		{ return Vector3(x * scale, y*scale, z*scale); }
	inline Vector3 operator*(const Vector3& rhs) const		{ return Vector3(x*rhs.x, y*rhs.y, z*rhs.z); }

	inline Vector3&	operator=(const Vector3& rhs)			{ x = rhs.x; y = rhs.y; z = rhs.z; return *this; }
	inline Vector3& operator+=(const Vector3& rhs)			{ x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	inline Vector3& operator-=(const Vector3& rhs)			{ x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
	inline Vector3& operator*=(const float scale)			{ x *= scale; y *= scale; z *= scale; return *this; }
	inline Vector3& operator*=(const Vector3& rhs)			{ x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
	inline Vector3& operator/=(const float scale)			{ x /= scale; y /= scale; z /= scale; return *this; }

	const float& operator[] (int i)const					{ Assert(i >= 0 && i <= 2); return (&x)[i]; }
	float&		operator[] (int i)							{ Assert(i >= 0 && i <= 2); return (&x)[i]; }

	inline bool operator==(const Vector3& rhs) const		{ return (x == rhs.x) && (y == rhs.y) && (z == rhs.y); }
	inline bool operator!=(const Vector3& rhs) const		{ return (x != rhs.x) || (y != rhs.y) || (z != rhs.z); }
	inline bool operator<(const Vector3& rhs) const			{ if (x < rhs.x) return true; if (x > rhs.x) return false; if (y < rhs.y) return true; if (y > rhs.y) return false; if (z < rhs.z) return true; if (z > rhs.z) return false; return false; }

	float x;
	float y;
	float z;

	static const Vector3 zero;
	static const Vector3 one;
	static const Vector3 xAxis;
	static const Vector3 yAxis;
	static const Vector3 zAxis;
};
inline Vector3 ToAuto(glm::vec3 vec)										{ return Vector3(vec.x, vec.y, vec.z); }

inline float Dot(const Vector3& lhs, const Vector3& rhs)					{ return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }

inline float Magnitude(const Vector3& vec)									{ return sqrt(Dot(vec, vec)); }

inline Vector3 & Normalize(const Vector3& vec)								{ return vec / Magnitude(vec); }

inline Vector3 Lerp(const Vector3& from, const Vector3& to, float t)		{ return lerp(from, to, t); }

Vector3 Slerp(const Vector3& from, const Vector3& to, float t);

inline Vector3 Min(const Vector3& lhs, const Vector3& rhs)					{ return Vector3(min(lhs.x, rhs.x), min(lhs.y, rhs.y),min(lhs.z,rhs.z)); }

inline Vector3 Max(const Vector3& lhs, const Vector3& rhs)					{ return Vector3(max(lhs.x, rhs.x), max(lhs.y, rhs.y), max(lhs.z, rhs.z)); }

inline bool IsNormalized(const Vector3& vec)								{ return Magnitude(vec) == 1.0f; }

inline Vector3 Abs(const Vector3& v)										{ return Vector3(abs(v.x), abs(v.y),abs(v.z)); }

inline Vector3 Cross(const Vector3& lhs, const Vector3& rhs)				{ return Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x); }

Vector3 RotateTowards(const Vector3& lhs, const Vector3& rhs, float maxAngle, float maxMagnitude);

Vector3 MoveTowards(const Vector3& lhs, const Vector3& rhs, float clampedDistance);

}

