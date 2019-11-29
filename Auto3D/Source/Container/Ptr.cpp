#include "Ptr.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

FRefCounted::FRefCounted() :
    _refCount(nullptr)
{
}

FRefCounted::~FRefCounted()
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

void FRefCounted::AddRef()
{
    if (!_refCount)
        _refCount = new FRefCount();

    ++(_refCount->_refs);
}

void FRefCounted::ReleaseRef()
{
    assert(_refCount && _refCount->_refs > 0);
    --(_refCount->_refs);
    if (_refCount->_refs == 0)
        delete this;
}

FRefCount* FRefCounted::RefCountPtr()
{
    if (!_refCount)
        _refCount = new FRefCount();

    return _refCount;
}

}
