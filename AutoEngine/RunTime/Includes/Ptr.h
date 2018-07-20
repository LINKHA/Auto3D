#pragma once
#include "Auto.h"
AUTO_BEGIN

template <class T> 
class SharedPtr
{
public:
	SharedPtr() noexcept 
		: ptr(nullptr)
	{
	}
	SharedPtr(const SharedPtr<T>& rhs) noexcept 
		: ptr(rhs.ptr)
	{
		AddRef();
	}
	virtual ~SharedPtr(){}

	SharedPtr * ptr;
};

AUTO_END