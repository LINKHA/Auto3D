#include "TextureNormal.h"
#include "RenderManager.h"
AUTO_BEGIN
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		// positions
		glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
		glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
		glm::vec3 pos3(1.0f, -1.0f, 0.0f);
		glm::vec3 pos4(1.0f, 1.0f, 0.0f);
		// texture coordinates
		glm::vec2 uv1(0.0f, 1.0f);
		glm::vec2 uv2(0.0f, 0.0f);
		glm::vec2 uv3(1.0f, 0.0f);
		glm::vec2 uv4(1.0f, 1.0f);
		// normal vector
		glm::vec3 nm(0.0f, 0.0f, 1.0f);

		// calculate tangent/bitangent vectors of both triangles
		glm::vec3 tangent1, bitangent1;
		glm::vec3 tangent2, bitangent2;
		// triangle 1
		// ----------
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);

		// triangle 2
		// ----------
		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent2 = glm::normalize(tangent2);


		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent2 = glm::normalize(bitangent2);


		float quadVertices[] = {
			// positions            // normal         // texcoords  // tangent                          // bitangent
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
		};
		// configure plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

GLfloat t_vertices[] = {
	// positions			// texture coords
	0.5f, 0.5f, 0.0f, 			1.0f, 1.0f, // top right
	0.5f, -0.5f,0.0f,			1.0f, 0.0f, // bottom right
	-0.5f,-0.5f,0.0f,			0.0f, 0.0f, // bottom left
	-0.5f,0.5f,	0.0f,			0.0f, 1.0f  // top left 
};

unsigned int t_indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

TextureNormal::TextureNormal()
	: m_shader(Shader(AtConfig::shader_path + "au_normal_mapping.auvs", AtConfig::shader_path + "au_normal_mapping.aufs"))
	, useStencil(false)
	, useDepth(true)
	, useBlend(false)
{
	m_ImagePath.ptr = "Resource/texture/brickwall.jpg";
	m_ImageNormalPath.ptr = "Resource/texture/brickwall_normal.jpg";
}
TextureNormal::TextureNormal(char* imagePath)
	: m_shader(Shader(AtConfig::shader_path + "au_normal_mapping.auvs", AtConfig::shader_path + "au_normal_mapping.aufs"))
	, useStencil(false)
	, useDepth(true)
	, useBlend(false)
{
	m_ImagePath.ptr = imagePath;
}
TextureNormal::TextureNormal(char* imagePath, const Shader & shader)
	: m_shader(shader)
	, useStencil(false)
	, useDepth(true)
	, useBlend(false)
{
	m_ImagePath.ptr = imagePath;
}
TextureNormal::~TextureNormal()
{
	glDeleteVertexArrays(1, &t_VAO);
	glDeleteBuffers(1, &t_VBO);
	glDeleteBuffers(1, &t_EBO);
}


void TextureNormal::Start()
{
	/////////////////////////////////////////////////////////////////////////////////////////////

	//glGenVertexArrays(1, &t_VAO);
	//glBindVertexArray(t_VAO);
	//glGenBuffers(1, &t_VBO);
	//glGenBuffers(1, &t_EBO);

	///////////////////////////////////////////////////////////////////////////////////////////////


	//glBindBuffer(GL_ARRAY_BUFFER, t_VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(t_vertices), t_vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t_EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(t_indices), t_indices, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	//EnableVertexAttribs(VERTEX_ATTRIB_POSITION | VERTEX_ATTRIB_COLOR);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
	///////////////////////////////////////////////////////////////////////////////////////////////

	//glGenTextures(1, &textureData);
	//glBindTexture(GL_TEXTURE_2D, textureData);

	/////////////////////////////////////////////////////////////////////////////////////////////
	m_image = LocalTextureLoad(m_ImagePath.ptr);
	m_imageNormal = LocalTextureLoad(m_ImageNormalPath.ptr);

	m_shader.Use();
	m_shader.SetInt("diffuseMap", 0);
	m_shader.SetInt("normalMap", 1);
	//stbi_image_free(m_image.ptr->Value);

	/////////////////////////////////////////////////////////////////////////////////////////////
}

void TextureNormal::Draw(Camera * cam)
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
	//glBindTexture(GL_TEXTURE_2D, textureData);
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
	if (cam->GetSortMode() == kSortPerspective)
	{
		projectionMat = glm::perspective(cam->Zoom,
			((float)rect.width * (float)cam->ViewRect.width) /
			((float)rect.height * (float)cam->ViewRect.height),
			cam->Near, cam->Far);
	}
	else if (cam->GetSortMode() == kSortOrthographic)
	{
		float t = ((float)rect.width * (float)cam->ViewRect.width) /
			((float)rect.height * (float)cam->ViewRect.height);
		projectionMat = glm::ortho(-t, t, -1.0f, 1.0f, cam->Near, cam->Far);
	}
	else
	{
		ErrorString("Fail load projection mat");
	}

	m_shader.SetMat4("model", modelMat);
	m_shader.SetMat4("view", viewMat);
	m_shader.SetMat4("projection", projectionMat);
	//m_shader.SetVec4("ourColor", m_Color.r, m_Color.g, m_Color.b, m_Color.a);
	glm::vec3 lightPos(0.5f, 1.0f, 0.3f);

	m_shader.SetVec3("viewPos", cam->Position);
	m_shader.SetVec3("lightPos", lightPos);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_image);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_imageNormal);

	renderQuad();

	/*glBindVertexArray(t_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);*/

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
void TextureNormal::SetTexParameters(const TexParams & params)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magFilter);
}

void TextureNormal::GenerateMipmap()
{
	glGenerateMipmap(GL_TEXTURE_2D);
	is_Mipmaps = true;
}

void TextureNormal::SetColor(const Color& color)
{
	m_Color.Set(color.r, color.g, color.b, color.a);
}

void TextureNormal::SetColor(const Vector3& vec)
{
	m_Color.Set(vec.x, vec.y, vec.z, 1.0f);
}
void TextureNormal::SetColor(float r, float g, float b, float a)
{
	m_Color.Set(r, g, b, a);
}

void TextureNormal::StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{
	m_sfail = sfail;
	m_dpfail = dpfail;
	m_dppass = dppass;
}
void TextureNormal::StencilFunc(GLenum func, GLint ref, GLuint mask)
{
	m_func = func;
	m_ref = ref;
	m_mask = mask;
}
void TextureNormal::StencilMask(GLuint mask)
{
	m_mas = mask;
}
void TextureNormal::DepthFunc(GLenum func)
{
	m_depthfunc = func;
}


AUTO_END