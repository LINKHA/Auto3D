#include "AreaAllocator.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

FAreaAllocator::FAreaAllocator()
{
    Reset(0, 0);
}

FAreaAllocator::FAreaAllocator(int width, int height, bool fastMode_)
{
    Reset(width, height, fastMode_);
}

FAreaAllocator::FAreaAllocator(int width, int height, int maxWidth, int maxHeight, bool fastMode_)
{
    Reset(width, height, maxWidth, maxHeight, fastMode_);
}

void FAreaAllocator::Reset(int width, int height, int maxWidth, int maxHeight, bool fastMode_)
{
    _doubleWidth = true;
    _size = TVector2I(width, height);
    _maxSize = TVector2I(maxWidth, maxHeight);
    _fastMode = fastMode_;

    _freeAreas.Clear();
    TRectI initialArea(0, 0, width, height);
    _freeAreas.Push(initialArea);
}

bool FAreaAllocator::Allocate(int width, int height, int& x, int& y)
{
    if (width < 0)
        width = 0;
    if (height < 0)
        height = 0;

    TVector<TRectI>::Iterator best;
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
                TRectI& first = _freeAreas.Front();
                if (_freeAreas.Size() == 1 && first.Left() == 0 && first.Top() == 0 && first.Right() == oldWidth && first.Bottom() == _size._y)
                    first.Right() = _size._x;
                else
                {
                    TRectI newArea(oldWidth, 0, _size._x, _size._y);
                    _freeAreas.Push(newArea);
                }
            }
            else if (!_doubleWidth && _size._y < _maxSize._y)
            {
                int oldHeight = _size._y;
                _size._y <<= 1;
                TRectI& first = _freeAreas.Front();
                if (_freeAreas.Size() == 1 && first.Left() == 0 && first.Top() == 0 && first.Right() == _size._x && first.Bottom() == oldHeight)
                    first.Bottom() = _size._y;
                else
                {
                    TRectI newArea(0, oldHeight, _size._x, _size._y);
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

    TRectI reserved(best->Left(), best->Top(), best->Left() + width, best->Top() + height);
    x = best->Left();
    y = best->Top();

    if (_fastMode)
    {
        // Reserve the area by splitting up the remaining free area
        best->Left() = reserved.Right();
        if (best->Height() > 2 * height || height >= _size._y / 2)
        {
            TRectI splitArea(reserved.Left(), reserved.Bottom(), best->Right(), best->Bottom());
            best->Bottom() = reserved.Bottom();
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

bool FAreaAllocator::SplitRect(TRectI original, const TRectI& reserve)
{
    if (reserve.Right() > original.Left() && reserve.Left() < original.Right() && reserve.Bottom() > original.Top() &&
        reserve.Top() < original.Bottom())
    {
        // Check for splitting from the right
        if (reserve.Right() < original.Right())
        {
            TRectI newRect = original;
            newRect.Left() = reserve.Right();
            _freeAreas.Push(newRect);
        }
        // Check for splitting from the left
        if (reserve.Left() > original.Left())
        {
            TRectI newRect = original;
            newRect.Right() = reserve.Left();
            _freeAreas.Push(newRect);
        }
        // Check for splitting from the bottom
        if (reserve.Bottom() < original.Bottom())
        {
            TRectI newRect = original;
            newRect.Top() = reserve.Bottom();
            _freeAreas.Push(newRect);
        }
        // Check for splitting from the top
        if (reserve.Top() > original.Top())
        {
            TRectI newRect = original;
            newRect.Bottom() = reserve.Top();
            _freeAreas.Push(newRect);
        }

        return true;
    }

    return false;
}

void FAreaAllocator::Cleanup()
{
    // Remove rects which are contained within another rect
    for (size_t i = 0; i < _freeAreas.Size();)
    {
        bool erased = false;
        for (size_t j = i + 1; j < _freeAreas.Size();)
        {
            if ((_freeAreas[i].Left() >= _freeAreas[j].Left()) &&
                (_freeAreas[i].Top() >= _freeAreas[j].Top()) &&
                (_freeAreas[i].Right() <= _freeAreas[j].Right()) &&
                (_freeAreas[i].Bottom() <= _freeAreas[j].Bottom()))
            {
                _freeAreas.Erase(i);
                erased = true;
                break;
            }
            if ((_freeAreas[j].Left() >= _freeAreas[i].Left()) &&
                (_freeAreas[j].Top() >= _freeAreas[i].Top()) &&
                (_freeAreas[j].Right() <= _freeAreas[i].Right()) &&
                (_freeAreas[j].Bottom() <= _freeAreas[i].Bottom()))
                _freeAreas.Erase(j);
            else
                ++j;
        }
        if (!erased)
            ++i;
    }
}

}
