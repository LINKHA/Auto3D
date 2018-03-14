#include "Vector2.h"

float Vector2::Length() const
{
	return sqrt(x * x + y * y);
}

float Vector2::Distance(const Vector2 & vec) const
{
	return sqrt((vec.x - x) * (vec.x - x) + (vec.y - y) * (vec.y - y));
}

float Vector2::Dot(const Vector2 & vec) const
{
	return x * vec.x + y * vec.y;
}

Vector2 & Vector2::Normalize()
{
	float xxyy = x * x + y * y;
	float invLength = 1.0 / sqrt(xxyy);
	x *= invLength;
	y *= invLength;
	return *this;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator+(const Vector2 & rhs) const
{
	return Vector2(x + rhs.x, y + rhs.y);
}

Vector2 Vector2::operator-(const Vector2 & rhs) const
{
	return Vector2(x - rhs.x, y - rhs.y);
}

Vector2 & Vector2::operator+=(const Vector2 & rhs)
{
	x += rhs.x; y += rhs.y; return *this;
}

Vector2 & Vector2::operator-=(const Vector2 & rhs)
{
	x -= rhs.x; y -= rhs.y; return *this;
}

Vector2 Vector2::operator*(const float scale) const
{
	return Vector2(x * scale, y*scale); 
}

Vector2 Vector2::operator*(const Vector2 & rhs) const
{
	return Vector2(x*rhs.x, y*rhs.y);
}

Vector2 & Vector2::operator*=(const float scale)
{
	x *= scale; y *= scale; return *this;
}

Vector2 & Vector2::operator*=(const Vector2 & rhs)
{
	x *= rhs.x; y *= rhs.y; return *this;
}

Vector2 Vector2::operator/(const float scale) const
{
	return Vector2(x / scale, y / scale);
}

Vector2 & Vector2::operator/=(const float scale)
{
	x /= scale; y /= scale; return *this;
}

bool Vector2::operator==(const Vector2 & rhs) const
{
	return (x == rhs.x) && (y == rhs.y);
}

bool Vector2::operator!=(const Vector2 & rhs) const
{
	return (x != rhs.x) || (y != rhs.y);
}

bool Vector2::operator<(const Vector2 & rhs) const
{
	if (x < rhs.x) return true;
	if (x > rhs.x) return false;
	if (y < rhs.y) return true;
	if (y > rhs.y) return false;
	return false;
}
