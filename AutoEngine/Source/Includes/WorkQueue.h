#pragma once
#include "GameManager.h"

namespace Auto3D {


/// Work queue item.
struct WorkItem 
{
	friend class WorkQueue;

public:
	/// Work function. Called with the work item and thread index (0 = main thread) as parameters.
	void(*workFunction_)(const WorkItem*, unsigned) {};
	/// Data start pointer.
	void* _start{};
	/// Data end pointer.
	void* _end{};
	/// Auxiliary data pointer.
	void* _aux{};
	/// Priority. Higher value = will be completed first.
	unsigned _priority{};
	/// Whether to send event on completion.
	bool s_endEvent{};
	/// Completed flag.
	volatile bool _completed{};

private:
	bool _pooled{};
};

class WorkQueue : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(WorkQueue, GlobalGameManager)
public:
	explicit WorkQueue(SharedPtr<Ambient> ambient);

private:
	/// Work item pool for reuse to cut down on allocation. The bool is a flag for item pooling and whether it is available or not.
	LIST<SharedPtr<WorkItem> > _poolItems;
	/// Work item collection. Accessed only by the main thread.
	LIST<SharedPtr<WorkItem> > _workItems;
	/// Work item prioritized queue for worker threads. Pointers are guaranteed to be valid (point to workItems.)
	LIST<WorkItem*> _queue;

	/// Shutting down flag.
	volatile bool _shutDown;
	/// Pausing flag. Indicates the worker threads should not contend for the queue mutex.
	volatile bool _pausing;
	/// Paused flag. Indicates the queue mutex being locked to prevent worker threads using up CPU time.
	bool _paused;
	/// Completing work in the main thread flag.
	bool _completing;
	/// Tolerance for the shared pool before it begins to deallocate.
	int _tolerance;
	/// Last size of the shared pool.
	unsigned _lastSize;
	/// Maximum milliseconds per frame to spend on low-priority work, when there are no worker threads.
	int _maxNonThreadedWorkMs;
};

}