#include "Texture2D.h"
#include "RenderManager.h"
AUTO_BEGIN

#if TEXTURE_DEBUG
Texture2D::Texture2D()
	: m_ImagePath("Resource/texture/square.jpg")
	, m_shader(Shader(AtConfig::shader_path + "au_texture_transform.auvs", AtConfig::shader_path + "au_texture_transform.aufs"))
{
}
Texture2D::Texture2D(_String imagePath)
	: m_ImagePath(imagePath)
	, m_shader(Shader(AtConfig::shader_path + "au_texture_transform.auvs", AtConfig::shader_path + "au_texture_transform.aufs"))
{
}
Texture2D::Texture2D(_String imagePath, const Shader & shader)
	: m_ImagePath(imagePath)
	, m_shader(shader)
{
}
Texture2D::~Texture2D()
{
	glDeleteVertexArrays(1, &t_VAO);
	glDeleteBuffers(1, &t_VBO);
	glDeleteBuffers(1, &t_EBO);
}


void Texture2D::Start()
{

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
	BindTexture2D(textureData);

	//setNearestParameters();
	SetLinerParameters();
	/////////////////////////////////////////////////////////////////////////////////////////////
	Image * image = LocalImageLoad((char*)m_ImagePath.data());

	if (image->Value)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->Width, image->Height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->Value);
		GenerateMipmap();
	}
	else
	{
		ErrorString("Failed to load texture");
	}

	//stbi_image_free(image->m_Image);

	/////////////////////////////////////////////////////////////////////////////////////////////
}

void Texture2D::Update()
{


	m_shader.Use();

	//m_transform.ptr = GetGameObject().GetTransformPtr();

	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;

	if (GetGameObjectPtr())
		modelMat = GetGameObject().GetComponent(Transform).GetTransformMat();
	else
		modelMat = Matrix4x4::identity;
	viewMat = INSTANCE(RenderManager).CameraArray.find(0)->second->GetViewMatrix();
	projectionMat = glm::perspective(INSTANCE(RenderManager).CameraArray.find(0)->second->Zoom, (float)800 / (float)600, 0.1f, 100.0f);
	//projectionMat = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 100.0f);
	m_shader.SetMat4("model", modelMat);
	m_shader.SetMat4("view", viewMat);
	m_shader.SetMat4("projection", projectionMat);
	m_shader.SetVec4("ourColor", m_Color.r, m_Color.g, m_Color.b, m_Color.a);
	glBindTexture(GL_TEXTURE_2D, textureData);
	glBindVertexArray(t_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}



//////////////////////////////////////////////////////////////////////////
//Image conpontent to use
void Texture2D::SetLinerParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
#else
Texture2D::Texture2D()
	: m_ImagePath("Resource/texture/square.jpg")
	, m_shader(Shader(AtConfig::shader_path + "au_texture_transform.auvs", AtConfig::shader_path + "au_texture_transform.aufs"))
{
}
Texture2D::Texture2D(_String imagePath)
	: m_ImagePath(imagePath)
	, m_shader(Shader(AtConfig::shader_path + "au_texture_transform.auvs", AtConfig::shader_path + "au_texture_transform.aufs"))
{
}
Texture2D::Texture2D(_String imagePath, const Shader & shader)
	: m_ImagePath(imagePath)
	, m_shader(shader)
{
}
Texture2D::~Texture2D()
{
	glDeleteVertexArrays(1, &t_VAO);
	glDeleteBuffers(1, &t_VBO);
	glDeleteBuffers(1, &t_EBO);
}


void Texture2D::Start()
{
	
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
	BindTexture2D(textureData);

	//setNearestParameters();
	SetLinerParameters();
	/////////////////////////////////////////////////////////////////////////////////////////////
	Image * image = LocalImageLoad((char*)m_ImagePath.data());

	if (image->Value)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->Width, image->Height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->Value);
		GenerateMipmap();
	}
	else
	{
		ErrorString("Failed to load texture");
	}

	//stbi_image_free(image->m_Image);

	/////////////////////////////////////////////////////////////////////////////////////////////
}

void Texture2D::Update()
{

	
	m_shader.Use();

	//m_transform.ptr = GetGameObject().GetTransformPtr();

	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;

	if (GetGameObject().GetTransformPtr())
		modelMat = GetGameObject().GetTransformPtr()->GetTransformMat();
	else
		modelMat = Matrix4x4::identity;
	viewMat = Application::Instance().m_camera.GetViewMatrix();
	projectionMat = glm::perspective(Application::Instance().m_camera.Zoom, (float)800 / (float)600, 0.1f, 100.0f);
////projectionMat = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 100.0f);
	m_shader.SetMat4("model", modelMat);
	m_shader.SetMat4("view", viewMat);
	m_shader.SetMat4("projection", projectionMat);
	m_shader.SetVec4("ourColor", m_Color.r, m_Color.g, m_Color.b, m_Color.a);
	glBindTexture(GL_TEXTURE_2D, textureData);
	glBindVertexArray(t_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}



//////////////////////////////////////////////////////////////////////////
//Image conpontent to use
void Texture2D::SetLinerParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

#endif //TEXTURE_DEBUG


AUTO_END