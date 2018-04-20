#include "Texture2D.h"

AUTO_BEGIN



Texture2D::Texture2D()
	:m_shader(Shader(AtConfig::shader_path + "au_texture_transform.auvs", AtConfig::shader_path + "au_texture_transform.aufs"))
	, m_transform(Transform())
{
}
Texture2D::Texture2D(const Shader& shader)
	:m_shader(shader)
	, m_transform(Transform())
{
}


Texture2D::~Texture2D()
{
	glDeleteVertexArrays(1, &t_VAO);
	glDeleteBuffers(1, &t_VBO);
	glDeleteBuffers(1, &t_EBO);
}

void Texture2D::draw(const Vector2& vec)
{
	draw(Vector3(vec.x, vec.y, 0.0f));
}

void Texture2D::draw(const Vector3 & vec)
{
	GLfloat vertices[] = {
		// positions											// texture coords
		vec.x + 0.5f, vec.y + 0.5f,  					1.0f, 1.0f, // top right
		vec.x + 0.5f, vec.y + -0.5f,					1.0f, 0.0f, // bottom right
		vec.x + -0.5f,vec.y + -0.5f,					0.0f, 0.0f, // bottom left
		vec.x + -0.5f,vec.y + 0.5f,						0.0f, 1.0f  // top left 
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	enableVertexAttribs(VERTEX_ATTRIB_POSITION | VERTEX_ATTRIB_COLOR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	/////////////////////////////////////////////////////////////////////////////////////////////
	bindTexture2D(textureData);

	//setNearestParameters();
	setLinerParameters();
	/////////////////////////////////////////////////////////////////////////////////////////////
	
	Image * image = localImageLoad("Resource/texture/square.jpg");

	if (image->m_Image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->m_Width, image->m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->m_Image);
		generateMipmap();
	}
	else
	{
		ErrorString("Failed to load texture");
	} 

	//stbi_image_free(image->m_Image);

	/////////////////////////////////////////////////////////////////////////////////////////////
}

void Texture2D::pushToRunloop()
{
	

	m_shader.use();

	float scaleAmount = (float)sin(glfwGetTime());

	
	m_transform.setPosition(Vector3(0.5f, 0.5f, 0.0f));
	m_transform.setRotation(Vector3(0.0, 0.0f, 90.0f));
	//m_transform.setRotation(90.0f, Vector3::zAxis));
	m_transform.setScale(Vector3(scaleAmount));


	m_transform.updateTransform();

	unsigned int transformLoc = glGetUniformLocation(m_shader.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m_transform.getTransformMat()));
	glBindTexture(GL_TEXTURE_2D, textureData);
	glBindVertexArray(t_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//init transform
	m_transform.identity();
}



//////////////////////////////////////////////////////////////////////////
//Image conpontent to use
void Texture2D::setLinerParameters()
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

void Texture2D::setNearestParameters()
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

void Texture2D::setTexParameters(const TexParams & params)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magFilter);
}

void Texture2D::generateMipmap()
{
	glGenerateMipmap(GL_TEXTURE_2D);
	is_Mipmaps = true;
}




AUTO_END