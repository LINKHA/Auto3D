#include "Mesh.h"
#include "Renderer.h"
#include "Light.h"
#include "Configs.h"
#include "ResourceSystem.h"
#include "DebugNew.h"
namespace Auto3D {

Mesh::Mesh(Ambient* ambient)
	:Super(ambient)
	, _shader(Shader(shader_path + "au_light_map_model_loading.auvs"
		, shader_path + "au_light_map_model_loading.aufs"))
	, _isUserShader(false)
{
	_material = new Material(_ambient);
	_modelPath = "../Resource/object/base/Cube.3DS";
}
Mesh::Mesh(Ambient* ambient,char* meshPath)
	: Super(ambient)
	, _shader(Shader(shader_path + "au_light_map_model_loading.auvs"
		, shader_path + "au_light_map_model_loading.aufs"))
	, _isUserShader(false)
{
	_material = new Material(_ambient);
	_modelPath = meshPath;
}
Mesh::Mesh(Ambient* ambient,char* meshPath, const Shader& shader)
	: Super(ambient)
	, _shader(shader)
	, _isUserShader(true)
{
	_material = new Material(_ambient);
	_modelPath = meshPath;
}
Mesh::~Mesh()
{
	UnloadOpaque(this);

	delete _material;
	_material = nullptr;
}

void Mesh::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<Mesh>(SCENE_ATTACH);
}


void Mesh::SetModel(char* modelPath)
{
	_modelPath = modelPath;
}

void Mesh::SetShader(const Shader& shader)
{
	_shader = shader;
}

void Mesh::Start()
{
	if (_isUserShader)
	{
		///User Shader code
	}
	else
	{
		if (_material->isTexture)
		{
			_shader = Shader(shader_path + "au_light_map_model_loading.auvs"
				, shader_path + "au_light_map_model_loading.aufs");
		}
		else
		{
			_shader = Shader(shader_path + "au_light_model_loading.auvs"
				, shader_path + "au_light_model_loading.aufs");
		}
	}
	_model = GetSubSystem<ResourceSystem>()->ModelLoad(_modelPath);
	RegisterOpaque(this);
}
void Mesh::Draw()
{
	_shader.Use();

	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;

	GLApply();

	if (GetNodePtr())		//if gameObject not empty
		modelMat = GetNode().GetComponent<Transform>()->GetTransformMat();
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
	if (_material->isTexture)
	{
		_shader.SetInt("material.color", 0);
	}
	else
	{
		_shader.SetVec3("material.color", _material->color.r, _material->color.g, _material->color.b);
	}
	_shader.SetVec3("material.diffuse", _material->diffuse);
	_shader.SetVec3("material.specular", _material->specular);
	_shader.SetFloat("material.shininess", _material->shininess);
}

void Mesh::drawLight()
{
	auto& lights = GetSubSystem<Renderer>()->GetLightContainer()->GetAllLights();
	int dir = 0;
	int point = 0;
	int spot = 0;
	for (auto it = lights.begin(); it != lights.end(); it++)
	{
		Light * t = *it;
		Vector3 ligthtPosition = t->GetNode().GetComponent<Transform>()->GetPosition();
		_shader.SetVec3("lightPos", ligthtPosition);

		switch (t->GetType())
		{
		case LightType::kDirectional:
			if (dir >= 4)break;
			_shader.SetVec3("dirLight[" + KhSTL::ToString(dir) + "].color", t->GetColorToVec());
			_shader.SetVec3("dirLight[" + KhSTL::ToString(dir) + "].direction", t->GetDirection());
			_shader.SetVec3("dirLight[" + KhSTL::ToString(dir) + "].ambient", t->ambient);
			_shader.SetVec3("dirLight[" + KhSTL::ToString(dir) + "].diffuse", t->diffuse);
			_shader.SetVec3("dirLight[" + KhSTL::ToString(dir) + "].specular", t->specular);
			dir++;
			break;
		case LightType::kPoint:
			if (point >= 8)break;
			_shader.SetVec3("pointLight[" + KhSTL::ToString(point) + "].color", t->GetColorToVec());
			_shader.SetVec3("pointLight[" + KhSTL::ToString(point) + "].position", ligthtPosition);
			_shader.SetFloat("pointLight[" + KhSTL::ToString(point) + "].constant", t->constant);
			_shader.SetFloat("pointLight[" + KhSTL::ToString(point) + "].linear", t->linear);
			_shader.SetFloat("pointLight[" + KhSTL::ToString(point) + "].quadratic", t->quadratic);
			_shader.SetVec3("pointLight[" + KhSTL::ToString(point) + "].ambient", t->ambient);
			_shader.SetVec3("pointLight[" + KhSTL::ToString(point) + "].diffuse", t->diffuse);
			_shader.SetVec3("pointLight[" + KhSTL::ToString(point) + "].specular", t->specular);
			point++;
			break;
		case LightType::kSpot:

			if (spot >= 4)break;
			_shader.SetVec3("spotLight[" + KhSTL::ToString(spot) + "].color", t->GetColorToVec());
			_shader.SetVec3("spotLight[" + KhSTL::ToString(spot) + "].position", ligthtPosition);
			_shader.SetVec3("spotLight[" + KhSTL::ToString(spot) + "].direction", t->GetDirection());
			_shader.SetFloat("spotLight[" + KhSTL::ToString(spot) + "].cutOff", t->cutOff);
			_shader.SetFloat("spotLight[" + KhSTL::ToString(spot) + "].outerCutOff", t->outerCutOff);
			_shader.SetFloat("spotLight[" + KhSTL::ToString(spot) + "].constant", t->constant);
			_shader.SetFloat("spotLight[" + KhSTL::ToString(spot) + "].linear", t->linear);
			_shader.SetFloat("spotLight[" + KhSTL::ToString(spot) + "].quadratic", t->quadratic);
			_shader.SetVec3("spotLight[" + KhSTL::ToString(spot) + "].ambient", t->ambient);
			_shader.SetVec3("spotLight[" + KhSTL::ToString(spot) + "].diffuse", t->diffuse);
			_shader.SetVec3("spotLight[" + KhSTL::ToString(spot) + "].specular", t->specular);
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