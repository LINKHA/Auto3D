#include "RefCounted.h"
#include "LogAssert.h"
#include "DebugNew.h"

namespace Auto3D {
RefCounted::RefCounted() :
	_refCount(new RefCount())
{
	(_refCount->weakRefs)++;
}

RefCounted::~RefCounted()
{
	Assert(_refCount);
	//Assert(_refCount->refs == 0);
	WarningIfString(_refCount->refs == 0, "ref count is not zero!");
	Assert(_refCount->weakRefs > 0);

	_refCount->refs = -1;
	(_refCount->weakRefs)--;
	if (!_refCount->weakRefs)
		delete _refCount;

	_refCount = nullptr;
}

void RefCounted::AddRef()
{
	Assert(_refCount->refs >= 0);
	(_refCount->refs)++;
}

void RefCounted::ReleaseRef()
{
	Assert(_refCount->refs > 0);
	(_refCount->refs)--;
	if (!_refCount->refs)
		delete this;
}

int RefCounted::Refs() const
{
	return _refCount->refs;
}

int RefCounted::WeakRefs() const
{
	return _refCount->weakRefs - 1;
}
}