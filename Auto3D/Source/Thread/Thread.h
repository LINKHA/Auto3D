#pragma once

#include "AutoConfig.h"

#ifndef WIN32
#include <pthread.h>
#endif

namespace Auto3D
{

#ifndef WIN32
typedef pthread_t ThreadID;
#else
typedef unsigned ThreadID;
#endif

/// Operating system thread.
class AUTO_API FThread
{
public:
    /// Construct. Does not start the thread yet.
    FThread();
    /// Destruct. If running, stop and wait for thread to finish.
    virtual ~FThread();
    /// The function to run in the thread.
    virtual void ThreadFunction() = 0;
    /// Start running the thread. Return true on success, or false if already running or if can not create the thread.
    bool Run();
    /// Set the running flag to false and wait for the thread to finish.
    void Stop();
    /// Set thread priority. The thread must have been started first.
    void SetPriority(int priority);
    /// Return whether thread exists.
    bool IsStarted() const { return _handle != nullptr; }

    /// Sleep the current thread for the specified amount of milliseconds. 0 to just yield the timeslice.
    static void Sleep(unsigned mSec);
    /// Set the current thread as the main thread.
    static void SetMainThread();
    /// Return the current thread's ID.
    static ThreadID CurrentThreadID();
    /// Return whether is executing in the main thread.
    static bool IsMainThread();
    
protected:
	/// Running flag.
	volatile bool _shouldRun;
    /// FThread _handle.
    void* _handle;
    /// Main thread's thread ID.
    static ThreadID mainThreadID;
};

}
