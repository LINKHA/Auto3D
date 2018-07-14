#include "Shadow.h"
#include "AtConfig.h"
#include "GLWindow.h"
#include "Camera.h"
#include "ShadowTest.h"
#include "stb_image.h"
#include "VertexData.h"
#include "LoadResource.h"
AUTO_BEGIN

Shadow::Shadow()
	: m_ShadowMapDepth(AtConfig::shader_path + "au_shadow_mapping_depth.auvs"
		, AtConfig::shader_path + "au_shadow_mapping_depth.aufs")
{}
Shadow::~Shadow()
{}

void Shadow::Start()
{
	INSTANCE(ShadowTest).BindDepathMap();

	/////////////////////////////////////////////////////////////////////////////////////////////
	mesh = new MeshShadow(0);
	mesh1 = new MeshShadow(1);
	mesh2 = new MeshShadow(2);
	mesh3 = new MeshShadow(3);

	mesh->Start();
	mesh1->Start();
	mesh2->Start();
	mesh3->Start();

	woodTexture = LocalTextureLoad("Resource/texture/wood.jpg");

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

	m_ShadowMapDepth.Use();
	m_ShadowMapDepth.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
	mesh->Draw(m_ShadowMapDepth);
	mesh1->Draw(m_ShadowMapDepth);
	mesh2->Draw(m_ShadowMapDepth);
	mesh3->Draw(m_ShadowMapDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	RectInt t = INSTANCE(GLWindow).GetWindowRectInt();
	glViewport(0, 0, t.width, t.height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, t.width, t.height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mesh->Draw2(camera, lightPos, lightSpaceMatrix);
	mesh1->Draw2(camera, lightPos, lightSpaceMatrix);
	mesh2->Draw2(camera, lightPos, lightSpaceMatrix);
	mesh3->Draw2(camera, lightPos, lightSpaceMatrix);
	
}


AUTO_END