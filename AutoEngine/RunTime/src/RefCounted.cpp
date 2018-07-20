#include "RefCounted.h"
AUTO_BEGIN
RefCounted::RefCounted() :
	_refCount(new RefCount())
{
	(_refCount->weakRefs)++;
}

RefCounted::~RefCounted()
{
	assert(_refCount);
	assert(_refCount->refs == 0);
	assert(_refCount->weakRefs > 0);

	_refCount->refs = -1;
	(_refCount->weakRefs)--;
	if (!_refCount->weakRefs)
		delete _refCount;

	_refCount = nullptr;
}

void RefCounted::AddRef()
{
	assert(_refCount->refs >= 0);
	(_refCount->refs)++;
}

void RefCounted::ReleaseRef()
{
	assert(_refCount->refs > 0);
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
AUTO_END