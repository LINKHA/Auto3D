#pragma once

#include "../Base/AutoPtr.h"
#include "../Base/String.h"
#include "../Math/Math.h"
#include "../Object/GameManager.h"
#include "../Time/Time.h"

namespace Auto3D
{

/// Profiling data for one block in the profiling tree.
class AUTO_API ProfilerBlock
{
public:
    /// Construct-
    ProfilerBlock(ProfilerBlock* parent, const char* name);
    /// Destruct.
    ~ProfilerBlock();

    /// Start time measurement and increment call count.
    void Begin();
    /// End time measurement.
    void End();
    /// Process stats at the end of frame.
    void EndFrame();
    /// Begin an interval lasting several frames.
    void BeginInterval();
    /// Return a child block; create if necessary.
    ProfilerBlock* FindOrCreateChild(const char* name);

    /// Block name.
    const char* _name;
    /// Hires timer for time measurement.
    HiresTimer _timer;
    /// Parent block.
    ProfilerBlock* _parent;
    /// Child blocks.
    Vector<AutoPtr<ProfilerBlock > > _children;
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
class AUTO_API Profiler : public Subsystem
{
	REGISTER_OBJECT_CLASS(Profiler, Subsystem)

public:
    /// Construct.
    Profiler();
    /// Destruct.
    ~Profiler();

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
    String OutputResults(bool showUnused = false, bool showTotal = false, size_t maxDepth = M_MAX_UNSIGNED) const;
    /// Return the current profiling block.
    const ProfilerBlock* CurrentBlock() const { return _current; }
    /// Return the root profiling block.
    const ProfilerBlock* RootBlock() const { return _root; }

private:
    /// Output results recursively.
    void OutputResults(ProfilerBlock* block, String& output, size_t depth, size_t maxDepth, bool showUnused, bool showTotal) const;

    /// Current profiling block.
    ProfilerBlock* _current;
    /// Root profiling block.
    AutoPtr<ProfilerBlock> _root;
    /// Frames in the current interval.
    size_t _intervalFrames;
    /// Total frames since start.
    size_t _totalFrames;
};

/// Helper class for automatically beginning and ending a profiling block
class AUTO_API AutoProfileBlock
{
public:
    /// Construct and begin a profiling block. The name must be persistent; string literals are recommended.
    AutoProfileBlock(const char* name)
    {
        _profiler = Object::GetSubsystem<Profiler>();
        if (_profiler)
            _profiler->BeginBlock(name);
    }

    /// Destruct. End the profiling block.
    ~AutoProfileBlock()
    {
        if (_profiler)
            _profiler->EndBlock();
    }

private:
    /// Profiler subsystem.
    Profiler* _profiler;
};

#ifdef AUTO_PROFILING
#define PROFILE(name) AutoProfileBlock profile_ ## name (#name)
#else
#define PROFILE(_name)
#endif

}
