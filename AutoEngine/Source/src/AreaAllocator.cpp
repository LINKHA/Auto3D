#include "AreaAllocator.h"
#include "NewDef.h"

namespace Auto3D
{

AreaAllocator::AreaAllocator()
{
	Reset(0, 0);
}

AreaAllocator::AreaAllocator(int width, int height, bool fastMode_)
{
	Reset(width, height, fastMode_);
}

AreaAllocator::AreaAllocator(int width, int height, int maxWidth, int maxHeight, bool fastMode_)
{
	Reset(width, height, maxWidth, maxHeight, fastMode_);
}

void AreaAllocator::Reset(int width, int height, int maxWidth, int maxHeight, bool fastMode_)
{
	_doubleWidth = true;
	_size = Vector2I(width, height);
	_maxSize = Vector2I(maxWidth, maxHeight);
	_fastMode = fastMode_;

	_freeAreas.clear();
	RectInt initialArea(0, 0, width, height);
	_freeAreas.push_back(initialArea);
}

bool AreaAllocator::Allocate(int width, int height, int& x, int& y)
{
	if (width < 0)
		width = 0;
	if (height < 0)
		height = 0;

	VECTOR<RectInt>::iterator best;
	int bestFreeArea;

	for (;;)
	{
		best = _freeAreas.end();
		bestFreeArea = M_MAX_INT;
		for (auto i = _freeAreas.begin(); i != _freeAreas.end(); ++i)
		{
			int freeWidth = i->Width();
			int freeHeight = i->Height();

			if (freeWidth >= width && freeHeight >= height)
			{
				// Calculate rank for free area. Lower is better
				int freeArea = freeWidth * freeHeight;

				if (freeArea < bestFreeArea)
				{
					best = i;
					bestFreeArea = freeArea;
				}
			}
		}

		if (best == _freeAreas.end())
		{
			if (_doubleWidth && _size._x < _maxSize._x)
			{
				int oldWidth = _size._x;
				_size._x <<= 1;
				// If no allocations yet, simply expand the single free area
				RectInt& first = _freeAreas.front();
				if (_freeAreas.size() == 1 && first._left == 0 && first._top == 0 && first._right == oldWidth && first._bottom == _size._y)
					first._right = _size._x;
				else
				{
					RectInt newArea(oldWidth, 0, _size._x, _size._y);
					_freeAreas.push_back(newArea);
				}
			}
			else if (!_doubleWidth && _size._y < _maxSize._y)
			{
				int oldHeight = _size._y;
				_size._y <<= 1;
				RectInt& first = _freeAreas.front();
				if (_freeAreas.size() == 1 && first._left == 0 && first._top == 0 && first._right == _size._x && first._bottom == oldHeight)
					first._bottom = _size._y;
				else
				{
					RectInt newArea(0, oldHeight, _size._x, _size._y);
					_freeAreas.push_back(newArea);
				}
			}
			else
				return false;

			_doubleWidth = !_doubleWidth;
		}
		else
			break;
	}

	RectInt reserved(best->_left, best->_top, best->_left + width, best->_top + height);
	x = best->_left;
	y = best->_top;

	if (_fastMode)
	{
		// Reserve the area by splitting up the remaining free area
		best->_left = reserved._right;
		if (best->Height() > 2 * height || height >= _size._y / 2)
		{
			RectInt splitArea(reserved._left, reserved._bottom, best->_right, best->_bottom);
			best->_bottom = reserved._bottom;
			_freeAreas.push_back(splitArea);
		}
	}
	else
	{
		// Remove the reserved area from all free areas
		for (size_t i = 0; i < _freeAreas.size();)
		{
			if (splitRect(_freeAreas[i], reserved))
				_freeAreas.erase(_freeAreas.begin() + i);
			else
				++i;
		}

		cleanUp();
	}

	return true;
}

bool AreaAllocator::splitRect(RectInt original, const RectInt& reserve)
{
	if (reserve._right > original._left && reserve._left < original._right && reserve._bottom > original._top &&
		reserve._top < original._bottom)
	{
		// Check for splitting from the right
		if (reserve._right < original._right)
		{
			RectInt newRect = original;
			newRect._left = reserve._right;
			_freeAreas.push_back(newRect);
		}
		// Check for splitting from the left
		if (reserve._left > original._left)
		{
			RectInt newRect = original;
			newRect._right = reserve._left;
			_freeAreas.push_back(newRect);
		}
		// Check for splitting from the bottom
		if (reserve._bottom < original._bottom)
		{
			RectInt newRect = original;
			newRect._top = reserve._bottom;
			_freeAreas.push_back(newRect);
		}
		// Check for splitting from the top
		if (reserve._top > original._top)
		{
			RectInt newRect = original;
			newRect._bottom = reserve._top;
			_freeAreas.push_back(newRect);
		}

		return true;
	}

	return false;
}

void AreaAllocator::cleanUp()
{
	// Remove rects which are contained within another rect
	for (size_t i = 0; i < _freeAreas.size();)
	{
		bool erased = false;
		for (size_t j = i + 1; j < _freeAreas.size();)
		{
			if ((_freeAreas[i]._left >= _freeAreas[j]._left) &&
				(_freeAreas[i]._top >= _freeAreas[j]._top) &&
				(_freeAreas[i]._right <= _freeAreas[j]._right) &&
				(_freeAreas[i]._bottom <= _freeAreas[j]._bottom))
			{
				_freeAreas.erase(_freeAreas.begin() + i);
				erased = true;
				break;
			}
			if ((_freeAreas[j]._left >= _freeAreas[i]._left) &&
				(_freeAreas[j]._top >= _freeAreas[i]._top) &&
				(_freeAreas[j]._right <= _freeAreas[i]._right) &&
				(_freeAreas[j]._bottom <= _freeAreas[i]._bottom))
				_freeAreas.erase(_freeAreas.begin() + j);
			else
				++j;
		}
		if (!erased)
			++i;
	}
}

}
