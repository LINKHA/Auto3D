#include "WorkQueue.h"

namespace Auto3D {

WorkQueue::WorkQueue(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, _shutDown(false)
	, _pausing(false)
	, _paused(false)
	, _completing(false)
	, _tolerance(10)
	, _lastSize(0)
	, _maxNonThreadedWorkMs(5)
{

}

WorkQueue::~WorkQueue()
{

}

SharedPtr<WorkItem> WorkQueue::GetFreeItem()
{
	if (_poolItems.size() > 0)
	{
		SharedPtr<WorkItem> item = _poolItems.front();
		_poolItems.pop_front();
		return item;
	}
	else
	{
		// No usable items found, create a new one set it as pooled and return it.
		SharedPtr<WorkItem> item(new WorkItem());
		item->_pooled = true;
		return item;
	}
}

}