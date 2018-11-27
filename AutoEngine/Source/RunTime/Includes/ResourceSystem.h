#pragma once
#include "GameManager.h"

namespace Auto3D {
class Resource;
class tImage;
class Model;

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
	REGISTER_DERIVED_CLASS(ResourceSystem, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(ResourceSystem);
	using ImageQueue = HASH_MAP<PInt8, tImage*>;
	using ModelQueue = HASH_MAP<PInt8, Model*>;
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
	* @brief : According fail path get image
	* @return: tImage*
	*/
	tImage* ImageLoad(PInt8 path);
	/**
	* @brief : According fail path get model
	* @return: ModelCommand*
	*/
	Model* ModelLoad(PInt8 path);
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
	* @brief : Free image
	*/
	void FreeImage(tImage * image);


	/**
	* @brief : Return a resource by type and name. Load if not loaded yet. 
	*	Return null if not found or if fails, unless SetReturnFailedResources(true) 
	*	has been called. Can be called only from the main thread.
	*/
	Resource* GetResource(STRING type, const STRING& name, bool sendEventOnFailure = true);
	/**
	* @brief : Template version of returning a resource by name
	*/
	template <typename _Ty> _Ty* GetResource(const STRING& name, bool sendEventOnFailure = true);
	/**
	* @brief : Add resource path in catalogue array
	*/
	void AddResourcePath(const WSTRING& path);
	/**
	* @brief : Get resource paths
	*/
	VECTOR<WSTRING> GetResourcePaths();
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
private:
	/// resources by type.
	HASH_MAP<STRING, ResourceGroup> _resourceGroups;
	/// image hash map queue
	ImageQueue _imageQueue;
	/// modle hash map queue
	ModelQueue _modelQueue;
	/// resource directory paths
	VECTOR<WSTRING> resourcePaths;
};

template <typename _Ty> _Ty* ResourceSystem::GetResource(const STRING& name, bool sendEventOnFailure)
{
	STRING type = _Ty::GetClassStringStatic();
	return static_cast<_Ty*>(GetResource(type, name, sendEventOnFailure));
}

}

