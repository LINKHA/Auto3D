#include "Sprite.h"
#include "RenderManager.h"
#include "VertexData.h"
#include "GameObject.h"
#include "Transform.h"
#include "OpenGLGather.h"
#include "LoadResource.h"
#include "GLWindow.h"
AUTO_BEGIN

Sprite::Sprite()
	: m_shader(Shader(AtConfig::shader_path + "au_texture_transform.auvs"
		, AtConfig::shader_path + "au_texture_transform.aufs"))
{
	m_ImagePath.ptr = "Resource/texture/square.jpg";
}
Sprite::Sprite(char* imagePath)
	: m_shader(Shader(AtConfig::shader_path + "au_texture_transform.auvs"
		, AtConfig::shader_path + "au_texture_transform.aufs"))
{
	m_ImagePath.ptr = imagePath;
}
Sprite::Sprite(char* imagePath, const Shader & shader)
	: m_shader(shader)
{
	m_ImagePath.ptr = imagePath;
}
Sprite::~Sprite()
{
	glDeleteVertexArrays(1, &t_VAO);
	glDeleteBuffers(1, &t_VBO);
	glDeleteBuffers(1, &t_EBO);
}


void Sprite::Start()
{
	/////////////////////////////////////////////////////////////////////////////////////////////

	glGenVertexArrays(1, &t_VAO);
	glBindVertexArray(t_VAO);
	glGenBuffers(1, &t_VBO);
	glGenBuffers(1, &t_EBO);

	/////////////////////////////////////////////////////////////////////////////////////////////

	glBindBuffer(GL_ARRAY_BUFFER, t_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_vertices), texture_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(texture_vertices), texture_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	EnableVertexAttribs(VERTEX_ATTRIB_POSITION | VERTEX_ATTRIB_COLOR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	/////////////////////////////////////////////////////////////////////////////////////////////

	glGenTextures(1, &textureData);
	glBindTexture(GL_TEXTURE_2D, textureData);

	/////////////////////////////////////////////////////////////////////////////////////////////
	m_image.ptr = LocalImageLoad(m_ImagePath.ptr);
	//SetNearestParameters();
	SetLinerParameters();
	if (m_image.ptr->Value)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, m_image.ptr->Format, m_image.ptr->Width, m_image.ptr->Height, 0, m_image.ptr->Format, GL_UNSIGNED_BYTE, m_image.ptr->Value);
		GenerateMipmap();
	}
	else
	{
		WarningString("Failed to load texture");
	}

	//stbi_image_free(m_image.ptr->Value);

	/////////////////////////////////////////////////////////////////////////////////////////////
}

void Sprite::Draw(Camera * cam)
{
	GLApply();

	if (cam == nullptr)
	{
		WarningString("Fail to find camera");
		return;
	}
	glBindTexture(GL_TEXTURE_2D, textureData);
	m_shader.Use();

	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;

	if (GetGameObjectPtr())
		modelMat = GetGameObject().GetComponent(Transform).GetTransformMat();
	else
		modelMat = Matrix4x4::identity;
	viewMat = cam->GetViewMatrix();
	projectionMat = cam->GetProjectionMatrix();

	m_shader.SetMat4("model", modelMat);
	m_shader.SetMat4("view", viewMat);
	m_shader.SetMat4("projection", projectionMat);
	m_shader.SetVec4("ourColor", m_Color.r, m_Color.g, m_Color.b, m_Color.a);

	glBindVertexArray(t_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	GLOriginal();
}
void Sprite::SetColor(const Color& color)
{
	m_Color.Set(color.r, color.g, color.b, color.a);
}

void Sprite::SetColor(const Vector3& vec)
{
	m_Color.Set(vec.x, vec.y, vec.z, 1.0f);
}
void Sprite::SetColor(float r, float g, float b, float a)
{
	m_Color.Set(r, g, b, a);
}

//////////////////////////////////////////////////////////////////////////
//Image conpontent to use
void Sprite::SetLinerParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_image.ptr->Format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_image.ptr->Format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	if (is_Mipmaps)
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_image.ptr->Format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_image.ptr->Format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	if (is_Mipmaps)
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
	is_Mipmaps = true;
}
AUTO_END