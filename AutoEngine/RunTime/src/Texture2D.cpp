#include "Texture2D.h"
#include "RenderManager.h"
AUTO_BEGIN

GLfloat vertices[] = {
	// positions			// texture coords
	0.5f, 0.5f, 0.0f, 			1.0f, 1.0f, // top right
	0.5f, -0.5f,0.0f,			1.0f, 0.0f, // bottom right
	-0.5f,-0.5f,0.0f,			0.0f, 0.0f, // bottom left
	-0.5f,0.5f,	0.0f,			0.0f, 1.0f  // top left 
};

unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

Texture2D::Texture2D() 
	: m_shader(Shader(AtConfig::shader_path + "au_texture_transform.auvs", AtConfig::shader_path + "au_texture_transform.aufs"))
	, useStencil(false)
	, useDepth(true)
	, useBlend(false)
{
	m_ImagePath.ptr = "Resource/texture/square.jpg";
}
Texture2D::Texture2D(char* imagePath)
	: m_shader(Shader(AtConfig::shader_path + "au_texture_transform.auvs", AtConfig::shader_path + "au_texture_transform.aufs"))
	, useStencil(false)
	, useDepth(true)
	, useBlend(false)
{
	m_ImagePath.ptr = imagePath;
}
Texture2D::Texture2D(char* imagePath, const Shader & shader)
	: m_shader(shader)
	, useStencil(false)
	, useDepth(true)
	, useBlend(false)
{
	m_ImagePath.ptr = imagePath;
}
Texture2D::~Texture2D()
{
	glDeleteVertexArrays(1, &t_VAO);
	glDeleteBuffers(1, &t_VBO);
	glDeleteBuffers(1, &t_EBO);
}


void Texture2D::Start()
{
	/////////////////////////////////////////////////////////////////////////////////////////////

	glGenVertexArrays(1, &t_VAO);
	glBindVertexArray(t_VAO);
	glGenBuffers(1, &t_VBO);
	glGenBuffers(1, &t_EBO);

	/////////////////////////////////////////////////////////////////////////////////////////////

	glBindBuffer(GL_ARRAY_BUFFER, t_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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

void Texture2D::Draw(Camera * cam)
{
	if (useDepth)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(m_depthfunc);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
	if (useStencil)
	{
		glEnable(GL_STENCIL_TEST);
		void StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
		void StencilFunc(GLenum func, GLint ref, GLuint mask);
		void StencilMask(GLuint mask);
		if (m_sfail)
			glStencilOp(m_sfail, m_dpfail, m_dppass);
		else
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		if (m_func)
			glStencilFunc(m_func, m_ref, m_mask);
		else
			glStencilFunc(GL_ALWAYS, 1, 0xFF);

		glStencilMask(m_mas);

	}
	if (useBlend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}


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
	RectInt rect = INSTANCE(GLWindow).GetWindowRectInt();
	projectionMat = glm::perspective(cam->Zoom,
		((float)rect.width * (float)cam->ViewRect.width) /
		((float)rect.height * (float)cam->ViewRect.height),
		cam->Near, cam->Far);
	//projectionMat = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 100.0f);
	m_shader.SetMat4("model", modelMat);
	m_shader.SetMat4("view", viewMat);
	m_shader.SetMat4("projection", projectionMat);
	m_shader.SetVec4("ourColor", m_Color.r, m_Color.g, m_Color.b, m_Color.a);
	
	glBindVertexArray(t_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	if (useStencil)
	{
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glDisable(GL_STENCIL_TEST);
		glStencilMask(0xFF);
	}
	if (!useDepth)
		glEnable(GL_DEPTH_TEST);
	else
		glDepthFunc(GL_LESS);
	if (useBlend)
	{
		glDisable(GL_BLEND);

	}
}
//////////////////////////////////////////////////////////////////////////
//Image conpontent to use
void Texture2D::SetLinerParameters()
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

void Texture2D::SetNearestParameters()
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

void Texture2D::SetTexParameters(const TexParams & params)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magFilter);
}

void Texture2D::GenerateMipmap()
{
	glGenerateMipmap(GL_TEXTURE_2D);
	is_Mipmaps = true;
}

void Texture2D::SetColor(const Color& color)
{
	m_Color.Set(color.r, color.g, color.b, color.a);
}

void Texture2D::SetColor(const Vector3& vec)
{
	m_Color.Set(vec.x, vec.y, vec.z, 1.0f);
}
void Texture2D::SetColor(float r, float g, float b, float a)
{
	m_Color.Set(r, g, b, a);
}

void Texture2D::StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{
	m_sfail = sfail;
	m_dpfail = dpfail;
	m_dppass = dppass;
}
void Texture2D::StencilFunc(GLenum func, GLint ref, GLuint mask)
{
	m_func = func;
	m_ref = ref;
	m_mask = mask;
}
void Texture2D::StencilMask(GLuint mask)
{
	m_mas = mask;
}
void Texture2D::DepthFunc(GLenum func)
{
	m_depthfunc = func;
}

AUTO_END