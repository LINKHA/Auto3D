#include "SpriteRenderer.h"
#include "Renderer.h"
#include "VertexData.h"
#include "Transform.h"
#include "AutoOGL.h"
#include "Configs.h"
#include "ResourceSystem.h"
#include "Image.h"
#include "NewDef.h"

namespace Auto3D {

SpriteRenderer::SpriteRenderer(Ambient* ambient)
	:Super(ambient)
	, _shader(_Shader(shader_path + "au_texture_transform.auvs"
		, shader_path + "au_texture_transform.aufs"))
{
	_color.Set(1.0f, 1.0f, 1.0f, 1.0f);
}

SpriteRenderer::~SpriteRenderer()
{
	if (_image->GetType() == ImageType::Opaque)
		UnloadOpaque(this);
	else if (_image->GetType() == ImageType::Custom)
		UnloadCustom(this);
	else if (_image->GetType() == ImageType::Translucent)
		UnloadTranslucent(this);

	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
	glDeleteBuffers(1, &_EBO);
}

void SpriteRenderer::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<SpriteRenderer>(SCENE_ATTACH);
}

void SpriteRenderer::SetImage(Image* image)
{
	_image.reset(image);
}

void SpriteRenderer::Start()
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

	//SetNearestParameters();
	SetLinerParameters();
	
	if (_image->GetData())
	{
		glTexImage2D(GL_TEXTURE_2D,0, 
			_image->GetFormat(),
			_image->GetWidth(),
			_image->GetHeight(),0, 
			_image->GetFormat(),
			GL_UNSIGNED_BYTE,
			_image->GetData());
		GenerateMipmap();
	}
	else
	{
		WarningString("Failed to load texture");
	}
	if(_image->GetType() == ImageType::Opaque)
		RegisterOpaque(this);
	else if (_image->GetType() == ImageType::Custom)
		RegisterCustom(this);
	else if (_image->GetType() == ImageType::Translucent)
		RegisterTranslucent(this);
	/////////////////////////////////////////////////////////////////////////////////////////////
}

void SpriteRenderer::Draw()
{
	GLApply();

	glBindTexture(GL_TEXTURE_2D, _textureData);
	_shader.Use();

	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;

	if (GetNodePtr())
		modelMat = GetNode().GetComponent<Transform>()->GetTransformMat();
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
void SpriteRenderer::SetColor(const Color& color)
{
	_color.Set(color.r, color.g, color.b, color.a);
}

void SpriteRenderer::SetColor(const Vector3& vec)
{
	_color.Set(vec.x, vec.y, vec.z, 1.0f);
}
void SpriteRenderer::SetColor(float r, float g, float b, float a)
{
	_color.Set(r, g, b, a);
}

//////////////////////////////////////////////////////////////////////////
//tImage conpontent to use
void SpriteRenderer::SetLinerParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _image->GetFormat() == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _image->GetFormat() == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
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

void SpriteRenderer::SetNearestParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _image->GetComponents() == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _image->GetComponents() == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
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

void SpriteRenderer::SetTexParameters(const TexParams& params)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magFilter);
}

void SpriteRenderer::GenerateMipmap()
{
	glGenerateMipmap(GL_TEXTURE_2D);
	_isMipmaps = true;
}
}