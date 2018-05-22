#include "Mesh.h"

AUTO_BEGIN
#if  MESH_DEBUG
Mesh::Mesh()
	: m_meshPath("Resource/object/nanosuit/nanosuit.obj")
	, m_shader(Shader(AtConfig::shader_path + "au_model_loading.auvs", AtConfig::shader_path + "au_model_loading.aufs"))
{
}

Mesh::Mesh(_String meshPath)
	: m_meshPath(meshPath)
	, m_shader(Shader(AtConfig::shader_path + "au_text_model_loading.auvs", AtConfig::shader_path + "au_text_model_loading.aufs"))
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
void Mesh::Start()
{
	m_Model = LocalModelLoad((char*)m_meshPath.data());
}
void Mesh::Update()
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
	m_shader.SetMat4("model", modelMat);
	m_shader.SetMat4("view", viewMat);
	m_shader.SetMat4("projection", projectionMat);

	m_shader.SetVec3("ourColor", m_Color.r, m_Color.g, m_Color.b);
	m_shader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
	m_shader.SetVec3("lightPos", 2.5f, 2.0f, 1.0f);
	m_shader.SetVec3("viewPos", Application::Instance().m_camera.Position);
	//std::cout << Application::Instance().m_camera.Position.x<<" "<< Application::Instance().m_camera.Position.y << " " << Application::Instance().m_camera.Position.z <<std::endl;
	m_Model.Draw(m_shader);
}
void Mesh::SetColor(const Color& color)
{
	m_Color.Set(color.r, color.g, color.b, color.a);
}

void Mesh::SetColor(const Vector3& vec)
{
	m_Color.Set(vec.x, vec.y, vec.z, 1.0f);
}
void Mesh::SetColor(float r, float g, float b, float a)
{
	m_Color.Set(r, g, b, a);
}
#else
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
void Mesh::Start()
{
	m_Model = LocalModelLoad((char*)m_meshPath.data());
}
void Mesh::Update()
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
	m_shader.SetMat4("model", modelMat);
	m_shader.SetMat4("view", viewMat);
	m_shader.SetMat4("projection", projectionMat);

	m_shader.SetVec4("ourColor", m_Color.r, m_Color.g, m_Color.b, m_Color.a);
	m_shader.SetVec4("lightColor", 1.0f, 1.0f, 1.0f, 1.0f);
	m_Model.Draw(m_shader);
}
void Mesh::SetColor(const Color& color)
{
	m_Color.Set(color.r, color.g, color.b, color.a);
}

void Mesh::SetColor(const Vector3& vec)
{
	m_Color.Set(vec.x, vec.y, vec.z, 1.0f);
}
void Mesh::SetColor(float r, float g, float b, float a)
{
	m_Color.Set(r, g, b, a);
}
#endif	//MESH_DEBUG
AUTO_END