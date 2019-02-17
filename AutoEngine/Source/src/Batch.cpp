#include "Batch.h"

namespace Auto3D {
inline bool CompareBatchesFrontToBack(SharedPtr<Batch> lhs, SharedPtr<Batch> rhs)
{
	if (lhs->_renderOrder != rhs->_renderOrder)
		return lhs->_renderOrder < rhs->_renderOrder;
	else if (lhs->_distance != rhs->_distance)
		return lhs->_distance < rhs->_distance;
	else
		return lhs->_sortKey < rhs->_sortKey;
}

inline bool CompareBatchesBackToFront(SharedPtr<Batch> lhs, SharedPtr<Batch> rhs)
{
	if (lhs->_renderOrder != rhs->_renderOrder)
		return lhs->_renderOrder < rhs->_renderOrder;
	else if (lhs->_distance != rhs->_distance)
		return lhs->_distance > rhs->_distance;
	else
		return lhs->_sortKey < rhs->_sortKey;
}

inline bool CompareBatchGroupOrder(SharedPtr<BatchGroup> lhs, SharedPtr<BatchGroup> rhs)
{
	return lhs->_renderOrder < rhs->_renderOrder;
}

void BatchQueue::SortBackToFront()
{
	_sortedBatches.resize(_batches.size());

	for (unsigned i = 0; i < _batches.size(); ++i)
		_sortedBatches[i] = _batches[i];

	std::sort(_sortedBatches.begin(), _sortedBatches.end(), CompareBatchesBackToFront);

	_sortedBatchGroups.resize(_batchGroups.size());

	unsigned index = 0;
	for (PAIR_MAP<BatchGroupKey, SharedPtr<BatchGroup> >::iterator i = _batchGroups.begin(); i != _batchGroups.end(); ++i)
		_sortedBatchGroups[index++] = i->second;

	std::sort(_sortedBatchGroups.begin(), _sortedBatchGroups.end(), CompareBatchGroupOrder);
}
void BatchQueue::SortFrontToBack()
{

}

}