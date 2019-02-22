// For conditions of distribution and use, see copyright notice in License.txt

#pragma once

#include "Rect.h"

namespace Auto3D
{
/*
	/// Rectangular area allocator.
	class AreaAllocator
	{
	public:
		/// Default construct with empty size.
		AreaAllocator();
		/// Construct with given width and height.
		AreaAllocator(int width, int height, bool fastMode = true);
		/// Construct with given width and height, and set the maximum it is allowed to grow to.
		AreaAllocator(int width, int height, int maxWidth, int maxHeight, bool fastMode = true);

		/// Reset to given width and height and remove all previous allocations.
		void Reset(int width, int height, int maxWidth = 0, int maxHeight = 0, bool fastMode = true);
		/// Try to allocate a rectangle. Return true on success, with x & y coordinates filled.
		bool Allocate(int width, int height, int& x, int& y);

		/// Return the current size.
		const Vector2I& Size() const { return size; }
		/// Return the current width.
		int Width() const { return size._x; }
		/// Return the current height.
		int Height() const { return size._y; }
		/// Return the maximum size.
		const Vector2I& MaxSize() const { return maxSize; }
		/// Return the maximum width.
		int MaxWidth() const { return maxSize._x; }
		/// Return the maximum height.
		int MaxHeight() const { return maxSize._y; }
		/// Return whether uses fast mode. Fast mode uses a simpler allocation scheme which may waste free space, but is OK for eg. fonts.
		bool IsFastMode() const { return fastMode; }

	private:
		/// Remove space from a free rectangle. Return true if the original rectangle should be erased from the free list. Not called in fast mode.
		bool SplitRect(RectInt original, const RectInt& reserve);
		/// Clean up redundant free space. Not called in fast mode.
		void Cleanup();

		/// Free rectangles.
		VECTOR<RectInt> freeAreas;
		/// Current size.
		Vector2I size;
		/// Maximum size allowed to grow to. It is zero when it is not allowed to grow.
		Vector2I maxSize;
		/// The dimension used for next growth. Used internally.
		bool doubleWidth;
		/// Fast mode flag.
		bool fastMode;
	};
*/
}
