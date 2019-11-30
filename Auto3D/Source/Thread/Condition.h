#pragma once

#include "AutoConfig.h"

namespace Auto3D
{

/// %FCondition on which a thread can wait.
class AUTO_API FCondition
{
public:
    /// Construct.
    FCondition();
    
    /// Destruct.
    ~FCondition();
    
    /// Set the condition. Will be automatically reset once a waiting thread wakes up.
    void Set();
    
    /// Wait on the condition.
    void Wait();
    
private:
    #ifndef WIN32
    /// FMutex for the _event, necessary for pthreads-based implementation.
    void* _mutex;
    #endif
    /// Operating system specific _event.
    void* _event;
};

}
