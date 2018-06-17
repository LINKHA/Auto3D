#include "Mesh.h"
#include "RenderManager.h"
#include "LightManager.h"
AUTO_BEGIN

LightManager& lights = INSTANCE(LightManager);

#if MESH_DEBUG 
Mesh::Mesh()
	: m_meshPath("Resource/object/nanosuit/nanosuit.obj")
	, m_shader(Shader(AtConfig::shader_path + "au_test_light_model_loading.auvs", AtConfig::shader_path + "au_test_light_model_loading.aufs"))
{
}

Mesh::Mesh(_String meshPath)
	: m_meshPath(meshPath)
	, m_shader(Shader(AtConfig::shader_path + "au_test_light_model_loading.auvs", AtConfig::shader_path + "au_test_light_model_loading.aufs"))
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
void Mesh::Update(Camera * cam)
{
	if (cam == nullptr)
	{
		WarningString("Fail to find camera");
		return;
	}
	m_shader.Use();

	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;

	if (GetGameObjectPtr())		//if gameObject not empty
		modelMat = GetGameObject().GetComponent(Transform).GetTransformMat();
	else
		modelMat = Matrix4x4::identity;
	viewMat = cam->GetViewMatrix();

	RectInt rect = INSTANCE(GLWindow).GetWindowRectInt();
	projectionMat = glm::perspective(cam->Zoom,
		((float)rect.width * (float)cam->ViewRect.width) /
		((float)rect.height * (float)cam->ViewRect.height),
		cam->Near, cam->Far);

	m_shader.SetMat4("model", modelMat);
	m_shader.SetMat4("view", viewMat);
	m_shader.SetMat4("projection", projectionMat);

	//Loop Light
	

	m_shader.SetVec3("viewPos", cam->Position);
	
	drawMaterial();
	drawLight();

	m_Model.Draw(m_shader);
}

void Mesh::drawMaterial()
{
	m_shader.SetVec3("material.color", m_Material.color.r, m_Material.color.g, m_Material.color.b);
	m_shader.SetVec3("material.diffuse", m_Material.diffuse);
	m_shader.SetVec3("material.specular", m_Material.specular);
	m_shader.SetFloat("material.shininess", m_Material.shininess);
}

void Mesh::drawLight()
{
	auto& t_lights= lights.GetAllLights();
	for (auto it = t_lights.begin(); it != t_lights.end(); it++)
	{
		Light * t = *it;
		Vector3 ligthtPosition= t->GetGameObject().GetComponent(Transform).GetPosition();
		m_shader.SetVec3("lightPos", ligthtPosition);
		switch (t->GetType())
		{
			case Directional:
				m_shader.SetVec3("dirLight.color", t->color);
				m_shader.SetVec3("dirLight.direction", t->direction);
				m_shader.SetVec3("dirLight.ambient", t->ambient);
				m_shader.SetVec3("dirLight.diffuse", t->diffuse);
				m_shader.SetVec3("dirLight.specular", t->specular);
				break;
			case Point:
				m_shader.SetVec3("pointLight.color", t->color);
				m_shader.SetVec3("pointLight.position", ligthtPosition);
				m_shader.SetFloat("pointLight.constant", t->constant);
				m_shader.SetFloat("pointLight.linear", t->linear);
				m_shader.SetFloat("pointLight.quadratic", t->quadratic);
				m_shader.SetVec3("pointLight.ambient", t->ambient);
				m_shader.SetVec3("pointLight.diffuse", t->diffuse);
				m_shader.SetVec3("pointLight.specular", t->specular);
				break;
			case Spot:
				m_shader.SetVec3("spotLight.color", t->color);
				m_shader.SetVec3("spotLight.position", ligthtPosition);
				m_shader.SetVec3("spotLight.direction", t->direction);
				m_shader.SetFloat("spotLight.cutOff", t->cutOff);
				m_shader.SetFloat("spotLight.outerCutOff", t->outerCutOff);
				m_shader.SetFloat("spotLight.constant", t->constant);
				m_shader.SetFloat("spotLight.linear", t->linear);
				m_shader.SetFloat("spotLight.quadratic", t->quadratic);
				m_shader.SetVec3("spotLight.ambient", t->ambient);
				m_shader.SetVec3("spotLight.diffuse", t->diffuse);
				m_shader.SetVec3("spotLight.specular", t->specular);
				break;
			default:
				WarningString("Fail to set light!");
		}
	}
}

#else	//Light Model
Mesh::Mesh()
	: m_meshPath("Resource/object/nanosuit/nanosuit.obj")
	, m_shader(Shader(AtConfig::shader_path + "au_light_model_loading.auvs", AtConfig::shader_path + "au_light_model_loading.aufs"))
{
}

Mesh::Mesh(_String meshPath)
	: m_meshPath(meshPath)
	, m_shader(Shader(AtConfig::shader_path + "au_light_model_loading.auvs", AtConfig::shader_path + "au_light_model_loading.aufs"))
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
void Mesh::Update(Camera * cam)
{
	if (cam == nullptr)
	{
		WarningString("Fail to find camera");
		return;
	}
	m_shader.Use();

	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;

	if (GetGameObjectPtr())		//if gameObject not empty
		modelMat = GetGameObject().GetComponent(Transform).GetTransformMat();
	else
		modelMat = Matrix4x4::identity;
	viewMat = cam->GetViewMatrix();

	RectInt rect = INSTANCE(GLWindow).GetWindowRectInt();
	projectionMat = glm::perspective(cam->Zoom,
		((float)rect.width * (float)cam->ViewRect.width) /
		((float)rect.height * (float)cam->ViewRect.height),
		cam->Near, cam->Far);

	m_shader.SetMat4("model", modelMat);
	m_shader.SetMat4("view", viewMat);
	m_shader.SetMat4("projection", projectionMat);

	m_shader.SetVec3("ourColor", m_Color.r, m_Color.g, m_Color.b);
	m_shader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
	m_shader.SetVec3("lightPos", 1.2f, 1.0f, 2.0f);
	m_shader.SetVec3("viewPos", cam->Position);

	m_shader.SetVec3("material.ambient", m_Material.ambient);
	m_shader.SetVec3("material.diffuse", m_Material.diffuse);
	m_shader.SetVec3("material.specular", m_Material.specular);
	m_shader.SetFloat("material.shininess", m_Material.shininess);

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

void Mesh::SetMaterial(const Material & m)
{
	m_Material = m;
}
#endif
AUTO_END