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
void Mesh::draw()
{
	model = localModelLoad((char*)m_meshPath.data());
}
void Mesh::pushToRunloop()
{
	m_shader.use();

	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;
	
	modelMat = m_transform.getTransformMat();
	viewMat = Application::Instance().m_camera.GetViewMatrix();
	projectionMat = glm::perspective(Application::Instance().m_camera.Zoom, (float)800 / (float)600, 0.1f, 100.0f);
	
	m_shader.setMat4("projection", projectionMat);
	m_shader.setMat4("view", viewMat);
	m_shader.setMat4("model", modelMat);

	model.Draw(m_shader);
	//m_transform.identity();
}

AUTO_END