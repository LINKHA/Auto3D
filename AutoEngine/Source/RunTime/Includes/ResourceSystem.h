#pragma once
#include "GameManager.h"

namespace Auto3D {
class Image;
class Model;
class Resource;

class ResourceSystem : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(ResourceSystem, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(ResourceSystem);
	using ImageQueue = HASH_MAP<PInt8, Image*>;
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
	* @return: Image*
	*/
	Image* ImageLoad(PInt8 path);
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
	void FreeImage(Image * image);

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
private:
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

