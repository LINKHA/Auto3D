#include "UIBatch.h"
#include "../Graphics/Texture.h"

namespace Auto3D
{
		
inline bool CompareBatchState(UIBatch& lhs, UIBatch& rhs)
{
	return lhs._sortKey < rhs._sortKey;
}

void UIBatchQueue::Clear()
{
	_batches.Clear();
}

void UIBatchQueue::Sort(Vector<Matrix3x4F>& instanceTransforms)
{
	Auto3D::Sort(_batches.Begin(), _batches.End(), CompareBatchState);
	// Build instances where adjacent batches have same state
	BuildInstances(_batches, instanceTransforms);
}

void UIBatchQueue::BuildInstances(Vector<UIBatch>& batches, Vector<Matrix3x4F>& instanceTransforms)
{
	UIBatch* start = nullptr;

	for (auto it = batches.Begin(), end = batches.End(); it != end; ++it)
	{
		UIBatch* current = &*it;

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