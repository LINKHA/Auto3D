#include "Mesh.h"
#include "Renderer.h"
#include "LightManager.h"
#include "Configs.h"
namespace Auto3D {


LightManager& lights = INSTANCE(LightManager);

Mesh::Mesh(Ambient* ambient)
	:Super(ambient)
	, _shader(Shader(shader_path + "au_light_map_model_loading.auvs"
		, shader_path + "au_light_map_model_loading.aufs"))
	, _isUserShader(false)
{
	_meshPath.ptr = "../Resource/object/base/Cube.FBX";

}
Mesh::Mesh(Ambient* ambient,char* meshPath)
	: Super(ambient)
	, _isUserShader(false)
{
	_meshPath.ptr = meshPath;
	
}
Mesh::Mesh(Ambient* ambient,char* meshPath, const Shader& shader)
	: Super(ambient)
	, _shader(shader)
	, _isUserShader(true)
{
	_meshPath.ptr = meshPath;
}
Mesh::~Mesh()
{
}
void Mesh::Start()
{
	if (_material.isTexture)
	{
		_shader = Shader(shader_path + "au_light_map_model_loading.auvs"
			, shader_path + "au_light_map_model_loading.aufs");
	}
	else
	{
		_shader = Shader(shader_path + "au_light_model_loading.auvs"
			, shader_path + "au_light_model_loading.aufs");
	}
	//////////////////////////////////////////////////////////////////////////
	_model = LocalModelLoad(_meshPath.ptr);
}
void Mesh::Draw()
{
	_shader.Use();

	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;

	GLApply();

	if (GetGameObjectPtr())		//if gameObject not empty
		modelMat = GetGameObject().GetComponent(Transform).GetTransformMat();
	else
		modelMat = Matrix4x4::identity;
	viewMat = GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix();
	projectionMat = GetSubSystem<Renderer>()->GetCurrentCamera().GetProjectionMatrix();

	_shader.SetMat4("model", modelMat);
	_shader.SetMat4("view", viewMat);
	_shader.SetMat4("projection", projectionMat);
	_shader.SetVec3("viewPos", GetSubSystem<Renderer>()->GetCurrentCamera().GetPosition());
	if (!_isUserShader)
	{
		drawMaterial();
		drawLight();
	}
	_model->Draw(_shader);

	GLOriginal();
}

void Mesh::drawMaterial()
{

	// shader configuration
	// --------------------
	if (_material.isTexture)
	{
		_shader.SetInt("material.color", 0);
	}
	else
	{
		_shader.SetVec3("material.color", _material.color.r, _material.color.g, _material.color.b);
	}
	_shader.SetVec3("material.diffuse", _material.diffuse);
	_shader.SetVec3("material.specular", _material.specular);
	_shader.SetFloat("material.shininess", _material.shininess);
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
		_shader.SetVec3("lightPos", ligthtPosition);

		switch (t->GetType())
		{
		case kDirectional:
			if (dir >= 4)break;
			_shader.SetVec3("dirLight[" + to_string(dir) + "].color", t->color);
			_shader.SetVec3("dirLight[" + to_string(dir) + "].direction", t->direction);
			_shader.SetVec3("dirLight[" + to_string(dir) + "].ambient", t->ambient);
			_shader.SetVec3("dirLight[" + to_string(dir) + "].diffuse", t->diffuse);
			_shader.SetVec3("dirLight[" + to_string(dir) + "].specular", t->specular);
			dir++;
			break;
		case kPoint:
			if (point >= 8)break;
			_shader.SetVec3("pointLight[" + to_string(point) + "].color", t->color);
			_shader.SetVec3("pointLight[" + to_string(point) + "].position", ligthtPosition);
			_shader.SetFloat("pointLight[" + to_string(point) + "].constant", t->constant);
			_shader.SetFloat("pointLight[" + to_string(point) + "].linear", t->linear);
			_shader.SetFloat("pointLight[" + to_string(point) + "].quadratic", t->quadratic);
			_shader.SetVec3("pointLight[" + to_string(point) + "].ambient", t->ambient);
			_shader.SetVec3("pointLight[" + to_string(point) + "].diffuse", t->diffuse);
			_shader.SetVec3("pointLight[" + to_string(point) + "].specular", t->specular);
			point++;
			break;
		case kSpot:

			if (spot >= 4)break;
			_shader.SetVec3("spotLight[" + to_string(spot) + "].color", t->color);
			_shader.SetVec3("spotLight[" + to_string(spot) + "].position", ligthtPosition);
			_shader.SetVec3("spotLight[" + to_string(spot) + "].direction", t->direction);
			_shader.SetFloat("spotLight[" + to_string(spot) + "].cutOff", t->cutOff);
			_shader.SetFloat("spotLight[" + to_string(spot) + "].outerCutOff", t->outerCutOff);
			_shader.SetFloat("spotLight[" + to_string(spot) + "].constant", t->constant);
			_shader.SetFloat("spotLight[" + to_string(spot) + "].linear", t->linear);
			_shader.SetFloat("spotLight[" + to_string(spot) + "].quadratic", t->quadratic);
			_shader.SetVec3("spotLight[" + to_string(spot) + "].ambient", t->ambient);
			_shader.SetVec3("spotLight[" + to_string(spot) + "].diffuse", t->diffuse);
			_shader.SetVec3("spotLight[" + to_string(spot) + "].specular", t->specular);
			spot++;
			break;
		default:
			WarningString("Fail to set light!");
		}
	}
	_shader.SetBool("dirBool", (bool)dir);
	_shader.SetBool("pointBool", (bool)point);
	_shader.SetBool("spotBool", (bool)spot);

	_shader.SetInt("dirNum", dir);
	_shader.SetInt("pointNum", point);
	_shader.SetInt("spotNum", spot);
}
}