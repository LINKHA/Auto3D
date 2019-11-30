#include "Batch2D.h"
#include "Graphics/Texture.h"

namespace Auto3D
{
		
inline bool CompareBatchState(FBatch2D& lhs, FBatch2D& rhs)
{
	return lhs._sortKey < rhs._sortKey;
}

void FBatch2DQueue::Clear()
{
	_batches.Clear();
}

void FBatch2DQueue::Sort(TVector<TMatrix3x4F>& instanceTransforms)
{
	Auto3D::Sort(_batches.Begin(), _batches.End(), CompareBatchState);
	// Build instances where adjacent batches have same state
	BuildInstances(_batches, instanceTransforms);
}

void FBatch2DQueue::BuildInstances(TVector<FBatch2D>& batches, TVector<TMatrix3x4F>& instanceTransforms)
{
	FBatch2D* start = nullptr;

	for (auto it = batches.Begin(), end = batches.End(); it != end; ++it)
	{
		FBatch2D* current = &*it;

		if (start && current->_type == EGeometryType::STATIC && current->_geometry == start->_geometry)
		{
			if (start->_type == EGeometryType::INSTANCED)
			{
				instanceTransforms.Push(*current->_worldMatrix);
				++start->_instanceCount;
			}
			else
			{
				// Begin new instanced batch
				start->_type = EGeometryType::INSTANCED;
				size_t instanceStart = instanceTransforms.Size();
				instanceTransforms.Push(*start->_worldMatrix);
				instanceTransforms.Push(*current->_worldMatrix);
				start->_instanceStart = instanceStart; // Overwrites non-instance world matrix
				start->_instanceCount = 2; // Overwrites sort _key / distance
			}
		}
		else
			start = (current->_type == EGeometryType::STATIC) ? current : nullptr;
	}
}



}