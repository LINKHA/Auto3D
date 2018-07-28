#include "HDR.h"
#include "AtConfig.h"
#include "LoadResource.h"
#include "Graphics.h"
#include "Renderer.h"
#include "BaseMesh.h"
AUTO_BEGIN
HDR::HDR()
	: m_shader(AtConfig::shader_path + "au_lighting.auvs"
		, AtConfig::shader_path + "au_lighting.aufs")
	, m_hdrShader(AtConfig::shader_path + "au_hdr.auvs"
		, AtConfig::shader_path + "au_hdr.aufs")
{}

HDR::~HDR()
{}

void HDR::Start()
{
	woodTexture = LocalTextureLoad("../Resource/texture/wood.jpg"); 

	glGenFramebuffers(1, &hdrFBO);
	// create floating point color buffer
	
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);

	RectInt t = GetSubSystem<Graphics>()->GetWindowRectInt();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, t.width, t.height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// create depth buffer (renderbuffer)
	
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, t.width, t.height);
	// attach buffers
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		ErrorString("Framebuffer not complete");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// lighting info
	// -------------
	// positions
	
	lightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
	lightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
	lightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
	lightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));
	// colors
	
	lightColors.push_back(glm::vec3(300.0f, 300.0f, 300.0f));
	lightColors.push_back(glm::vec3(0.3f, 0.0f, 0.0f));
	lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.3f));
	lightColors.push_back(glm::vec3(0.0f, 0.3f, 0.0f));
	// shader configuration
	// --------------------
	m_shader.Use();
	m_shader.SetInt("diffuseTexture", 0);
	m_hdrShader.Use();
	m_hdrShader.SetInt("hdrBuffer", 0);
}

void HDR::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 projection = GetSubSystem<Renderer>()->GetCurrentCamera().GetProjectionMatrix();
	glm::mat4 view = GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix();
	m_shader.Use();
	m_shader.SetMat4("projection", projection);
	m_shader.SetMat4("view", view);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	// set lighting uniforms
	for (unsigned int i = 0; i < lightPositions.size(); i++)
	{
		m_shader.SetVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
		m_shader.SetVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
	}
	m_shader.SetVec3("viewPos", GetSubSystem<Renderer>()->GetCurrentCamera().GetPosition());
	// render tunnel
	glm::mat4 model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
	model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
	m_shader.SetMat4("model", model);
	m_shader.SetInt("inverse_normals", true);
	renderCube(&cubeVAO,&cubeVBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
	// --------------------------------------------------------------------------------------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_hdrShader.Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	m_hdrShader.SetInt("hdr", hdr);
	m_hdrShader.SetFloat("exposure", exposure);
	renderQuad(&quadVAO,&quadVBO);

	std::cout << "hdr: " << (hdr ? "on" : "off") << "| exposure: " << exposure << std::endl;
}

AUTO_END
