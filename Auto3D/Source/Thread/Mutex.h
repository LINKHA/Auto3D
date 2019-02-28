#pragma once

#include "../AutoConfig.h"

namespace Auto3D
{

/// Operating system mutual exclusion primitive.
class AUTO_API Mutex
{
public:
    /// Construct.
    Mutex();
    /// Destruct.
    ~Mutex();
    
    /// Acquire the _mutex. Block if already acquired.
    void Acquire();
    /// Release the _mutex.
    void Release();
    
private:
    /// Mutex _handle.
    void* _handle;
};

/// Lock that automatically acquires and releases a _mutex.
class AUTO_API MutexLock
{
public:
    /// Construct and acquire the _mutex.
    MutexLock(Mutex& mutex);
    /// Destruct. Release the _mutex.
    ~MutexLock();

    /// Prevent copy construction.
	MutexLock(const MutexLock& rhs) = delete;
    /// Prevent assignment.
    MutexLock& operator = (const MutexLock& rhs) = delete;
    
    /// Mutex reference.
    Mutex& _mutex;
};

}
