#pragma once
#include "ComponentSetting.h"
#include "glm/glm.hpp"
#include "stl_use.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
namespace Auto3D {
/**
* @brief : mesh vertex message
*/
struct MeshVertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};
/**
* @brief : texture data message
*/
struct TextureData {
	unsigned int data;
	__String type;
	__String path;
};
class Shader;
/**
* @brief : mesh node message
*/
class MeshNode
{
public:
	/**
	* @brief : Set vertices indeices and texture
	*/
	MeshNode(_VECTOR(MeshVertex) vertices, _VECTOR(unsigned int) indices, _VECTOR(TextureData) textures);
	~MeshNode() = default;
	/**
	* @brief : Render the mesh
	*/
	void Draw(const Shader& shader);
private:
	/**
	* @brief : Initializes all the buffer objects/arrays
	*/
	void setupMesh();
public:
	_VECTOR(unsigned int) indices;
	_VECTOR(MeshVertex) vertices;
	_VECTOR(TextureData) textures;
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
};

/**
*@brief : component setting with load model
*/
class Model : public ComponentSetting
{
	REGISTER_DERIVED_CLASS(Model, ComponentSetting);
	DECLARE_OBJECT_SERIALIZE(Model);
	using TextureDatas = _VECTOR(TextureData);
	using MeshNodes = _VECTOR(MeshNode);
public:
	/**
	* @brief : Add path and gamma to build model
	*/
	Model(Ambient* ambient,__String const& path = "../Resource/object/base/Cube.3DS", bool gamma = false);
	/**
	* @brief : Draw all this model mesh node
	*/
	void Draw(Shader shader);
	/**
	* @brief : Get texture datas container
	* @return : _VECTOR(TextureData)
	*/
	TextureDatas& GetTextureDatas() { return _textureDatas; }
	/**
	* @brief : Get mesh node message container
	* @return : _VECTOR(MeshNode)
	*/
	MeshNodes& GetMeshNodes() { return _meshNodes; }
private:
	/**
	* @brief : The model with ASSIMP extension is loaded from the file and the 
	*		    generated grid is stored in the grid vector.
	*/
	bool loadModel(__String const& path);
	/**
	* @brief : Processes nodes recursively. Process each individual grid on 
	*			the node and repeat this process, if any, on its child nodes.
	*/
	void processNode(aiNode* node, const aiScene* scene);
	/**
	* @brief : Populate data into Mesh
	*/
	MeshNode processMesh(aiMesh* mesh, const aiScene* scene);
	/**
	* @brief : load all material texture with this file
	*/
	TextureDatas loadMaterialTextures(aiMaterial* mat, aiTextureType type, __String typeName);
private:
	///texture data message container
	TextureDatas _textureDatas;
	///mesh node message container
	MeshNodes _meshNodes;
	///path remove the file name
	__String _directory;
	///gamma
	bool _gammaCorrection;
	///file path
	__String _path;
	///judge this file is null
	bool _isNull;
};

}