#include "Mesh.h"

AUTO_BEGIN
Mesh::Mesh()
	: m_meshPath("Resource/object/nanosuit/nanosuit.obj")
	, m_shader(Shader(AtConfig::shader_path + "au_model_loading.auvs", AtConfig::shader_path + "au_model_loading.aufs"))
	, m_transform(Transform())
{
}
Mesh::Mesh(_String meshPath, const Transform& transform )
	: m_meshPath(meshPath)
	, m_shader(Shader(AtConfig::shader_path + "au_model_loading.auvs", AtConfig::shader_path + "au_model_loading.aufs"))
	, m_transform(transform)
{
}
Mesh::Mesh(_String meshPath, const Shader& shader, const Transform& transform )
	: m_meshPath(meshPath)
	, m_shader(shader)
	, m_transform(transform)
{
}
Mesh::~Mesh()
{
}
void Mesh::Draw()
{
	model = LocalModelLoad((char*)m_meshPath.data());
}
void Mesh::PushToRunloop()
{
	m_shader.Use();

	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;
	
	modelMat = m_transform.GetTransformMat();
	viewMat = Application::Instance().m_camera.GetViewMatrix();
	projectionMat = glm::perspective(Application::Instance().m_camera.Zoom, (float)800 / (float)600, 0.1f, 100.0f);
	
	m_shader.SetMat4("projection", projectionMat);
	m_shader.SetMat4("view", viewMat);
	m_shader.SetMat4("model", modelMat);
	m_shader.SetVec4("ourColor", 0.0f, 0.0f, 0.5f, 1.0f);
	model.Draw(m_shader);
	m_transform.Identity();
}

AUTO_END