#include "Vector2.h"

MATH_BEGIN

const Vector2	Vector2::zero = Vector2(0, 0);
const Vector2	Vector2::xAxis = Vector2(1, 0);
const Vector2	Vector2::yAxis = Vector2(0, 1);



inline Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

/*inline Vector2 Vector2::operator+(const Vector2 & rhs) const
{
	return Vector2(x + rhs.x, y + rhs.y);
}*/

inline Vector2 Vector2::operator-(const Vector2 & rhs) const
{
	return Vector2(x - rhs.x, y - rhs.y);
}

inline Vector2 Vector2::operator*(const float scale) const
{
	return Vector2(x * scale, y*scale);
}

inline Vector2 Vector2::operator*(const Vector2 & rhs) const
{
	return Vector2(x*rhs.x, y*rhs.y);
}

inline Vector2 Vector2::operator/(const float scale) const
{
	return Vector2(x / scale, y / scale);
}

inline Vector2& Vector2::operator=(const Vector2& rhs)
{
	x = rhs.x; y = rhs.y; return *this;
}

inline Vector2 & Vector2::operator+=(const Vector2 & rhs)
{
	x += rhs.x; y += rhs.y; return *this;
}

inline Vector2 & Vector2::operator-=(const Vector2 & rhs)
{
	x -= rhs.x; y -= rhs.y; return *this;
}

inline Vector2 & Vector2::operator*=(const float scale)
{
	x *= scale; y *= scale; return *this;
}

inline Vector2 & Vector2::operator*=(const Vector2 & rhs)
{
	x *= rhs.x; y *= rhs.y; return *this;
}

inline Vector2 & Vector2::operator/=(const float scale)
{
	x /= scale; y /= scale; return *this;
}

inline bool Vector2::operator==(const Vector2 & rhs) const
{
	return (x == rhs.x) && (y == rhs.y);
}

inline bool Vector2::operator!=(const Vector2 & rhs) const
{
	return (x != rhs.x) || (y != rhs.y);
}

const float& Vector2::operator[] (int i)const
{
	DebugAssertIf(i < 0 || i > 1); return (&x)[i];
}

float& Vector2::operator[] (int i)
{
	DebugAssertIf(i < 0 || i > 1); return (&x)[i];
}

inline bool Vector2::operator<(const Vector2 & rhs) const
{
	if (x < rhs.x) return true;
	if (x > rhs.x) return false;
	if (y < rhs.y) return true;
	if (y > rhs.y) return false;
	return false;
}

MATH_END