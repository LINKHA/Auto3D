#pragma once
#include "GameManager.h"
#include "stl_use.h"

namespace Auto3D {
class Image;
class Model;
class Resource : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(Resource, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Resource);
	using ImageQueue = HASH_MAP(PInt8, Image*);
	using ModelQueue = HASH_MAP(PInt8, Model*);
public:
	explicit Resource(Ambient* ambient);
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
	unsigned int CubemapLoad(VECTOR(STRING) faces);
	/**
	* @brief : According fail path get HDR cube
	* @return: unsigned int
	*/
	unsigned int HdrLoad(PInt8 path);
	/**
	* @brief : Free image
	*/
	void FreeImage(Image * image);
private:
	///image hash map queue
	ImageQueue _imageQueue;
	///modle hash map queue
	ModelQueue _modelQueue;
};

}

