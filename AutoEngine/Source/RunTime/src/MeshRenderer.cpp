#include "MeshRenderer.h"
#include "Renderer.h"
#include "Light.h"
#include "Configs.h"
#include "ResourceSystem.h"
#include "NewDef.h"
namespace Auto3D {

MeshRenderer::MeshRenderer(Ambient* ambient)
	:Super(ambient)
	, _tshader(_Shader(shader_path + "au_light_map_model_loading.auvs"
		, shader_path + "au_light_map_model_loading.aufs"))
	, _isUserShader(false)
{
	_material.reset(new Material(_ambient));
}

MeshRenderer::~MeshRenderer()
{
	UnloadOpaque(this);
}

void MeshRenderer::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<MeshRenderer>(SCENE_ATTACH);
}

void MeshRenderer::Start()
{
	if (_isUserShader)
	{
		///User Shader code
	}
	else
	{
		if (_material->isTexture)
		{
			_tshader = _Shader(shader_path + "au_light_map_model_loading.auvs"
				, shader_path + "au_light_map_model_loading.aufs");
		}
		else
		{
			_tshader = _Shader(shader_path + "au_light_model_loading.auvs"
				, shader_path + "au_light_model_loading.aufs");
		}
	}
	RegisterOpaque(this);
}

void MeshRenderer::Draw()
{
	if (!_mesh)
		return;

	_tshader.Use();

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

	_tshader.SetMat4("model", modelMat);
	_tshader.SetMat4("view", viewMat);
	_tshader.SetMat4("projection", projectionMat);
	_tshader.SetVec3("viewPos", GetSubSystem<Renderer>()->GetCurrentCamera().GetPosition());
	if (!_isUserShader)
	{
		drawMaterial();
		drawLight();
	}
	_mesh->DrawMesh(_tshader);

	GLOriginal();
}

void MeshRenderer::SetMesh(Mesh* mesh)
{
	_mesh.reset(mesh);
}

void MeshRenderer::SetShader(const _Shader& shader)
{
	_tshader = shader;
}


void MeshRenderer::drawMaterial()
{
	if (_material->isTexture)
	{
		_tshader.SetInt("material.color", 0);
	}
	else
	{
		_tshader.SetVec3("material.color", _material->color.r, _material->color.g, _material->color.b);
	}
	_tshader.SetVec3("material.diffuse", _material->diffuse);
	_tshader.SetVec3("material.specular", _material->specular);
	_tshader.SetFloat("material.shininess", _material->shininess);
}

void MeshRenderer::drawLight()
{
	auto& lights = GetSubSystem<Renderer>()->GetLightContainer()->GetAllLights();
	int dir = 0;
	int point = 0;
	int spot = 0;
	for (auto it = lights.begin(); it != lights.end(); it++)
	{
		Light * t = *it;
		Vector3 ligthtPosition = t->GetNode().GetComponent<Transform>()->GetPosition();
		_tshader.SetVec3("lightPos", ligthtPosition);

		switch (t->GetType())
		{
		case LightType::Directional:
			if (dir >= 4)break;
			_tshader.SetVec3("dirLight[" + KhSTL::ToString(dir) + "].color", t->GetColorToVec());
			_tshader.SetVec3("dirLight[" + KhSTL::ToString(dir) + "].direction", t->GetDirection());
			_tshader.SetVec3("dirLight[" + KhSTL::ToString(dir) + "].ambient", t->ambient);
			_tshader.SetVec3("dirLight[" + KhSTL::ToString(dir) + "].diffuse", t->diffuse);
			_tshader.SetVec3("dirLight[" + KhSTL::ToString(dir) + "].specular", t->specular);
			dir++;
			break;
		case LightType::Point:
			if (point >= 8)break;
			_tshader.SetVec3("pointLight[" + KhSTL::ToString(point) + "].color", t->GetColorToVec());
			_tshader.SetVec3("pointLight[" + KhSTL::ToString(point) + "].position", ligthtPosition);
			_tshader.SetFloat("pointLight[" + KhSTL::ToString(point) + "].constant", t->constant);
			_tshader.SetFloat("pointLight[" + KhSTL::ToString(point) + "].linear", t->linear);
			_tshader.SetFloat("pointLight[" + KhSTL::ToString(point) + "].quadratic", t->quadratic);
			_tshader.SetVec3("pointLight[" + KhSTL::ToString(point) + "].ambient", t->ambient);
			_tshader.SetVec3("pointLight[" + KhSTL::ToString(point) + "].diffuse", t->diffuse);
			_tshader.SetVec3("pointLight[" + KhSTL::ToString(point) + "].specular", t->specular);
			point++;
			break;
		case LightType::Spot:

			if (spot >= 4)break;
			_tshader.SetVec3("spotLight[" + KhSTL::ToString(spot) + "].color", t->GetColorToVec());
			_tshader.SetVec3("spotLight[" + KhSTL::ToString(spot) + "].position", ligthtPosition);
			_tshader.SetVec3("spotLight[" + KhSTL::ToString(spot) + "].direction", t->GetDirection());
			_tshader.SetFloat("spotLight[" + KhSTL::ToString(spot) + "].cutOff", t->cutOff);
			_tshader.SetFloat("spotLight[" + KhSTL::ToString(spot) + "].outerCutOff", t->outerCutOff);
			_tshader.SetFloat("spotLight[" + KhSTL::ToString(spot) + "].constant", t->constant);
			_tshader.SetFloat("spotLight[" + KhSTL::ToString(spot) + "].linear", t->linear);
			_tshader.SetFloat("spotLight[" + KhSTL::ToString(spot) + "].quadratic", t->quadratic);
			_tshader.SetVec3("spotLight[" + KhSTL::ToString(spot) + "].ambient", t->ambient);
			_tshader.SetVec3("spotLight[" + KhSTL::ToString(spot) + "].diffuse", t->diffuse);
			_tshader.SetVec3("spotLight[" + KhSTL::ToString(spot) + "].specular", t->specular);
			spot++;
			break;
		default:
			WarningString("Fail to set light!");
		}
	}
	_tshader.SetBool("dirBool", (bool)dir);
	_tshader.SetBool("pointBool", (bool)point);
	_tshader.SetBool("spotBool", (bool)spot);

	_tshader.SetInt("dirNum", dir);
	_tshader.SetInt("pointNum", point);
	_tshader.SetInt("spotNum", spot);
}
}