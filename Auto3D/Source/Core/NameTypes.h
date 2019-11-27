#pragma once
#include "AutoConfig.h"
#include "Container/String.h"
#include "Container/StringHash.h"
#include "StringHashRegister.h"

namespace Auto3D
{
///Simple version of String, this object is lossy compressed content using hash storage, case insensitive.
class AUTO_API Name
{
	/// Structure.
	Name(const char* name)
	{
		SetName(name);
	}
	/// Structure.
	Name(const String& name)
	{
		SetName(name.CString());
	}

	/// Set name to value and add to string hash map.
	void SetName(const char* name)
	{
		_value = StringHash(name);
		StringHashRegister::Get().RegisterString(_value, name);
	}
	/// Equality operator.
	inline bool operator==(const char* other) const
	{
		return _value == StringHash(other);
	}
	/// Equality operator.
	inline bool operator==(const Name& other) const
	{
		return _value == other.Value();
	}
	/// Inequality operator.
	inline bool operator!=(const char* other) const
	{
		return !operator==(other);
	}
	/// Inequality operator.
	inline bool operator!=(const Name& other) const
	{
		return !operator==(other);
	}
	/// Get string from stringhash map.
	/// Note that since the hash is lossy and case insensitive, it is all lowercase.
	const String ToString() { return StringHashRegister::Get().GetStringCopy(_value); }
	/// Return value.
	const StringHash& Value()const { return _value; }
private:
	/// String hash value.
	StringHash _value;
};

}