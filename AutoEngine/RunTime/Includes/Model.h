#pragma once
#include "ComponentSetting.h"
#include "glm/glm.hpp"
#include "stl_use.h"

namespace Auto3D {

struct MeshVertex {
	/// position
	glm::vec3 Position;
	/// normal
	glm::vec3 Normal;
	/// texCoords
	glm::vec2 TexCoords;
	/// tangent
	glm::vec3 Tangent;
	/// bitangent
	glm::vec3 Bitangent;
};
struct TextureData {
	unsigned int data;
	_String type;
	_String path;
};
class Shader;
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
private:
	_VECTOR(MeshVertex) _vertices;
	_VECTOR(unsigned int) _indices;
	_VECTOR(TextureData) _textures;
	unsigned int _vao;
	unsigned int _vbo;
	unsigned int _ebo;
};


class Model : public ComponentSetting
{
	REGISTER_DERIVED_CLASS(Model, ComponentSetting);
	DECLARE_OBJECT_SERIALIZE(Model);
	using TextureDatas = _VECTOR(TextureData);
	using MeshNodes = _VECTOR(MeshNode);
public:
	explicit Model(Ambient* ambient,_String const& path, bool gamma = false);
	/**
	* @brief : Draw all this model mesh node
	*/
	void Draw(Shader shader);

private:
	/**
	* @brief : The model with ASSIMP extension is loaded from the file and the 
	*		    generated grid is stored in the grid vector.
	*/
	bool loadModel(_String const& path);
	/**
	* @brief : Processes nodes recursively. Process each individual grid on 
	*			the node and repeat this process, if any, on its child nodes.
	*/
	void processNode(aiNode* node, const aiScene* scene);
	/**
	* @brief : Populate data into Mesh
	*/
	MeshNode processMesh(aiMesh* mesh, const aiScene* scene);

	TextureDatas loadMaterialTextures(aiMaterial* mat, aiTextureType type, _String typeName);
private:
	TextureDatas _textureDatas;
	MeshNodes _meshNodes;
	_String _directory;
	bool _gammaCorrection;
	_String _path;
	bool _isNull;
};

}