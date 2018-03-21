#include "Vector3.hpp"

inline float Vector3::Length() const
{
	return sqrt(x * x + y * y + z * z);
}

inline float Vector3::Distance(const Vector3 & vec) const
{
	return sqrt((vec.x - x) * (vec.x - x) + (vec.y - y) * (vec.y - y) + (vec.z - z)*(vec.z - z));
}

inline float Vector3::Dot(const Vector3 & vec) const
{
	return x * vec.x + y * vec.y + z * vec.z;
}

inline Vector3 & Vector3::Normalize()
{
	float xxyyzz = x * x + y * y + z * z;
	float invLength = 1.0 / sqrt(xxyyzz);
	x *= invLength;
	y *= invLength;
	z *= invLength;
	return *this;
}

inline Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

inline Vector3 & Vector3::operator=(const Vector3 & rhs)
{
	x += rhs.x; y += rhs.y; z += rhs.z; return *this;
}

inline Vector3 Vector3::operator+(const Vector3 & rhs) const
{
	return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

inline Vector3 Vector3::operator-(const Vector3 & rhs) const
{
	return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

inline Vector3 & Vector3::operator+=(const Vector3 & rhs)
{
	x += rhs.x; y += rhs.y; z += rhs.z; return *this;
}

inline Vector3 & Vector3::operator-=(const Vector3 & rhs)
{
	x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this;
}

inline Vector3 Vector3::operator*(const float scale) const
{
	return Vector3(x * scale, y*scale,z*scale);
}

inline Vector3 Vector3::operator*(const Vector3 & rhs) const
{
	return Vector3(x*rhs.x, y*rhs.y, z*rhs.z);
}

inline Vector3 & Vector3::operator*=(const float scale)
{
	x *= scale; y *= scale; z *= scale; return *this;
}

inline Vector3 & Vector3::operator*=(const Vector3 & rhs)
{
	x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this;
}

inline Vector3 Vector3::operator/(const float scale) const
{
	return Vector3(x / scale, y / scale, z / scale);
}

inline Vector3 & Vector3::operator/=(const float scale)
{
	x /= scale; y /= scale; z /= scale; return *this;
}

inline bool Vector3::operator==(const Vector3 & rhs) const
{
	return (x == rhs.x) && (y == rhs.y) && (z == rhs.y);
}

bool Vector3::operator!=(const Vector3 & rhs) const
{
	return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
}

inline bool Vector3::operator<(const Vector3 & rhs) const
{
	if (x < rhs.x) return true;
	if (x > rhs.x) return false;
	if (y < rhs.y) return true;
	if (y > rhs.y) return false;
	if (z < rhs.z) return true;
	if (z > rhs.z) return false;
	return false;
}
