#pragma once

#include "../Container/Ptr.h"
#include "../AutoConfig.h"

namespace Auto3D
{

class Graphics;

/// Base class for objects that allocate GPU resources.
class AUTO_API GPUObject
{
public:
    /// Construct. Acquire the %Graphics subsystem if available and register self.
    GPUObject();
    /// Destruct. Unregister from the %Graphics subsystem.
    virtual ~GPUObject();
    
    /// Release the GPU resource.
    virtual void Release();
    /// Recreate the GPU resource after data loss. Not called on all rendering API's.
    virtual void Recreate();
    /// Return whether the contents have been lost due to graphics context having been destroyed.
    virtual bool IsDataLost() const { return _dataLost; }
    
    /// Set data lost state. Not needed on all rendering API's.
    void SetDataLost(bool enable) { _dataLost = enable; }

protected:
    /// %Graphics subsystem pointer.
    WeakPtr<Graphics> _graphics;

private:
    /// Data lost flag.
    bool _dataLost;
};

}
