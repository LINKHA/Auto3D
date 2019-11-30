#pragma once

#include "AutoConfig.h"

namespace Auto3D
{

/// Operating system mutual exclusion primitive.
class AUTO_API FMutex
{
public:
    /// Construct.
    FMutex();
    /// Destruct.
    ~FMutex();

    /// Acquire the _mutex. Block if already acquired.
    void Acquire();
    /// Release the _mutex.
    void Release();
    
private:
    /// FMutex _handle.
    void* _handle;
};

/// Lock that automatically acquires and releases a _mutex.
class AUTO_API FMutexLock
{
public:
    /// Construct and acquire the _mutex.
    FMutexLock(FMutex& mutex);
    /// Destruct. Release the _mutex.
    ~FMutexLock();

    /// Prevent copy construction.
	FMutexLock(const FMutexLock& rhs) = delete;
    /// Prevent assignment.
    FMutexLock& operator = (const FMutexLock& rhs) = delete;
    
    /// FMutex reference.
    FMutex& _mutex;
};

}
