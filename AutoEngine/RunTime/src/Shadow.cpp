#include "Shadow.h"
#include "AtConfig.h"
#include "GLWindow.h"
#include "Camera.h"
#include "ShadowTest.h"
#include "stb_image.h"
#include "VertexData.h"
AUTO_BEGIN



Shadow::Shadow()
	:m_ShadowMap(AtConfig::shader_path + "au_shadow_mapping.auvs"
		, AtConfig::shader_path + "au_shadow_mapping.aufs")
	,m_ShadowMapDepth(AtConfig::shader_path + "au_shadow_mapping_depth.auvs"
		, AtConfig::shader_path + "au_shadow_mapping_depth.aufs")
{}
Shadow::~Shadow()
{}

void Shadow::Start()
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	float planeVertices[] = {
		// positions            // normals         // texcoords
		25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};
	// plane VAO
	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

	woodTexture = loadTexture("Resource/texture/wood.jpg");
	// configure depth map FBO
	// -----------------------
	
	INSTANCE(ShadowTest).BindDepathMap();

	m_ShadowMap.Use();
	m_ShadowMap.SetInt("diffuseTexture", 0);
	m_ShadowMap.SetInt("shadowMap", 1);

	lightPos = glm::vec3(-2.0f, 4.0f, -1.0f);
}
void Shadow::Draw(Camera* camera)
{
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	
	
	glViewport(0, 0, INSTANCE(ShadowTest).shadowWidth, INSTANCE(ShadowTest).shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, INSTANCE(ShadowTest).depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);

	// render scene from light's point of view
	m_ShadowMapDepth.Use();
	m_ShadowMapDepth.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
	/////////////////////////////////////////////////////////////////////////////////////////////
	renderScene(m_ShadowMapDepth);
	/////////////////////////////////////////////////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// reset viewport
	RectInt t = INSTANCE(GLWindow).GetWindowRectInt();
	glViewport(0, 0, t.width, t.height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 2. render scene as normal using the generated depth/shadow map  
	// --------------------------------------------------------------
	glViewport(0, 0, t.width, t.height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/////////////////////////////////////////////////////////////////////////////////////////////
	m_ShadowMap.Use();
	glm::mat4 projection = camera->GetProjectionMatrix();
	glm::mat4 view = camera->GetViewMatrix();
	m_ShadowMap.SetMat4("projection", projection);
	m_ShadowMap.SetMat4("view", view);
	// set light uniforms
	m_ShadowMap.SetVec3("viewPos", camera->GetPosition());
	m_ShadowMap.SetVec3("lightPos", lightPos);
	m_ShadowMap.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, INSTANCE(ShadowTest).depthMap);
	renderScene(m_ShadowMap);
	/////////////////////////////////////////////////////////////////////////////////////////////
}

// renders the 3D scene
// --------------------
void Shadow::renderScene(const Shader &shader)
{
	// floor
	glm::mat4 model;
	shader.SetMat4("model", model);
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// cubes
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	renderCube();
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	renderCube();
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.SetMat4("model", model);
	renderCube();
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void Shadow::renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(shadow_vertices), shadow_vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
unsigned int Shadow::loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}


AUTO_END