#include "Mesh.h"

AUTO_BEGIN
Mesh::Mesh()
	: m_meshPath("Resource/object/nanosuit/nanosuit.obj")
	, m_shader(Shader(AtConfig::shader_path + "au_model_loading.auvs", AtConfig::shader_path + "au_model_loading.aufs"))
{
}

Mesh::Mesh(_String meshPath)
	: m_meshPath(meshPath)
	, m_shader(Shader(AtConfig::shader_path + "au_model_loading.auvs", AtConfig::shader_path + "au_model_loading.aufs"))
{
}
Mesh::Mesh(_String meshPath, const Shader& shader)
	: m_meshPath(meshPath)
	, m_shader(shader)
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
	if (GetGameObject().GetTransformPtr())
		modelMat = GetGameObject().GetTransformPtr()->GetTransformMat();
	else
		modelMat = Matrix4x4::identity;
	viewMat = Application::Instance().m_camera.GetViewMatrix();
	projectionMat = glm::perspective(Application::Instance().m_camera.Zoom, (float)800 / (float)600, 0.1f, 100.0f);
	
	m_shader.SetMat4("projection", projectionMat);
	m_shader.SetMat4("view", viewMat);
	m_shader.SetMat4("model", modelMat);
	m_shader.SetVec4("ourColor", 0.0f, 0.0f, 0.5f, 1.0f);
	model.Draw(m_shader);
}

AUTO_END