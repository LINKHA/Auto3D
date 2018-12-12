#pragma once
#include "GameManager.h"


namespace Auto3D {
class Resource;
class File;
class Image;

static const unsigned PRIORITY_LAST = 0xffffffff;

/// Container of resources with specific type.
struct ResourceGroup
{
	/**
	* @brief : Construct with defaults
	*/
	ResourceGroup() :
		memoryBudget(0),
		memoryUse(0)
	{}

	/// Memory budget.
	unsigned long long memoryBudget;
	/// Current memory use.
	unsigned long long memoryUse;
	/// Resources.
	HASH_MAP<STRING, SharedPtr<Resource> > resources;
};

class ResourceSystem : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(ResourceSystem, GlobalGameManager)
public:
	explicit ResourceSystem(Ambient* ambient);
	/**
	* @brief : Resource Init
	*/
	void Init();
	/**
	* @brief : According fail path get texture
	* @return : unsigned int
	*/
	unsigned int TextureLoad(PInt8 path,bool vertically = true);
	/**
	* @brief : According fail path get cubemap
	* @return: unsigned int
	*/
	unsigned int CubemapLoad(VECTOR<STRING> faces);
	/**
	* @brief : According fail path get HDR cube
	* @return: unsigned int
	*/
	unsigned int HdrLoad(PInt8 path);

	/**
	* @brief : Return a resource by type and name. Load if not loaded yet. 
	*	Return null if not found or if fails, unless SetReturnFailedResources(true) 
	*	has been called. Can be called only from the main thread.
	*/
	Resource* GetResource(STRING type, const STRING& name, bool sendEventOnFailure = true);
	/**
	* @brief : Template version of returning a resource by name
	*/
	template<typename _Ty> _Ty* GetResource(const STRING& name, bool sendEventOnFailure = true);
	/**
	* @brief : Add resource path in catalogue array
	*/
	void AddResourceDir(const STRING& path, unsigned priority = PRIORITY_LAST);
	/**
	* @brief : Get resource paths
	*/
	VECTOR<STRING> GetResourceDirs();
	/**
	* @brief : Return added resource load directories
	*/
	const VECTOR<STRING>& GetResourceDirs() const { return _resourceDirs; }
	/**
	* @brief :Open and return a file from the resource load paths or from inside a package file. 
	*	If not found, use a fallback search with absolute path. Return null if fails.
	*	Can be called from outside the main thread
	*/
	File* GetFile(const STRING& name, bool sendEventOnFailure = true);
	/**
	* @brief : Remove unsupported constructs from the resource name to prevent ambiguity,
	*	and normalize absolute filename to resource path relative if possible
	*/
	STRING SanitateResourceName(const STRING& name) const;
	/**
	* @brief : Return full absolute file name of resource if possible, or empty if not found
	*/
	STRING GetResourceFileName(const STRING& name) const;
	/**
	* @brief : Store a dependency for a resource. 
	*	If a dependency file changes, the resource will be reloaded
	*/
	void StoreResourceDependency(Resource* resource, const STRING& dependency);

	/**
	* @brief : Register scene library objects.
	*/
	void RegisterResourceLib(Ambient* ambient);
private:
	/**
	* @brief : Find a resource
	*/
	const SharedPtr<Resource>& findResource(STRING type, STRING name);
	/**
	* @brief : Find a resource by name only. Searches all type groups
	*/
	const SharedPtr<Resource>& findResource(STRING name);
	/**
	* @brief : Search FileSystem for file
	*/
	File* searchResourceDirs(const STRING& name);
private:
	/// resources by type.
	HASH_MAP<STRING, ResourceGroup> _resourceGroups{};
	/// resource directory paths
	VECTOR<STRING> _resourceDirs{};
};

template <typename _Ty> _Ty* ResourceSystem::GetResource(const STRING& name, bool sendEventOnFailure)
{
	STRING type = _Ty::GetClassStringStatic();
	return static_cast<_Ty*>(GetResource(type, name, sendEventOnFailure));
}

}

