#include "Mesh.h"
#include "RenderManager.h"
#include "LightManager.h"
AUTO_BEGIN


LightManager& lights = INSTANCE(LightManager);

Mesh::Mesh()
	: m_shader(Shader(AtConfig::shader_path + "au_light_map_model_loading.auvs"
		, AtConfig::shader_path + "au_light_map_model_loading.aufs"))
	, m_userShader(false)
{
	m_meshPath.ptr = "Resource/object/base/Cube.FBX";

}
Mesh::Mesh(char* meshPath)
	: m_userShader(false)
{
	m_meshPath.ptr = meshPath;
	
}
Mesh::Mesh(char* meshPath, const Shader& shader)
	: m_shader(shader)
	, m_userShader(true)
{
	m_meshPath.ptr = meshPath;
}
Mesh::~Mesh()
{
}
void Mesh::Start()
{
	if (m_Material.isTexture)
	{
		m_shader = Shader(AtConfig::shader_path + "au_light_map_model_loading.auvs"
			, AtConfig::shader_path + "au_light_map_model_loading.aufs");
	}
	else
	{
		m_shader = Shader(AtConfig::shader_path + "au_light_model_loading.auvs"
			, AtConfig::shader_path + "au_light_model_loading.aufs");
	}
	//////////////////////////////////////////////////////////////////////////
	m_Model = LocalModelLoad(m_meshPath.ptr);
}
void Mesh::Draw()
{
	if (INSTANCE(RenderManager).GetCurrentCameraPtr() == nullptr)
	{
		WarningString("Fail to find camera");
		return;
	}
	m_shader.Use();

	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;

	GLApply();

	if (GetGameObjectPtr())		//if gameObject not empty
		modelMat = GetGameObject().GetComponent(Transform).GetTransformMat();
	else
		modelMat = Matrix4x4::identity;
	viewMat = INSTANCE(RenderManager).GetCurrentCamera().GetViewMatrix();
	projectionMat = INSTANCE(RenderManager).GetCurrentCamera().GetProjectionMatrix();

	m_shader.SetMat4("model", modelMat);
	m_shader.SetMat4("view", viewMat);
	m_shader.SetMat4("projection", projectionMat);
	m_shader.SetVec3("viewPos", INSTANCE(RenderManager).GetCurrentCamera().GetPosition());
	if (!m_userShader)
	{
		drawMaterial();
		drawLight();
	}
	m_Model.Draw(m_shader);

	GLOriginal();
}

void Mesh::drawMaterial()
{

	// shader configuration
	// --------------------
	if (m_Material.isTexture)
	{
		m_shader.SetInt("material.color", 0);
	}
	else
	{
		m_shader.SetVec3("material.color", m_Material.color.r, m_Material.color.g, m_Material.color.b);
	}
	m_shader.SetVec3("material.diffuse", m_Material.diffuse);
	m_shader.SetVec3("material.specular", m_Material.specular);
	m_shader.SetFloat("material.shininess", m_Material.shininess);
}

void Mesh::drawLight()
{
	auto& t_lights = lights.GetAllLights();
	int dir = 0;
	int point = 0;
	int spot = 0;
	for (auto it = t_lights.begin(); it != t_lights.end(); it++)
	{
		Light * t = *it;
		Vector3 ligthtPosition = t->GetGameObject().GetComponent(Transform).GetPosition();
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
			m_shader.SetVec3("pointLight[" + to_string(point) + "].color", t->color);
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
AUTO_END