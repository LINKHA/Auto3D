#include "Batch2D.h"
#include "Graphics/Texture.h"

namespace Auto3D
{
		
inline bool CompareBatchState(Batch2D& lhs, Batch2D& rhs)
{
	return lhs._sortKey < rhs._sortKey;
}

void Batch2DQueue::Clear()
{
	_batches.Clear();
}

void Batch2DQueue::Sort(TVector<Matrix3x4F>& instanceTransforms)
{
	Auto3D::Sort(_batches.Begin(), _batches.End(), CompareBatchState);
	// Build instances where adjacent batches have same state
	BuildInstances(_batches, instanceTransforms);
}

void Batch2DQueue::BuildInstances(TVector<Batch2D>& batches, TVector<Matrix3x4F>& instanceTransforms)
{
	Batch2D* start = nullptr;

	for (auto it = batches.Begin(), end = batches.End(); it != end; ++it)
	{
		Batch2D* current = &*it;

		if (start && current->_type == GeometryType::STATIC && current->_geometry == start->_geometry)
		{
			if (start->_type == GeometryType::INSTANCED)
			{
				instanceTransforms.Push(*current->_worldMatrix);
				++start->_instanceCount;
			}
			else
			{
				// Begin new instanced batch
				start->_type = GeometryType::INSTANCED;
				size_t instanceStart = instanceTransforms.Size();
				instanceTransforms.Push(*start->_worldMatrix);
				instanceTransforms.Push(*current->_worldMatrix);
				start->_instanceStart = instanceStart; // Overwrites non-instance world matrix
				start->_instanceCount = 2; // Overwrites sort _key / distance
			}
		}
		else
			start = (current->_type == GeometryType::STATIC) ? current : nullptr;
	}
}



}