#include "Vector4.hpp"




Vector4 & Vector4::Normalize()
{
	float xxyyzzww = x * x + y * y + z * z + w * w;
	float invLength = 1.0 / sqrt(xxyyzzww);
	x *= invLength;
	y *= invLength;
	z *= invLength;
	w *= invLength;
	return *this;
}

inline Vector4 Vector4::operator-() const
{
	return Vector4(-x,-y,-z,-w);
}

inline Vector4 Vector4::operator+(const Vector4 & rhs) const
{
	return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

inline Vector4 Vector4::operator*(const Vector4 & rhs) const
{
	return Vector4(x*rhs.x, y*rhs.y, z*rhs.z, w*rhs.w);
}

inline Vector4 Vector4::operator*(const float scale) const
{
	return Vector4(x*scale, y*scale, z*scale, w*scale);
}

inline Vector4 Vector4::operator/(const float scale) const
{
	return Vector4(x / scale, y / scale, z / scale, w / scale);
}


inline Vector4& Vector4::operator=(const Vector4 & rhs)
{
	x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; return *this;
}

inline Vector4 & Vector4::operator+=(const Vector4 & rhs)
{
	x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this;
}

inline Vector4 & Vector4::operator-=(const Vector4 & rhs)
{
	x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this;
}

inline Vector4 & Vector4::operator*=(const float scale)
{
	x *= scale; y *= scale; z *= scale; w *= scale; return *this;
}

inline Vector4 & Vector4::operator*=(const Vector4 & rhs)
{
	x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w; return *this;
}
inline Vector4 & Vector4::operator/=(const float scale)
{
	x /= scale; y /= scale; z /= scale; w /= scale; return *this;
}

inline bool  Vector4::operator==(const Vector4 & rhs)const
{
	return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w);
}

inline bool Vector4::operator!=(const Vector4 & rhs) const
{
	return (x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w);
}

inline bool Vector4::operator<(const Vector4 & rhs) const
{
	if (x < rhs.x) return true;
	if (x > rhs.x) return false;
	if (y < rhs.y) return true;
	if (y > rhs.y) return false;
	if (z < rhs.z) return true;
	if (z > rhs.z) return false;
	if (w < rhs.x) return true;
	if (w > rhs.w) return false;
	return false;
}
