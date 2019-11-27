
#pragma once

#include "Container/HashMap.h"
#include "Container/Ptr.h"
#include "Container/StringHash.h"
#include "Container/Singleton.h"

namespace Auto3D
{

class Mutex;

template <typename _Ty1, typename _Ty2> class HashMap;
/// Map of strings.
using StringMap = HashMap<StringHash, String>;

/// Helper class used for StringHash reversing.
class AUTO_API StringHashRegister
{
	friend class Singleton<StringHashRegister>;
private:
	/// Construct. threadSafe controls whether the RegisterString and GetStringCopy are thread-safe.
	explicit StringHashRegister(bool threadSafe = true);
	/// Destruct.
	~StringHashRegister();
public:
	/// Gets the singleton instance of the module manager.
	static StringHashRegister& Get();

	/// Register string for hash reverse mapping. Could be used from StringHash ctor.
	StringHash RegisterString(const StringHash& hash, const char* string);
	/// Register string for hash reverse mapping.
	StringHash RegisterString(const char* string);
	/// Register string for hash reverse mapping. 
	bool RemoveString(const StringHash& hash);
	/// Register string for hash reverse mapping. 
	bool RemoveString(const char* string);

	/// Return string for given StringHash. Return empty string if not found.
	String GetStringCopy(const StringHash& hash) const;
	/// Return whether the string in contained in the register.
	bool Contains(const StringHash& hash) const;

	/// Return String for given StringHash. Return value is unsafe to use if RegisterString is called from other threads.
	const String& GetString(const StringHash& hash) const;
	/// Return map of hashes. Return value is unsafe to use if RegisterString is called from other threads.
	const StringMap& GetInternalMap() const { return _map; }

private:
	/// Hash to string map.
	StringMap _map;
	/// Mutex.
	UniquePtr<Mutex> _mutex;
};

}
