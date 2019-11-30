#pragma once

#include "Core/GameManager.h"

namespace Auto3D
{

class AResource;
class FStream;

typedef THashMap<TPair<FStringHash, FStringHash>, TSharedPtr<AResource> > ResourceMap;
 
/// %AResource cache subsystem. Loads resources on demand and stores them for later access.
class AUTO_API ResourceCache : public ABaseModule
{
    REGISTER_OBJECT_CLASS(ResourceCache, ABaseModule)
public:
    /// Construct and register subsystem.
    ResourceCache();
    /// Destruct. Destroy all owned resources and unregister subsystem.
    ~ResourceCache();

    /// Add a resource directory. Return true on success.
    bool AddResourceDir(const FString& pathName, bool addFirst = false);
    /// Add a manually created resource. If returns success, the resource cache takes ownership of it.
    bool AddManualResource(AResource* resource);
    /// Remove a resource directory.
    void RemoveResourceDir(const FString& pathName);
    /// Open a resource file stream from the resource directories. Return a pointer to the stream, or null if not found.
    TAutoPtr<FStream> OpenResource(const FString& name);
    /// Load and return a resource.
    AResource* LoadResource(FStringHash type, const FString& name);
    /// Unload resource. Optionally force removal even if referenced.
    void UnloadResource(FStringHash type, const FString& name, bool force = false);
    /// Unload all resources of type.
    void UnloadResources(FStringHash type, bool force = false);
    /// Unload resources by type and partial name.
    void UnloadResources(FStringHash type, const FString& partialName, bool force = false);
    /// Unload resources by partial name.
    void UnloadResources(const FString& partialName, bool force = false);
    /// Unload all resources.
    void UnloadAllResources(bool force = false);
    /// Reload an existing resource. Return true on success.
    bool ReloadResource(AResource* resource);
    /// Load and return a resource, template version.
    template <typename _Ty> _Ty* LoadResource(const FString& name) { return static_cast<_Ty*>(LoadResource(_Ty::GetTypeStatic(), name)); }
    /// Load and return a resource, template version.
    template <typename _Ty> _Ty* LoadResource(const char* name) { return static_cast<_Ty*>(LoadResource(_Ty::GetTypeStatic(), name)); }

    /// Return resources by type.
    void ResourcesByType(TVector<AResource*>& result, FStringHash type) const;
    /// Return resource directories.
    const TVector<FString>& ResourceDirs() const { return _resourceDirs; }
    /// Return whether a file exists in the resource directories.
    bool Exists(const FString& name) const;
    /// Return an absolute filename from a resource name.
    FString ResourceFileName(const FString& name) const;

    /// Return resources by type, template version.
    template <typename _Ty> void ResourcesByType(TVector<_Ty*>& dest) const
    {
        TVector<AResource*>& resources = reinterpret_cast<TVector<AResource*>&>(dest);
        FStringHash type = _Ty::TypeStatic();
        ResourcesByType(resources, type);

        // Perform conversion of the returned pointers
        for (size_t i = 0; i < resources.Size(); ++i)
        {
            AResource* resource = resources[i];
            dest[i] = static_cast<_Ty*>(resource);
        }
    }

    /// Normalize and remove unsupported constructs from a resource name.
    FString SanitateResourceName(const FString& name) const;
    /// Normalize and remove unsupported constructs from a resource directory name.
    FString SanitateResourceDirName(const FString& name) const;

private:
    ResourceMap _resources;
    TVector<FString> _resourceDirs;
};

/// Register AResource related object factories and attributes.
AUTO_API void RegisterResourceLibrary();

}
