#pragma once

#include "IO/ResourceRef.h"
#include "Core/Object.h"

namespace Auto3D
{

class Stream;

/// Base class for resources.
class AUTO_API AResource : public AObject
{
    REGISTER_OBJECT_CLASS(AResource,AObject)

public:
    /// Load the resource data from a stream. May be executed outside the main thread, should not access GPU resources. Return true on success.
    virtual bool BeginLoad(Stream& source);
    /// Finish resource loading if necessary. Always called from the main thread, so GPU resources can be accessed here. Return true on success.
    virtual bool EndLoad();
    /// Save the resource to a stream. Return true on success.
    virtual bool Save(Stream& dest);

    /// Load the resource synchronously from a binary stream. Return true on success.
    bool Load(Stream& source);
    /// Set name of the resource, usually the same as the file being loaded from.
    void SetName(const FString& newName);

	void SetMemoryUse(unsigned _size);

    /// Return name of the resource.
    const FString& Name() const { return _name; }
    /// Return name hash of the resource.
    const FStringHash& NameHash() const { return _nameHash; }
	/// Return memory use in bytes, possibly approximate.
	unsigned GetMemoryUse() const { return _memoryUse; }
private:
    /// AResource name.
    FString _name;
    /// AResource name hash.
    FStringHash _nameHash;

	/// Memory use in bytes.
	unsigned _memoryUse;
};

/// Return name from a resource pointer.
inline const FString& ResourceName(AResource* resource)
{
    return resource ? resource->Name() : FString::EMPTY;
}

/// Return type from a resource pointer, or default type if null.
inline FStringHash ResourceType(AResource* resource, FStringHash defaultType)
{
    return resource ? resource->GetType() : defaultType;
}

/// Make a resource ref from a resource pointer.
inline ResourceRef MakeResourceRef(AResource* resource, FStringHash defaultType)
{
    return ResourceRef(ResourceType(resource, defaultType), ResourceName(resource));
}

/// Return resource names from a vector of resource pointers.
template <typename _Ty> TVector<FString> ResourceNames(const TVector<_Ty*>& resources)
{
    TVector<FString> ret(resources.Size());
    for (size_t i = 0; i < resources.Size(); ++i)
        ret[i] = ResourceName(resources[i]);

    return ret;
}

/// Make a resource ref list from a vector of resource poitners.
template <typename _Ty> ResourceRefList MakeResourceRefList(const TVector<_Ty*>& resources)
{
    return ResourceRefList(_Ty::TypeStatic(), GetResourceNames(resources));
}

}
