#pragma once
#include "Auto.h"
#include "MathBase.h"
#include "Vector2.h"
MATH_BEGIN
template <typename T>
struct Rect
{
public:
	using RectType = Rect<T>;
	using BaseType = float;

	T x;
	T y;
	T width;
	T height;

	Rect()
	{
		Reset();
	}

	Rect(T inX, T inY, T iWidth, T iHeight)	{ x = inX;  y = inY; width = iWidth; height = iHeight; }

	T GetRight() const							{ return x + width; }
	T GetBottom() const							{ return y + height; }
	void SetLeft(T l)							{ T oldXMax = GetXMax(); x = l; width = oldXMax - x; }
	void SetTop(T t)							{ T oldYMax = GetYMax(); y = t; height = oldYMax - y; }
	void SetRight(T r)							{ width = r - x; }
	void SetBottom(T b)							{ height = b - y; }

	T GetXMax() const							{ return x + width; }
	T GetYMax() const							{ return y + height; }

	bool IsEmpty() const						{ return width <= 0 || height <= 0; }
	void SetPosition(const Vector2& position)	{ x = position.x; y = position.y; }
	Vector2& GetPosition() const					{ return Vector2(x, y); }
	void SetSize(const Vector2& size)			{ width = size.x; height = size.y; }
	Vector2& GetSize() const						{ return Vector2(width, height); }
	void Reset()								{ x = y = width = height = 0; }
	void Set(T inX, T inY, T iWidth, T iHeight) { x = inX; width = iWidth; y = inY; height = iHeight; }
	
	void Normalize()							{ width = max<T>(width, 0);height = max<T>(height, 0); }

	bool operator == (const RectType& r)const { return x == r.x && y == r.y && width == r.width && height == r.height; }
	bool operator != (const RectType& r)const { return x != r.x || y != r.y || width != r.width || height != r.height; }
};
using Rectf = Rect<float>;
using RectInt = Rect<int>;

MATH_END
