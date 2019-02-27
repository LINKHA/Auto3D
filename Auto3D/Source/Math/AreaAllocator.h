#pragma once

#include "Rect.h"
#include "Vector2.h"
#include "../Base/Vector.h"

namespace Auto3D
{

/// Rectangular area allocator.
class AUTO_API AreaAllocator
{
public:
    /// Default construct with empty _size.
    AreaAllocator();
    /// Construct with given width and height.
    AreaAllocator(int width, int height, bool fastMode = true);
    /// Construct with given width and height, and set the maximum it is allowed to grow to.
    AreaAllocator(int width, int height, int maxWidth, int maxHeight, bool fastMode = true);

    /// Reset to given width and height and remove all previous allocations.
    void Reset(int width, int height, int maxWidth = 0, int maxHeight = 0, bool fastMode = true);
    /// Try to allocate a rectangle. Return true on success, with x & y coordinates filled.
    bool Allocate(int width, int height, int& x, int& y);

    /// Return the current _size.
    const Vector2I& Size() const { return _size; }
    /// Return the current width.
    int Width() const { return _size._x; }
    /// Return the current height.
    int Height() const { return _size._y; }
    /// Return the maximum _size.
    const Vector2I& MaxSize() const { return _maxSize; }
    /// Return the maximum width.
    int MaxWidth() const { return _maxSize._x; }
    /// Return the maximum height.
    int MaxHeight() const { return _maxSize._y; }
    /// Return whether uses fast mode. Fast mode uses a simpler allocation scheme which may waste free space, but is OK for eg. fonts.
    bool IsFastMode() const { return _fastMode; }

private:
    /// Remove space from a free rectangle. Return true if the original rectangle should be erased from the free list. Not called in fast mode.
    bool SplitRect(RectI original, const RectI& reserve);
    /// Clean up redundant free space. Not called in fast mode.
    void Cleanup();

    /// Free rectangles.
    Vector<RectI> _freeAreas;
    /// Current _size.
    Vector2I _size;
    /// Maximum _size allowed to grow to. It is zero when it is not allowed to grow.
    Vector2I _maxSize;
    /// The dimension used for next growth. Used internally.
    bool _doubleWidth;
    /// Fast mode flag.
    bool _fastMode;
};

}
