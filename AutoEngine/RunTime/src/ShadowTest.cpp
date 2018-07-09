#include "ShadowTest.h"
#include "AtConfig.h"
#include "Camera.h"
AUTO_BEGIN

ShadowTest::ShadowTest()
	:m_shadowShader(AtConfig::shader_path + "au_shadow_mapping_depth.auvs", AtConfig::shader_path + "au_shadow_mapping_depth.aufs")
{
	shadowWidth = 1024;
	shadowHeight = 1024;
}


ShadowTest::~ShadowTest()
{
}
void ShadowTest::Start()
{
	glGenFramebuffers(1, &m_depthMapFBO);
	// create depth texture
	glGenTextures(1, &m_depthMap);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void ShadowTest::UpdateStart(Camera * camera)
{
	float nearPlane = 1.0f;
	float farPlane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f)
		, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	m_shadowShader.Use();
	m_shadowShader.SetMat4("lightSpaceMatrix", lightProjection * lightView);




	// reset view
	glViewport(0, 0, shadowWidth, shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	
}
void ShadowTest::UpdateEnd(Camera * camera)
{

}
AUTO_END