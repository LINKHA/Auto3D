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
	int dir = 0;
	int point = 0;
	int spot = 0;
	for (auto it = t_lights.begin(); it != t_lights.end(); it++)
	{
		Light * t = *it;
		Vector3 ligthtPosition= t->GetGameObject().GetComponent(Transform).GetPosition();
		m_shader.SetVec3("lightPos", ligthtPosition);

		switch (t->GetType())
		{
			case Directional:
				if (dir >= 4)break;
				m_shader.SetVec3("dirLight[" + to_string(dir) + "].color", t->color);
				m_shader.SetVec3("dirLight[" + to_string(dir) + "].direction", t->direction);
				m_shader.SetVec3("dirLight[" + to_string(dir) + "].ambient", t->ambient);
				m_shader.SetVec3("dirLight[" + to_string(dir) + "].diffuse", t->diffuse);
				m_shader.SetVec3("dirLight[" + to_string(dir) + "].specular", t->specular);
				dir++;
				break;
			case Point:
				if (point >= 8)break;
				m_shader.SetVec3("pointLight[" + to_string( point) + "].color", t->color);
				m_shader.SetVec3("pointLight[" + to_string(point) + "].position", ligthtPosition);
				m_shader.SetFloat("pointLight[" + to_string(point) + "].constant", t->constant);
				m_shader.SetFloat("pointLight[" + to_string(point) + "].linear", t->linear);
				m_shader.SetFloat("pointLight[" + to_string(point) + "].quadratic", t->quadratic);
				m_shader.SetVec3("pointLight[" + to_string(point) + "].ambient", t->ambient);
				m_shader.SetVec3("pointLight[" + to_string(point) + "].diffuse", t->diffuse);
				m_shader.SetVec3("pointLight[" + to_string(point) + "].specular", t->specular);
				point++;
				break;
			case Spot:
				
				if (spot >= 4)break;
				m_shader.SetVec3("spotLight[" + to_string(spot) + "].color", t->color);
				m_shader.SetVec3("spotLight[" + to_string(spot) + "].position", ligthtPosition);
				m_shader.SetVec3("spotLight[" + to_string(spot) + "].direction", t->direction);
				m_shader.SetFloat("spotLight[" + to_string(spot) + "].cutOff", t->cutOff);
				m_shader.SetFloat("spotLight[" + to_string(spot) + "].outerCutOff", t->outerCutOff);
				m_shader.SetFloat("spotLight[" + to_string(spot) + "].constant", t->constant);
				m_shader.SetFloat("spotLight[" + to_string(spot) + "].linear", t->linear);
				m_shader.SetFloat("spotLight[" + to_string(spot) + "].quadratic", t->quadratic);
				m_shader.SetVec3("spotLight[" + to_string(spot) + "].ambient", t->ambient);
				m_shader.SetVec3("spotLight[" + to_string(spot) + "].diffuse", t->diffuse);
				m_shader.SetVec3("spotLight[" + to_string(spot) + "].specular", t->specular);
				spot++;
				break;
			default:
				WarningString("Fail to set light!");
		}
	}
	m_shader.SetBool("dirBool", (bool)dir);
	m_shader.SetBool("pointBool", (bool)point);
	m_shader.SetBool("spotBool", (bool)spot);

	m_shader.SetInt("dirNum", dir);
	m_shader.SetInt("pointNum", point);
	m_shader.SetInt("spotNum", spot);
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