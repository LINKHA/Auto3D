#pragma once
#include "Auto.h"
#include "MathBase.h"
#include "Vector2.h"
namespace Auto3D {
template <typename _Ty>
struct Rect
{
public:
	using RectType = Rect<_Ty>;
	using BaseType = float;

	_Ty x;
	_Ty y;
	_Ty width;
	_Ty height;

	Rect()
	{
		Reset();
	}

	Rect(_Ty inX, _Ty inY, _Ty iWidth, _Ty iHeight)	{ x = inX;  y = inY; width = iWidth; height = iHeight; }

	_Ty GetRight() const { return x + width; }

	_Ty GetBottom() const { return y + height; }

	void SetLeft(_Ty l) { T oldXMax = GetXMax(); x = l; width = oldXMax - x; }

	void SetTop(_Ty t) { T oldYMax = GetYMax(); y = t; height = oldYMax - y; }

	void SetRight(_Ty r) { width = r - x; }

	void SetBottom(_Ty b) { height = b - y; }

	_Ty GetXMax() const	{ return x + width; }

	_Ty GetYMax() const	{ return y + height; }

	bool IsEmpty() const { return width <= 0 || height <= 0; }

	void SetPosition(const Vector2& position) { x = position.x; y = position.y; }

	Vector2& GetPosition() const { return Vector2(x, y); }

	void SetSize(const Vector2& size) { width = size.x; height = size.y; }

	Vector2& GetSize() const { return Vector2(width, height); }

	void Reset() { x = y = width = height = 0; }

	void Set(_Ty inX, _Ty inY, _Ty iWidth, _Ty iHeight) { x = inX; width = iWidth; y = inY; height = iHeight; }
	
	void Normalize() { width = max<T>(width, 0);height = max<T>(height, 0); }

	bool operator == (const RectType& r)const { return x == r.x && y == r.y && width == r.width && height == r.height; }
	
	bool operator != (const RectType& r)const { return x != r.x || y != r.y || width != r.width || height != r.height; }
	
};
using Rectf = Rect<float>;
using RectInt = Rect<int>;

inline bool CompareApproximately(const Rectf& lhs, const Rectf& rhs)
{
	return CompareApproximately(lhs.x, rhs.x) && CompareApproximately(lhs.y, rhs.y) &&
		CompareApproximately(lhs.width, rhs.width) && CompareApproximately(lhs.height, rhs.height);
}

}

