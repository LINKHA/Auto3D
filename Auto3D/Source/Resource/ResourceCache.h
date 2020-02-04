#pragma once

#include "Container/Ptr.h"
#include "Container/AutoPtr.h"
#include "Container/Singleton.h"


namespace Auto3D
{

class OResource;
class FStream;

/// %OResource cache subsystem. Loads resources on demand and stores them for later access.
class AUTO_API GResourceModule
{
	REGISTER_SINGLETON(GResourceModule)
public:
    /// Construct and register subsystem.
    GResourceModule();
    /// Destruct. Destroy all owned resources and unregister subsystem.
    ~GResourceModule();

    /// Add a resource directory. Return true on success.
    bool AddResourceDir(const FString& pathName, bool addFirst = false);
    /// Add a manually created resource. If returns success, the resource cache takes ownership of it.
    bool AddManualResource(OResource* resource);
    /// Remove a resource directory.
    void RemoveResourceDir(const FString& pathName);
    /// Open a resource file stream from the resource directories. Return a pointer to the stream, or null if not found.
    FStream* OpenResource(const FString& name);
 
    /// Unload resource. Optionally force removal even if referenced.
    void UnloadResource(FString type, const FString& name, bool force = false);
    /// Unload all resources of type.
    void UnloadResources(FString type, bool force = false);
    /// Unload resources by type and partial name.
    void UnloadResources(FString type, const FString& partialName, bool force = false);
    /// Unload resources by partial name.
    void UnloadResources(const FString& partialName, bool force = false);
    /// Unload all resources.
    void UnloadAllResources(bool force = false);
    /// Reload an existing resource. Return true on success.
    bool ReloadResource(OResource* resource);

	/// Load and return a resource.
	OResource* LoadResource(FString type, const FString& name);
    /// Load and return a resource, template version.
    template <typename _Ty> _Ty* LoadResource(const FString& name) { return static_cast<_Ty*>(LoadResource(_Ty::GetTypeNameStatic(), name)); }
    /// Load and return a resource, template version.
    template <typename _Ty> _Ty* LoadResource(const char* name) { return static_cast<_Ty*>(LoadResource(_Ty::GetTypeNameStatic(), name)); }

    /// Return resources by type.
    void ResourcesByType(TVector<OResource*>& result, FString type) const;
    /// Return resource directories.
    const TVector<FString>& ResourceDirs() const { return _resourceDirs; }
    /// Return whether a file exists in the resource directories.
    bool Exists(const FString& name) const;
    /// Return an absolute filename from a resource name.
    FString ResourceFileName(const FString& name) const;

    /// Return resources by type, template version.
    template <typename _Ty> void ResourcesByType(TVector<_Ty*>& dest) const
    {
        TVector<OResource*>& resources = reinterpret_cast<TVector<OResource*>&>(dest);
        FStringHash type = _Ty::TypeStatic();
        ResourcesByType(resources, type);

        // Perform conversion of the returned pointers
        for (size_t i = 0; i < resources.Size(); ++i)
        {
            OResource* resource = resources[i];
            dest[i] = static_cast<_Ty*>(resource);
        }
    }

    /// Normalize and remove unsupported constructs from a resource name.
    FString SanitateResourceName(const FString& name) const;
    /// Normalize and remove unsupported constructs from a resource directory name.
    FString SanitateResourceDirName(const FString& name) const;

private:
	THashMap<TPair<FString, FString>, OResource*> _resources;
    TVector<FString> _resourceDirs;
};

}
