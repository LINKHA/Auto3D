#pragma once

#include "../AutoConfig.h"

#ifndef WIN32
#include <pthread.h>
#endif

namespace Auto3D
{

#ifdef _WIN32
typedef unsigned TLSKeyID;
#else
typedef pthread_key_t TLSKeyID;
#endif

/// %Thread local storage value.
class AUTO_API ThreadLocalValue
{
public:
    /// Construct.
    ThreadLocalValue();
    
    /// Destruct.
    ~ThreadLocalValue();

    /// Set the value.
    void SetValue(void* value);

    /// Return the value.
    void* Value() const;
    
    /// Return whether was successfully allocated. Returns false when the OS resources for thread local values have been exhausted.
    bool GetValid() const { return _valid; }

private:
    /// Key used by the OS to identify the value.
    TLSKeyID _key;
    /// Valid flag.
    bool _valid;
};

}
