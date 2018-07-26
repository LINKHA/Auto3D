#include "ShadowPoint.h"
#include "AtConfig.h"
#include "stb_image.h"
#include "ShadowTest.h"
#include "GameWindow.h"
#include "Camera.h"
#include "VertexData.h"
#include "LoadResource.h"
#include "Renderer.h"
AUTO_BEGIN
ShadowPoint::ShadowPoint()
	:m_ShadowMap(AtConfig::shader_path + "au_point_shadows.auvs"
		, AtConfig::shader_path + "au_point_shadows.aufs")
	, m_ShadowMapDepth(AtConfig::shader_path + "au_point_shadows_depth.auvs"
		, AtConfig::shader_path + "au_point_shadows_depth.aufs"
		, AtConfig::shader_path + "au_point_shadows_depth.augs")
{}
ShadowPoint::~ShadowPoint()
{}

void ShadowPoint::Start()
{
	// configure depth map FBO
	// -----------------------
	woodTexture = LocalTextureLoad("Resource/texture/wood.jpg");

	INSTANCE(ShadowTest).BindPointDepathMap();

	// shader configuration
	// --------------------
	m_ShadowMap.Use();
	m_ShadowMap.SetInt("diffuseTexture", 0);
	m_ShadowMap.SetInt("depthMap", 1);
	lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
}
void ShadowPoint::Draw()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int shadowWidth = INSTANCE(ShadowTest).shadowWidth;
	int shadowHeight = INSTANCE(ShadowTest).shadowHeight;
	float near_plane = 1.0f;
	float far_plane = 25.0f;
	glm::mat4 shadowProj = glm::perspective(90.0f, (float)shadowWidth / (float)shadowHeight, near_plane, far_plane);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	glViewport(0, 0, shadowWidth, shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, INSTANCE(ShadowTest).depthPointMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_ShadowMapDepth.Use();
	for (unsigned int i = 0; i < 6; ++i)
		m_ShadowMapDepth.SetMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	m_ShadowMapDepth.SetFloat("far_plane", far_plane);
	m_ShadowMapDepth.SetVec3("lightPos", lightPos);
	renderScene(m_ShadowMapDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. render scene as normal 
	// -------------------------
	RectInt t = GetSubSystem<GameWindow>()->GetWindowRectInt();
	glViewport(0, 0, t.width, t.height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_ShadowMap.Use();
	glm::mat4 projection = GetSubSystem<Renderer>()->GetCurrentCamera().GetProjectionMatrix();
	glm::mat4 view = GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix();
	m_ShadowMap.SetMat4("projection", projection);
	m_ShadowMap.SetMat4("view", view);
	// set lighting uniforms
	m_ShadowMap.SetVec3("lightPos", lightPos);
	m_ShadowMap.SetVec3("viewPos", GetSubSystem<Renderer>()->GetCurrentCamera().GetPosition());
	m_ShadowMap.SetInt("shadows", true); // enable/disable shadows by pressing 'SPACE'
	m_ShadowMap.SetFloat("far_plane", far_plane);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, INSTANCE(ShadowTest).depthPointmap);
	renderScene(m_ShadowMap);
}
// renders the 3D scene
// --------------------
void ShadowPoint::renderScene(const Shader& shader)
{
	// room cube
	glm::mat4 model;
	model = glm::scale(model, glm::vec3(5.0f));
	shader.SetMat4("model", model);
	glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
	shader.SetInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	renderCube();
	shader.SetInt("reverse_normals", 0); // and of course disable it
	glEnable(GL_CULL_FACE);
	// cubes
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	renderCube();
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.75f));
	shader.SetMat4("model", model);
	renderCube();
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	renderCube();
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	renderCube();
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.75f));
	shader.SetMat4("model", model);
	renderCube();
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void ShadowPoint::renderCube()
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
AUTO_END