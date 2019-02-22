#include "Rect.h"

namespace Auto3D {


const Rect Rect::FULL(-1.0f, -1.0f, 1.0f, 1.0f);
const Rect Rect::POSITIVE(0.0f, 0.0f, 1.0f, 1.0f);
const Rect Rect::ZERO(0.0f, 0.0f, 0.0f, 0.0f);

const RectInt RectInt::ZERO(0, 0, 0, 0);

void RectInt::Clip(const RectInt& rect)
{
	if (rect._left > _left)
		_left = rect._left;
	if (rect._right < _right)
		_right = rect._right;
	if (rect._top > _top)
		_top = rect._top;
	if (rect._bottom < _bottom)
		_bottom = rect._bottom;

	if (_left >= _right || _top >= _bottom)
		*this = RectInt();
}

void RectInt::Merge(const RectInt& rect)
{
	if (Width() <= 0 || Height() <= 0)
	{
		*this = rect;
	}
	else if (rect.Width() > 0 && rect.Height() > 0)
	{
		if (rect._left < _left)
			_left = rect._left;
		if (rect._top < _top)
			_top = rect._top;
		if (rect._right > _right)
			_right = rect._right;
		if (rect._bottom > _bottom)
			_bottom = rect._bottom;
	}
}

STRING Rect::ToString() const
{
	char tempBuffer[KhSTL::CONVERSION_BUFFER_LENGTH];
	sprintf(tempBuffer, "%g %g %g %g", _min._x, _min._y, _max._x, _max._y);
	return STRING(tempBuffer);
}

STRING RectInt::ToString() const
{
	char tempBuffer[KhSTL::CONVERSION_BUFFER_LENGTH];
	sprintf(tempBuffer, "%d %d %d %d", _left, _top, _right, _bottom);
	return STRING(tempBuffer);
}

void Rect::Clip(const Rect& rect)
{
	if (rect._min._x > _min._x)
		_min._x = rect._min._x;
	if (rect._max._x < _max._x)
		_max._x = rect._max._x;
	if (rect._min._y > _min._y)
		_min._y = rect._min._y;
	if (rect._max._y < _max._y)
		_max._y = rect._max._y;

	if (_min._x > _max._x || _min._y > _max._y)
	{
		_min = Vector2F(M_INFINITY, M_INFINITY);
		_max = Vector2F(-M_INFINITY, -M_INFINITY);
	}
}

}