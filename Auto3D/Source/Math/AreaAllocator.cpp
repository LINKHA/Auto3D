#include "AreaAllocator.h"

#include "../Debug/DebugNew.h"

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
    _size = IntVector2(width, height);
    _maxSize = IntVector2(maxWidth, maxHeight);
    _fastMode = fastMode_;

    _freeAreas.Clear();
    IntRect initialArea(0, 0, width, height);
    _freeAreas.Push(initialArea);
}

bool AreaAllocator::Allocate(int width, int height, int& x, int& y)
{
    if (width < 0)
        width = 0;
    if (height < 0)
        height = 0;

    Vector<IntRect>::Iterator best;
    int bestFreeArea;

    for (;;)
    {
        best = _freeAreas.End();
        bestFreeArea = M_MAX_INT;
        for (auto i = _freeAreas.Begin(); i != _freeAreas.End(); ++i)
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

        if (best == _freeAreas.End())
        {
            if (_doubleWidth && _size._x < _maxSize._x)
            {
                int oldWidth = _size._x;
                _size._x <<= 1;
                // If no allocations yet, simply expand the single free area
                IntRect& first = _freeAreas.Front();
                if (_freeAreas.Size() == 1 && first._left == 0 && first._top == 0 && first._right == oldWidth && first._bottom == _size._y)
                    first._right = _size._x;
                else
                {
                    IntRect newArea(oldWidth, 0, _size._x, _size._y);
                    _freeAreas.Push(newArea);
                }
            }
            else if (!_doubleWidth && _size._y < _maxSize._y)
            {
                int oldHeight = _size._y;
                _size._y <<= 1;
                IntRect& first = _freeAreas.Front();
                if (_freeAreas.Size() == 1 && first._left == 0 && first._top == 0 && first._right == _size._x && first._bottom == oldHeight)
                    first._bottom = _size._y;
                else
                {
                    IntRect newArea(0, oldHeight, _size._x, _size._y);
                    _freeAreas.Push(newArea);
                }
            }
            else
                return false;

            _doubleWidth = !_doubleWidth;
        }
        else
            break;
    }

    IntRect reserved(best->_left, best->_top, best->_left + width, best->_top + height);
    x = best->_left;
    y = best->_top;

    if (_fastMode)
    {
        // Reserve the area by splitting up the remaining free area
        best->_left = reserved._right;
        if (best->Height() > 2 * height || height >= _size._y / 2)
        {
            IntRect splitArea(reserved._left, reserved._bottom, best->_right, best->_bottom);
            best->_bottom = reserved._bottom;
            _freeAreas.Push(splitArea);
        }
    }
    else
    {
        // Remove the reserved area from all free areas
        for (size_t i = 0; i < _freeAreas.Size();)
        {
            if (SplitRect(_freeAreas[i], reserved))
                _freeAreas.Erase(i);
            else
                ++i;
        }

        Cleanup();
    }

    return true;
}

bool AreaAllocator::SplitRect(IntRect original, const IntRect& reserve)
{
    if (reserve._right > original._left && reserve._left < original._right && reserve._bottom > original._top &&
        reserve._top < original._bottom)
    {
        // Check for splitting from the right
        if (reserve._right < original._right)
        {
            IntRect newRect = original;
            newRect._left = reserve._right;
            _freeAreas.Push(newRect);
        }
        // Check for splitting from the left
        if (reserve._left > original._left)
        {
            IntRect newRect = original;
            newRect._right = reserve._left;
            _freeAreas.Push(newRect);
        }
        // Check for splitting from the bottom
        if (reserve._bottom < original._bottom)
        {
            IntRect newRect = original;
            newRect._top = reserve._bottom;
            _freeAreas.Push(newRect);
        }
        // Check for splitting from the top
        if (reserve._top > original._top)
        {
            IntRect newRect = original;
            newRect._bottom = reserve._top;
            _freeAreas.Push(newRect);
        }

        return true;
    }

    return false;
}

void AreaAllocator::Cleanup()
{
    // Remove rects which are contained within another rect
    for (size_t i = 0; i < _freeAreas.Size();)
    {
        bool erased = false;
        for (size_t j = i + 1; j < _freeAreas.Size();)
        {
            if ((_freeAreas[i]._left >= _freeAreas[j]._left) &&
                (_freeAreas[i]._top >= _freeAreas[j]._top) &&
                (_freeAreas[i]._right <= _freeAreas[j]._right) &&
                (_freeAreas[i]._bottom <= _freeAreas[j]._bottom))
            {
                _freeAreas.Erase(i);
                erased = true;
                break;
            }
            if ((_freeAreas[j]._left >= _freeAreas[i]._left) &&
                (_freeAreas[j]._top >= _freeAreas[i]._top) &&
                (_freeAreas[j]._right <= _freeAreas[i]._right) &&
                (_freeAreas[j]._bottom <= _freeAreas[i]._bottom))
                _freeAreas.Erase(j);
            else
                ++j;
        }
        if (!erased)
            ++i;
    }
}

}
