#include "Mesh.h"
#include "Deserializer.h"
#include "ResourceSystem.h"
#include "ShaderVariation.h"


namespace Auto3D {

/////////////////////////////////////////////////////////////////////////////////////////////
//MeshNode
/////////////////////////////////////////////////////////////////////////////////////////////
MeshNode::MeshNode(VECTOR<MeshVertex> tVertices, VECTOR<unsigned int> tIndices, VECTOR<TextureData> tTextures)
{
	vertices = tVertices;
	indices = tIndices;
	textures = tTextures;

	// now that we have all the required data, set the vertex buffers and its attribute pointers.
	setupMesh();
}
void MeshNode::Draw(ShaderVariation* shader)
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
										  // retrieve texture number (the N in diffuse_textureN)
		STRING number;
		STRING name = textures[i].type;
		if (name == "texture_diffuse")
			number = KhSTL::ToString(diffuseNr++);
		else if (name == "texture_specular")
			number = KhSTL::ToString(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = KhSTL::ToString(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = KhSTL::ToString(heightNr++); // transfer unsigned int to stream

												 // now set the sampler to the correct texture unit

		glUniform1i(glGetUniformLocation(shader->GetHandle().name, (name + number).CStr()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].data);
	}

	// draw mesh
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void MeshNode::setupMesh()
{
	// create buffers/arrays
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)0);
	// vertex normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, normal));
	// vertex texture position
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, texCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, bitangent));

	glBindVertexArray(0);
}



Mesh::Mesh(SharedPtr<Ambient> ambient)
	:Super(ambient)
{}

Mesh::~Mesh() = default;

void Mesh::RegisterObject(SharedPtr<Ambient> ambient)
{
	ambient->RegisterFactory<Mesh>();
}

bool Mesh::BeginLoad(Deserializer& source)
{
	STRING fileID = source.ReadFileID();
	//if (fileID != "UMDL" && fileID != "UMD2")
	//{
	//	WarningString(source.GetName() + " is not a valid model file");
	//	return false;
	//}
	//bool hasVertexDeclarations = (fileID == "UMD2");

	_isNull = !loadModel(source.GetName());

	return true;
}

void Mesh::DrawMesh(ShaderVariation* shader)
{
	for (unsigned int i = 0; i < _meshNodes.size(); i++)
		_meshNodes[i].Draw(shader);
}

bool Mesh::loadModel(STRING const & path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(std::string(path.CStr()), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		ErrorString("importer.GetErrorString()\n");
		return false;
	}
	// retrieve the directory path of the filepath
	_directory = path.SubString(0, path.FindLast('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
	return true;
}

void Mesh::processNode(aiNode* node, const aiScene* scene)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshNodes.push_back(processMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

MeshNode Mesh::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// data to fill
	VECTOR<MeshVertex> vertices;
	VECTOR<unsigned int> indices;
	VECTOR<TextureData> textures;
	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		MeshVertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
						  // positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.tangent = vector;
		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.bitangent = vector;
		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// diffuse maps
	VECTOR<TextureData> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// specular maps
	VECTOR<TextureData> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// normal maps
	VECTOR<TextureData> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// height maps
	VECTOR<TextureData> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	return MeshNode(vertices, indices, textures);
}

VECTOR<TextureData> Mesh::loadMaterialTextures(aiMaterial* mat, aiTextureType type, STRING typeName)
{
	VECTOR<TextureData> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < _textureDatas.size(); j++)
		{
			if (std::strcmp(_textureDatas[j].path.CStr(), str.C_Str()) == 0)
			{
				textures.push_back(_textureDatas[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			TextureData texture;

			STRING filename = STRING(str.data);
			filename = _directory + "/" + filename;
			texture.data = GetSubSystem<ResourceSystem>()->TextureLoad((char*)filename.CStr(), false);

			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			_textureDatas.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

}