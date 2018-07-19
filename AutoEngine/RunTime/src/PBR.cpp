#include "PBR.h"
#include "AtConfig.h"
#include "Camera.h"
#include "BaseMesh.h"
#include "RenderManager.h"
AUTO_BEGIN
glm::vec3 p_lightPositions[] = {
	glm::vec3(-10.0f,  10.0f, 10.0f),
	glm::vec3(10.0f,  10.0f, 10.0f),
	glm::vec3(-10.0f, -10.0f, 10.0f),
	glm::vec3(10.0f, -10.0f, 10.0f),
};
glm::vec3 p_lightColors[] = {
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f)
};

PBR::PBR()
	:m_shader(AtConfig::shader_path + "au_pbr.auvs"
		, AtConfig::shader_path + "au_pbr.aufs")
{
}


PBR::~PBR()
{
}
void PBR::Start()
{
	m_shader.Use();
	m_shader.SetVec3("albedo", 0.5f, 0.0f, 0.0f);
	m_shader.SetFloat("ao", 1.0f);
}
void PBR::Draw()
{
	glm::mat4 projection = INSTANCE(RenderManager).GetCurrentCamera().GetProjectionMatrix();
	m_shader.Use();
	m_shader.SetMat4("projection", projection);
	glm::mat4 view = INSTANCE(RenderManager).GetCurrentCamera().GetViewMatrix();
	m_shader.SetMat4("view", view);
	m_shader.SetVec3("camPos", INSTANCE(RenderManager).GetCurrentCamera().GetPosition());

	glm::mat4 model;

	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;
	for (unsigned int row = 0; row < nrRows; ++row)
	{
		m_shader.SetFloat("metallic", (float)row / (float)nrRows);
		for (unsigned int col = 0; col < nrColumns; ++col)
		{
			// we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
			// on direct lighting.
			m_shader.SetFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

			model = glm::mat4();
			model = glm::translate(model, glm::vec3(
				(float)(col - (nrColumns / 2)) * spacing,
				(float)(row - (nrRows / 2)) * spacing,
				0.0f
			));
			m_shader.SetMat4("model", model);
			renderSphere(&sphereVAO,&indexCount);
		}
	}

	// render light source (simply re-render sphere at light positions)
	// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
	// keeps the codeprint small.
	for (unsigned int i = 0; i < sizeof(p_lightPositions) / sizeof(p_lightPositions[0]); ++i)
	{
		glm::vec3 newPos = p_lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
		newPos = p_lightPositions[i];
		m_shader.SetVec3("lightPositions[" + std::to_string(i) + "]", newPos);
		m_shader.SetVec3("lightColors[" + std::to_string(i) + "]", p_lightColors[i]);

		model = glm::mat4();
		model = glm::translate(model, newPos);
		model = glm::scale(model, glm::vec3(0.5f));
		m_shader.SetMat4("model", model);
		renderSphere(&sphereVAO, &indexCount);
	}
}
AUTO_END