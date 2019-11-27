#include "StringHashRegister.h"
#include "Thread/Mutex.h"
#include "Debug/Log.h"

#include <cstdio>

#include "Debug/DebugNew.h"

namespace Auto3D
{

StringHashRegister::StringHashRegister(bool threadSafe)
{
	if (threadSafe)
		_mutex = MakeUnique<Mutex>();
}


StringHashRegister::~StringHashRegister()       // NOLINT(hicpp-use-equals-default, modernize-use-equals-default)
{
	// Keep destructor here to let mutex_ destruct
}

StringHashRegister& StringHashRegister::Get()
{
	return Singleton<StringHashRegister>::Instance();
}

StringHash StringHashRegister::RegisterString(const StringHash& hash, const char* string)
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

StringHash StringHashRegister::RegisterString(const char* string)
{
	StringHash hash(string);
	return RegisterString(hash, string);
}

bool StringHashRegister::RemoveString(const StringHash& hash)
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

bool StringHashRegister::RemoveString(const char* string)
{
	StringHash hash(string);
	return RemoveString(hash);
}

String StringHashRegister::GetStringCopy(const StringHash& hash) const
{
	if (_mutex)
		_mutex->Acquire();

	const String copy = GetString(hash);

	if (_mutex)
		_mutex->Release();

	return copy;
}

bool StringHashRegister::Contains(const StringHash& hash) const
{
	if (_mutex)
		_mutex->Acquire();

	const bool contains = _map.Contains(hash);

	if (_mutex)
		_mutex->Release();

	return contains;
}

const String& StringHashRegister::GetString(const StringHash& hash) const
{
	auto iter = _map.Find(hash);
	return iter == _map.End() ? String::EMPTY : iter->_second;
}

}
