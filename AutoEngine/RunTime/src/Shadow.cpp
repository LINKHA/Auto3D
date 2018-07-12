#include "Shadow.h"
#include "AtConfig.h"
#include "GLWindow.h"
#include "Camera.h"
#include "ShadowTest.h"
#include "stb_image.h"
#include "VertexData.h"
#include "LoadResource.h"
AUTO_BEGIN


#if SHADOW_DEBUG
Shadow::Shadow(int i)
	:m_ShadowMap(AtConfig::shader_path + "au_shadow_mapping.auvs"
		, AtConfig::shader_path + "au_shadow_mapping.aufs")
	, m_ShadowMapDepth(AtConfig::shader_path + "au_shadow_mapping_depth.auvs"
		, AtConfig::shader_path + "au_shadow_mapping_depth.aufs")
	,k(i)
{}
Shadow::~Shadow()
{}

void Shadow::Start()
{

	woodTexture = LocalTextureLoad("Resource/texture/wood.jpg");
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

	// cubes
	switch (k)
	{
	case 0:
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		shader.SetMat4("model", model);
		renderCube();
		break;
	case 1:
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.5f));
		shader.SetMat4("model", model);
		renderCube();
	case 2:
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.25));
		shader.SetMat4("model", model);
		renderCube();
	}
	
	

}


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
#else
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

	woodTexture = LocalTextureLoad("Resource/texture/wood.jpg");
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
	glBindVertexArray(0);
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

#endif

AUTO_END