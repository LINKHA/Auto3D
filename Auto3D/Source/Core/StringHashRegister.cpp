#include "StringHashRegister.h"
#include "Thread/Mutex.h"
#include "Debug/Log.h"

#include <cstdio>

#include "Debug/DebugNew.h"

namespace Auto3D
{

FStringHashRegister::FStringHashRegister(bool threadSafe)
{
	if (threadSafe)
		_mutex = MakeUnique<Mutex>();
}


FStringHashRegister::~FStringHashRegister()       // NOLINT(hicpp-use-equals-default, modernize-use-equals-default)
{
	// Keep destructor here to let mutex_ destruct
}

FStringHashRegister& FStringHashRegister::Get()
{
	return TSingleton<FStringHashRegister>::Instance();
}

FStringHash FStringHashRegister::RegisterString(const FStringHash& hash, const char* string)
{
	if (_mutex)
		_mutex->Acquire();

	auto iter = _map.Find(hash);
	if (iter == _map.End())
	{
		_map[hash] = string;
	}
	else if (iter->_second.Compare(string, false) != 0)
	{
		WarningStringF("StringHash collision detected! Both \"%s\" and \"%s\" have hash #%s",
			string, iter->_second.CString(), hash.ToString().CString());
	}

	if (_mutex)
		_mutex->Release();

	return hash;
}

FStringHash FStringHashRegister::RegisterString(const char* string)
{
	FStringHash hash(string);
	return RegisterString(hash, string);
}

bool FStringHashRegister::RemoveString(const FStringHash& hash)
{
	if (_mutex)
		_mutex->Acquire();

	bool flag = false;
	auto iter = _map.Find(hash);
	if (iter == _map.End())
	{
		WarningString("StringHash remove failed. This hash value does not exist!");
		flag |= false;
	}
	else 
	{
		_map.Erase(hash);
		flag |= true;
	}

	if (_mutex)
		_mutex->Release();

	return flag;
}

bool FStringHashRegister::RemoveString(const char* string)
{
	FStringHash hash(string);
	return RemoveString(hash);
}

FString FStringHashRegister::GetStringCopy(const FStringHash& hash) const
{
	if (_mutex)
		_mutex->Acquire();

	const FString copy = GetString(hash);

	if (_mutex)
		_mutex->Release();

	return copy;
}

bool FStringHashRegister::Contains(const FStringHash& hash) const
{
	if (_mutex)
		_mutex->Acquire();

	const bool contains = _map.Contains(hash);

	if (_mutex)
		_mutex->Release();

	return contains;
}

const FString& FStringHashRegister::GetString(const FStringHash& hash) const
{
	auto iter = _map.Find(hash);
	return iter == _map.End() ? FString::EMPTY : iter->_second;
}

}
