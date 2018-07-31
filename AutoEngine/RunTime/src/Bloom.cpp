#include "Bloom.h"
#include "AtConfig.h"
#include "LoadResource.h"
#include "Graphics.h"
#include "Camera.h"
#include "Renderer.h"
#include "BaseMesh.h"
namespace Auto3D {

Bloom::Bloom(Ambient* ambient)
	: Component(ambient)
	, m_shader(Shader(AtConfig::shader_path + "au_bloom.auvs"
		, AtConfig::shader_path + "au_bloom.aufs"))
	, m_shaderLight(Shader(AtConfig::shader_path + "au_bloom.auvs"
		, AtConfig::shader_path + "au_bloom_light_box.aufs"))
	, m_shaderBlur(Shader(AtConfig::shader_path + "au_bloom_blur.auvs"
		, AtConfig::shader_path + "au_bloom_blur.aufs"))
	, m_shaderBloomFinal(Shader(AtConfig::shader_path + "au_bloom_final.auvs"
		, AtConfig::shader_path + "au_bloom_final.aufs"))
{
}


Bloom::~Bloom()
{
}

void Bloom::Start()
{
	woodTexture = LocalTextureLoad("../Resource/texture/wood.jpg");
	containerTexture = LocalTextureLoad("../Resource/texture/wood.jpg");

	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

	glGenTextures(2, colorBuffers);
	RectInt t = GetSubSystem<Graphics>()->GetWindowRectInt();
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, t.width, t.height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}

	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, t.width, t.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	glDrawBuffers(2, attachments);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, t.width, t.height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}

	// lighting info
	// -------------
	// positions
	lightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
	lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
	lightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
	lightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));
	// colors
	lightColors.push_back(glm::vec3(2.0f, 2.0f, 2.0f));
	lightColors.push_back(glm::vec3(1.5f, 0.0f, 0.0f));
	lightColors.push_back(glm::vec3(0.0f, 0.0f, 1.5f));
	lightColors.push_back(glm::vec3(0.0f, 1.5f, 0.0f));


	// shader configuration
	// --------------------
	m_shader.Use();
	m_shader.SetInt("diffuseTexture", 0);
	m_shaderBlur.Use();
	m_shaderBlur.SetInt("image", 0);
	m_shaderBloomFinal.Use();
	m_shaderBloomFinal.SetInt("scene", 0);
	m_shaderBloomFinal.SetInt("bloomBlur", 1);
}
void Bloom::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 projection = GetSubSystem<Renderer>()->GetCurrentCamera().GetProjectionMatrix();
	glm::mat4 view = GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix();
	glm::mat4 model;
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
	// create one large cube that acts as the floor
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
	model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
	m_shader.SetMat4("model", model);
	m_shader.SetMat4("model", model);
	renderCube(&cubeVAO,&cubeVBO);
	// then create multiple cubes as the scenery
	glBindTexture(GL_TEXTURE_2D, containerTexture);
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	m_shader.SetMat4("model", model);
	renderCube(&cubeVAO, &cubeVBO);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	m_shader.SetMat4("model", model);
	renderCube(&cubeVAO, &cubeVBO);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	m_shader.SetMat4("model", model);
	renderCube(&cubeVAO, &cubeVBO);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
	model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(1.25));
	m_shader.SetMat4("model", model);
	renderCube(&cubeVAO, &cubeVBO);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
	model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	m_shader.SetMat4("model", model);
	renderCube(&cubeVAO, &cubeVBO);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	m_shader.SetMat4("model", model);
	renderCube(&cubeVAO, &cubeVBO);

	// finally show all the light sources as bright cubes
	m_shaderLight.Use();
	m_shaderLight.SetMat4("projection", projection);
	m_shaderLight.SetMat4("view", view);

	for (unsigned int i = 0; i < lightPositions.size(); i++)
	{
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(lightPositions[i]));
		model = glm::scale(model, glm::vec3(0.25f));
		m_shaderLight.SetMat4("model", model);
		m_shaderLight.SetVec3("lightColor", lightColors[i]);
		renderCube(&cubeVAO, &cubeVBO);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. blur bright fragments with two-pass Gaussian Blur 
	// --------------------------------------------------
	bool horizontal = true, first_iteration = true;
	unsigned int amount = 10;
	m_shaderBlur.Use();
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		m_shaderBlur.SetInt("horizontal", horizontal);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
		renderQuad(&quadVAO,&quadVBO);
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
	// --------------------------------------------------------------------------------------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_shaderBloomFinal.Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
	m_shaderBloomFinal.SetInt("bloom", bloom);
	m_shaderBloomFinal.SetFloat("exposure", exposure);
	renderQuad(&quadVAO, &quadVBO);

}
}