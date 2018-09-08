#include "Sprite.h"
#include "Renderer.h"
#include "VertexData.h"
#include "GameObject.h"
#include "Transform.h"
#include "GLGather.h"
#include "Configs.h"
#include "Resource.h"
#include "Image.h"

namespace Auto3D {

Sprite::Sprite(Ambient* ambient)
	:Super(ambient)
	, _shader(Shader(shader_path + "au_texture_transform.auvs"
		, shader_path + "au_texture_transform.aufs"))
{
	_imagePath = "Resource/texture/square.jpg";
	_color.Set(1.0f, 1.0f, 1.0f, 1.0f);
}

void Sprite::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<Sprite>(SCENE_ATTACH);
}

Sprite::~Sprite()
{
	UnloadOpaque(this);
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
	glDeleteBuffers(1, &_EBO);
}

void Sprite::SetImage(char * imagePath)
{
	_imagePath = imagePath;
}

void Sprite::Start()
{
	/////////////////////////////////////////////////////////////////////////////////////////////

	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);

	/////////////////////////////////////////////////////////////////////////////////////////////

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_vertices), texture_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(texture_vertices), texture_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	EnableVertexAttribs(VERTEX_ATTRIB_POSITION | VERTEX_ATTRIB_COLOR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	/////////////////////////////////////////////////////////////////////////////////////////////

	glGenTextures(1, &_textureData);
	glBindTexture(GL_TEXTURE_2D, _textureData);

	/////////////////////////////////////////////////////////////////////////////////////////////
	//_image = LocalImageLoad(_imagePath);
	_image = GetSubSystem<Resource>()->ImageLoad(_imagePath);


	//SetNearestParameters();
	SetLinerParameters();
	if (_image->value)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, _image->format, _image->width, _image->height, 0, _image->format, GL_UNSIGNED_BYTE, _image->value);
		GenerateMipmap();
	}
	else
	{
		WarningString("Failed to load texture");
	}

	
	//stbi_image_free(m_image->Value);
	RegisterOpaque(this);
	/////////////////////////////////////////////////////////////////////////////////////////////
}

void Sprite::Draw()
{
	GLApply();

	glBindTexture(GL_TEXTURE_2D, _textureData);
	_shader.Use();

	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;

	if (GetGameObjectPtr())
		modelMat = GetGameObject().GetComponent(Transform).GetTransformMat();
	else
		modelMat = Matrix4x4::identity;
	viewMat = GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix();
	projectionMat = GetSubSystem<Renderer>()->GetCurrentCamera().GetProjectionMatrix();

	_shader.SetMat4("model", modelMat);
	_shader.SetMat4("view", viewMat);
	_shader.SetMat4("projection", projectionMat);
	_shader.SetVec4("ourColor", _color.r, _color.g, _color.b, _color.a);

	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	GLOriginal();
}
void Sprite::SetColor(const Color& color)
{
	_color.Set(color.r, color.g, color.b, color.a);
}

void Sprite::SetColor(const Vector3& vec)
{
	_color.Set(vec.x, vec.y, vec.z, 1.0f);
}
void Sprite::SetColor(float r, float g, float b, float a)
{
	_color.Set(r, g, b, a);
}

//////////////////////////////////////////////////////////////////////////
//Image conpontent to use
void Sprite::SetLinerParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _image->format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _image->format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	if (_isMipmaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Sprite::SetNearestParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _image->format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _image->format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	if (_isMipmaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Sprite::SetTexParameters(const TexParams & params)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magFilter);
}

void Sprite::GenerateMipmap()
{
	glGenerateMipmap(GL_TEXTURE_2D);
	_isMipmaps = true;
}
}