#pragma once
#include "Resource.h"
#include "glm/glm.hpp"
#include "Assimp/include/assimp/Importer.hpp"
#include "Assimp/include/assimp/scene.h"
#include "Assimp/include/assimp/postprocess.h"

namespace Auto3D {

class Shader;

/**
* mesh vertex message
*/
struct MeshVertex 
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};
/**
* texture data message
*/
struct TextureData
{
	unsigned int data;
	STRING type;
	STRING path;
};

/**
* mesh node message
*/
class MeshNode
{
public:
	/**
	* @brief : Set vertices indeices and texture
	*/
	MeshNode(VECTOR<MeshVertex> vertices, VECTOR<unsigned int> indices, VECTOR<TextureData> textures);
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
	VECTOR<unsigned int> indices;
	VECTOR<MeshVertex> vertices;
	VECTOR<TextureData> textures;
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
};

class Mesh : public Resource
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Mesh, Resource);
	DECLARE_OBJECT_SERIALIZE(Mesh)
public:
	explicit Mesh(Ambient* ambient);
	/**
	* @brief : Register object factory
	*/
	static void RegisterObject(Ambient* ambient);
	/*
	*@brief : Load resource from stream.May be called from a worker thread.Return true if successful
	*/
	bool BeginLoad(Deserializer& source)override;
	/**
	* @brief : Draw all this model mesh node
	*/
	void DrawMesh(const Shader& shader);
	/**
	* @brief : Get texture datas container
	* @return : VECTOR(TextureData)
	*/
	VECTOR<TextureData> GetTextureDatas() { return _textureDatas; }
	/**
	* @brief : Get mesh node message container
	* @return : VECTOR(MeshNode)
	*/
	VECTOR<MeshNode> GetMeshNodes() { return _meshNodes; }
private:
	/**
	* @brief : The model with ASSIMP extension is loaded from the file and the
	*		    generated grid is stored in the grid vector.
	*/
	bool loadModel(STRING const& path);
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
	VECTOR<TextureData> loadMaterialTextures(aiMaterial* mat, aiTextureType type, STRING typeName);
private:
	///texture data message container
	VECTOR<TextureData> _textureDatas;
	///mesh node message container
	VECTOR<MeshNode> _meshNodes;
	///path remove the file name
	STRING _directory;
	///gamma
	bool _gammaCorrection;
	///file path
	STRING _path;
	///judge this file is null
	bool _isNull;
};

}