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

}