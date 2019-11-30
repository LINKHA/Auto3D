#pragma once

#include "Container/AutoPtr.h"
#include "Container/String.h"
#include "Math/MathDef.h"
#include "Core/GameManager.h"
#include "Time/Time.h"
#include "Core/Modules/ModuleManager.h"

namespace Auto3D
{

/// Profiling data for one block in the profiling tree.
class AUTO_API FProfilerBlock
{
public:
	/// Construct.
	FProfilerBlock(FProfilerBlock* parent, const char* name);
	/// Destruct.
	~FProfilerBlock();

	/// Start time measurement and increment call count.
	void Begin();
	/// End time measurement.
	void End();
	/// Process stats at the end of frame.
	void EndFrame();
	/// Begin an interval lasting several frames.
	void BeginInterval();
	/// Return a child block; create if necessary.
	FProfilerBlock* FindOrCreateChild(const char* name);

	/// Block name.
	const char* _name;
	/// Hires timer for time measurement.
	FHiresTimer _timer;
	/// Parent block.
	FProfilerBlock* _parent;
	/// Child blocks.
	TVector<TAutoPtr<FProfilerBlock > > _children;
	/// Current frame's accumulated time.
	long long _time;
	/// Current frame's longest call.
	long long _maxTime;
	/// Current frame's call count.
	unsigned _count;
	/// Previous frame's accumulated time.
	long long _frameTime;
	/// Previous frame's longest call.
	long long _frameMaxTime;
	/// Previous frame's call count.
	unsigned _frameCount;
	/// Current interval's accumulated time.
	long long _intervalTime;
	/// Current interval's longest call.
	long long _intervalMaxTime;
	/// Current interval's call count.
	unsigned _intervalCount;
	/// Accumulated time since start.
	long long _totalTime;
	/// Longest call since start.
	long long _totalMaxTime;
	/// Call count since start.
	unsigned _totalCount;
};

/// Hierarchical performance profiler subsystem.
class AUTO_API AProfiler : public ABaseModule
{
	REGISTER_OBJECT_CLASS(AProfiler, ABaseModule)

public:
	/// Construct.
	AProfiler();
	/// Destruct.
	~AProfiler();

	/// Begin a profiling block. The name must be persistent; string literals are recommended.
	void BeginBlock(const char* name);
	/// End the current profiling block.
	void EndBlock();
	/// Begin the next profiling frame.
	void BeginFrame();
	/// End the current profiling frame.
	void EndFrame();
	/// Begin a profiler interval.
	void BeginInterval();

	/// Output results into a string.
	FString OutputResults(bool showUnused = false, bool showTotal = false, size_t maxDepth = M_MAX_UNSIGNED) const;
	/// Return the current profiling block.
	const FProfilerBlock* CurrentBlock() const { return _current; }
	/// Return the root profiling block.
	const FProfilerBlock* RootBlock() const { return _root; }

private:
	/// Output results recursively.
	void OutputResults(FProfilerBlock* block, FString& output, size_t depth, size_t maxDepth, bool showUnused, bool showTotal) const;

	/// Current profiling block.
	FProfilerBlock* _current;
	/// Root profiling block.
	TAutoPtr<FProfilerBlock> _root;
	/// Frames in the current interval.
	size_t _intervalFrames;
	/// Total frames since start.
	size_t _totalFrames;
};

/// Helper class for automatically beginning and ending a profiling block
class AUTO_API FAutoProfileBlock
{
public:
	/// Construct and begin a profiling block. The name must be persistent; string literals are recommended.
	FAutoProfileBlock(const char* name)
	{
		_profiler = GModuleManager::Get().ProfilerModule();
		if (_profiler)
			_profiler->BeginBlock(name);
	}

	/// Destruct. End the profiling block.
	~FAutoProfileBlock()
	{
		if (_profiler)
			_profiler->EndBlock();
	}

private:
	/// AProfiler subsystem.
	AProfiler* _profiler;
};

#ifdef AUTO_PROFILING
#define PROFILE(name) AutoProfileBlock profile_ ## name (#name)
#else
#define PROFILE(_name)
#endif

}
