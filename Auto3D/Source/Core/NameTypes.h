#pragma once
#include "AutoConfig.h"
#include "Container/String.h"
#include "Container/StringHash.h"
#include "StringHashRegister.h"

namespace Auto3D
{
///Simple version of FString, this object is lossy compressed content using hash storage, case insensitive.
class AUTO_API GetName
{
public:
	/// Structure.
	GetName(const char* name)
	{
		SetName(name);
	}
	/// Structure.
	GetName(const FString& name)
	{
		SetName(name.CString());
	}

	/// Set name to value and add to string hash map.
	void SetName(const char* name)
	{
		_value = FStringHash(name);
		GStringHashRegister::Get().RegisterString(_value, name);
	}
	/// Equality operator.
	inline bool operator==(const char* other) const
	{
		return _value == FStringHash(other);
	}
	/// Equality operator.
	inline bool operator==(const GetName& other) const
	{
		return _value == other.Value();
	}
	/// Inequality operator.
	inline bool operator!=(const char* other) const
	{
		return !operator==(other);
	}
	/// Inequality operator.
	inline bool operator!=(const GetName& other) const
	{
		return !operator==(other);
	}
	/// Get string from stringhash map.
	/// Note that since the hash is lossy and case insensitive, it is all lowercase.
	const FString ToString() { return GStringHashRegister::Get().GetStringCopy(_value); }
	/// Return value.
	const FStringHash& Value()const { return _value; }
private:
	/// FString hash value.
	FStringHash _value;
};

}