#include "Texture2D.h"

AUTO_BEGIN
Texture2D::Texture2D()
	:m_shader(Shader(AtConfig::shader_path + "AUTO_texture.vs", AtConfig::shader_path + "AUTO_texture.fs"))
{
	//m_shader = Shader(AtConfig::shader_path + "AUTO_texture.vs", AtConfig::shader_path + "AUTO_texture.fs");
}
Texture2D::Texture2D(const Shader& shader)
	:m_shader(shader)
{
}


Texture2D::~Texture2D()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Texture2D::draw(const Vector2& vec)
{
	draw(Vector3(vec.x, vec.y, 0.0f));
}

void Texture2D::draw(const Vector3 & vec)
{
	
	GLfloat vertices[] = {
		// positions											// texture coords
		vec.x + 0.5f, vec.x + 0.5f,   0.0f,						1.0f, 1.0f, // top right
		vec.x + 0.5f, vec.x + -0.5f,	0.0f,					1.0f, 0.0f, // bottom right
		vec.x + -0.5f,vec.x + -0.5f,	0.0f,					0.0f, 0.0f, // bottom left
		vec.x + -0.5f,vec.x + 0.5f,	0.0f,						0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	/////////////////////////////////////////////////////////////////////////////////////////////

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	/////////////////////////////////////////////////////////////////////////////////////////////

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	//enableVertexAttribs(VERTEX_ATTRIB_POSITION | VERTEX_ATTRIB_COLOR);
	text();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	/////////////////////////////////////////////////////////////////////////////////////////////
	// 加载并创建纹理
	// -------------------------

	glGenTextures(1, &texture);			
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	/////////////////////////////////////////////////////////////////////////////////////////////


	int width, height, nrChannels;
	unsigned char *data = stbi_load("Resource/texture/square.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	/////////////////////////////////////////////////////////////////////////////////////////////

	
	
}

void Texture2D::pushToRunloop()
{
	m_shader.use();
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Texture2D::updateData(const Vector3& position)
{

}

void Texture2D::updateData(const Vector2& position)
{

}
AUTO_END