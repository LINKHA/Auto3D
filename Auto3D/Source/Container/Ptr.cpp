#include "Ptr.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

ARefCounted::ARefCounted() :
    _refCount(nullptr)
{
}

ARefCounted::~ARefCounted()
{
    if (_refCount)
    {
        assert(_refCount->_refs == 0);
        if (_refCount->_weakRefs == 0)
            delete _refCount;
        else
            _refCount->_expired = true;
    }
}

void ARefCounted::AddRef()
{
    if (!_refCount)
        _refCount = new RefCount();

    ++(_refCount->_refs);
}

void ARefCounted::ReleaseRef()
{
    assert(_refCount && _refCount->_refs > 0);
    --(_refCount->_refs);
    if (_refCount->_refs == 0)
        delete this;
}

RefCount* ARefCounted::RefCountPtr()
{
    if (!_refCount)
        _refCount = new RefCount();

    return _refCount;
}

}
