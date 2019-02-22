#include "AreaAllocator.h"
#include "NewDef.h"

namespace Auto3D
{
//
//AreaAllocator::AreaAllocator()
//{
//	Reset(0, 0);
//}
//
//AreaAllocator::AreaAllocator(int width, int height, bool fastMode_)
//{
//	Reset(width, height, fastMode_);
//}
//
//AreaAllocator::AreaAllocator(int width, int height, int maxWidth, int maxHeight, bool fastMode_)
//{
//	Reset(width, height, maxWidth, maxHeight, fastMode_);
//}
//
//void AreaAllocator::Reset(int width, int height, int maxWidth, int maxHeight, bool fastMode_)
//{
//	doubleWidth = true;
//	size = Vector2(width, height);
//	maxSize = Vector2(maxWidth, maxHeight);
//	fastMode = fastMode_;
//
//	freeAreas.clear();
//	RectInt initialArea(0, 0, width, height);
//	freeAreas.push_back(initialArea);
//}
//
//bool AreaAllocator::Allocate(int width, int height, int& x, int& y)
//{
//	if (width < 0)
//		width = 0;
//	if (height < 0)
//		height = 0;
//
//	VECTOR<RectInt>::iterator best;
//	int bestFreeArea;
//
//	for (;;)
//	{
//		best = freeAreas.end();
//		bestFreeArea = M_MAX_INT;
//		for (auto i = freeAreas.begin(); i != freeAreas.end(); ++i)
//		{
//			int freeWidth = i->Width();
//			int freeHeight = i->Height();
//
//			if (freeWidth >= width && freeHeight >= height)
//			{
//				// Calculate rank for free area. Lower is better
//				int freeArea = freeWidth * freeHeight;
//
//				if (freeArea < bestFreeArea)
//				{
//					best = i;
//					bestFreeArea = freeArea;
//				}
//			}
//		}
//
//		if (best == freeAreas.end())
//		{
//			if (doubleWidth && size._x < maxSize._x)
//			{
//				int oldWidth = size._x;
//				size._x <<= 1;
//				// If no allocations yet, simply expand the single free area
//				RectInt& first = freeAreas.front();
//				if (freeAreas.size() == 1 && first._left == 0 && first._top == 0 && first.right == oldWidth && first.bottom == size.y)
//					first._right = size._x;
//				else
//				{
//					RectInt newArea(oldWidth, 0, size.x, size.y);
//					freeAreas.push_back(newArea);
//				}
//			}
//			else if (!doubleWidth && size.y < maxSize.y)
//			{
//				int oldHeight = size.y;
//				size.y <<= 1;
//				RectInt& first = freeAreas.Front();
//				if (freeAreas.Size() == 1 && first.left == 0 && first.top == 0 && first.right == size.x && first.bottom == oldHeight)
//					first.bottom = size.y;
//				else
//				{
//					RectInt newArea(0, oldHeight, size.x, size.y);
//					freeAreas.push_back(newArea);
//				}
//			}
//			else
//				return false;
//
//			doubleWidth = !doubleWidth;
//		}
//		else
//			break;
//	}
//
//	RectInt reserved(best->left, best->top, best->left + width, best->top + height);
//	x = best->left;
//	y = best->top;
//
//	if (fastMode)
//	{
//		// Reserve the area by splitting up the remaining free area
//		best->left = reserved.right;
//		if (best->Height() > 2 * height || height >= size.y / 2)
//		{
//			RectInt splitArea(reserved.left, reserved.bottom, best->right, best->bottom);
//			best->bottom = reserved.bottom;
//			freeAreas.push_back(splitArea);
//		}
//	}
//	else
//	{
//		// Remove the reserved area from all free areas
//		for (size_t i = 0; i < freeAreas.Size();)
//		{
//			if (SplitRect(freeAreas[i], reserved))
//				freeAreas.Erase(i);
//			else
//				++i;
//		}
//
//		Cleanup();
//	}
//
//	return true;
//}
//
//bool AreaAllocator::SplitRect(RectInt original, const RectInt& reserve)
//{
//	if (reserve.right > original.left && reserve.left < original.right && reserve.bottom > original.top &&
//		reserve.top < original.bottom)
//	{
//		// Check for splitting from the right
//		if (reserve.right < original.right)
//		{
//			RectInt newRect = original;
//			newRect.left = reserve.right;
//			freeAreas.push_back(newRect);
//		}
//		// Check for splitting from the left
//		if (reserve.left > original.left)
//		{
//			RectInt newRect = original;
//			newRect.right = reserve.left;
//			freeAreas.push_back(newRect);
//		}
//		// Check for splitting from the bottom
//		if (reserve.bottom < original.bottom)
//		{
//			RectInt newRect = original;
//			newRect.top = reserve.bottom;
//			freeAreas.push_back(newRect);
//		}
//		// Check for splitting from the top
//		if (reserve.top > original.top)
//		{
//			RectInt newRect = original;
//			newRect.bottom = reserve.top;
//			freeAreas.push_back(newRect);
//		}
//
//		return true;
//	}
//
//	return false;
//}
//
//void AreaAllocator::Cleanup()
//{
//	// Remove rects which are contained within another rect
//	for (size_t i = 0; i < freeAreas.Size();)
//	{
//		bool erased = false;
//		for (size_t j = i + 1; j < freeAreas.Size();)
//		{
//			if ((freeAreas[i].left >= freeAreas[j].left) &&
//				(freeAreas[i].top >= freeAreas[j].top) &&
//				(freeAreas[i].right <= freeAreas[j].right) &&
//				(freeAreas[i].bottom <= freeAreas[j].bottom))
//			{
//				freeAreas.Erase(i);
//				erased = true;
//				break;
//			}
//			if ((freeAreas[j].left >= freeAreas[i].left) &&
//				(freeAreas[j].top >= freeAreas[i].top) &&
//				(freeAreas[j].right <= freeAreas[i].right) &&
//				(freeAreas[j].bottom <= freeAreas[i].bottom))
//				freeAreas.Erase(j);
//			else
//				++j;
//		}
//		if (!erased)
//			++i;
//	}
//}

}
