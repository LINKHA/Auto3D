#include "Renderer/Batch.h"
#include "Container/Sort.h"

namespace Auto3D
{
inline bool CompareBatchState(FBatch& lhs, FBatch& rhs)
{
	return lhs._pass._sortKey < rhs._pass._sortKey;
}

inline bool CompareBatchDistanceFrontToBack(FBatch& lhs, FBatch& rhs)
{
	return lhs._distance < rhs._distance;
}

inline bool CompareBatchDistanceBackToFront(FBatch& lhs, FBatch& rhs)
{
	return lhs._distance > rhs._distance;
}

FRenderQueue::FRenderQueue():
	_sort(ERenderCommandSortMode::STATE)
{
}

void FRenderQueue::Clear()
{
	_batches.Clear();
}

void FRenderQueue::Sort()
{
	switch (_sort)
	{
	case ERenderCommandSortMode::STATE:
		Auto3D::Sort(_batches.Begin(), _batches.End(), CompareBatchState);
		break;

	case ERenderCommandSortMode::FRONT_TO_BACK:
		Auto3D::Sort(_batches.Begin(), _batches.End(), CompareBatchDistanceFrontToBack);
		break;

	case ERenderCommandSortMode::BACK_TO_FRONT:
		Auto3D::Sort(_batches.Begin(), _batches.End(), CompareBatchDistanceBackToFront);
		break;

	default:
		break;
	}

	// Build instances where adjacent batches have same state
	BuildInstances(_batches);
}

void FRenderQueue::BuildInstances(TVector<FBatch>& batches)
{
	static int instanceCounter;
	instanceCounter = 0;

	FBatch* start = nullptr;

	for (auto it = batches.Begin(), end = batches.End(); it != end; ++it)
	{
		FBatch* current = &*it;

		if (start && current->_type == EGeometryType::STATIC && current->_pass == start->_pass)
		{
			if (start->_type == EGeometryType::INSTANCED)
			{
				++start->_instanceCount;
				++instanceCounter;
			}
			else
			{
				--instanceCounter;
				// Begin new instanced batch
				start->_type = EGeometryType::INSTANCED;
				size_t instanceStart = instanceCounter;
				start->_instanceStart = instanceStart; // Overwrites non-instance world matrix
				start->_instanceCount = 2; // Overwrites sort _key / distance
				instanceCounter += 2;
			}
		}
		else
		{
			start = (current->_type == EGeometryType::STATIC) ? current : nullptr;
			++instanceCounter;
		}
			
	}
}

}