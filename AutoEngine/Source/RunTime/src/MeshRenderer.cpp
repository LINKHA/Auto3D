#include "MeshRenderer.h"
#include "Renderer.h"
#include "Light.h"
#include "ResourceSystem.h"
#include "Shader.h"
#include "ShaderVariation.h"
#include "LogDef.h"

#include "NewDef.h"
namespace Auto3D {

MeshRenderer::MeshRenderer(Ambient* ambient)
	:Super(ambient)
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
	if (!_isUserShader)
	{
		if (_material->isTexture)
		{
			auto* shader = GetSubSystem<ResourceSystem>()->GetResource<Shader>("shader/au_light_map_model_loading.glsl");
			_shader = MakeShared<ShaderVariation>(shader);
			_shader->Create();
		}
		else
		{
			auto* cach = GetSubSystem<ResourceSystem>();
			auto* shader = cach->GetResource<Shader>("shader/au_light_model_loading.glsl");
			_shader = MakeShared<ShaderVariation>(shader);
			_shader->Create();
		}
	}
	RegisterOpaque(this);
}

void MeshRenderer::Draw()
{
	if (!_mesh)
		return;

	_shader->Use();

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

	_shader->SetMat4("model", modelMat);
	_shader->SetMat4("view", viewMat);
	_shader->SetMat4("projection", projectionMat);
	_shader->SetVec3("viewPos", GetSubSystem<Renderer>()->GetCurrentCamera().GetPosition());
	if (!_isUserShader)
	{
		drawMaterial();
		drawLight();
	}
	_mesh->DrawMesh(_shader.get());

	GLOriginal();
}

void MeshRenderer::SetMesh(Mesh* mesh)
{
	_mesh.reset(mesh);
}

void MeshRenderer::SetShaderVariation(ShaderVariation* shader)
{
	_isUserShader = true;
	_shader.reset(shader);
	_shader->Create();
}


void MeshRenderer::drawMaterial()
{
	if (_material->isTexture)
	{
		_shader->SetInt("material.color", 0);
	}
	else
	{
		_shader->SetVec3("material.color", _material->color.r, _material->color.g, _material->color.b);
	}
	_shader->SetVec3("material.diffuse", _material->diffuse);
	_shader->SetVec3("material.specular", _material->specular);
	_shader->SetFloat("material.shininess", _material->shininess);
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
		_shader->SetVec3("lightPos", ligthtPosition);

		switch (t->GetType())
		{
		case LightType::Directional:
			if (dir >= 4)break;
			_shader->SetVec3("dirLight[" + KhSTL::ToString(dir) + "].color", t->GetColorToVec());
			_shader->SetVec3("dirLight[" + KhSTL::ToString(dir) + "].direction", t->GetDirection());
			_shader->SetVec3("dirLight[" + KhSTL::ToString(dir) + "].ambient", t->ambient);
			_shader->SetVec3("dirLight[" + KhSTL::ToString(dir) + "].diffuse", t->diffuse);
			_shader->SetVec3("dirLight[" + KhSTL::ToString(dir) + "].specular", t->specular);
			dir++;
			break;
		case LightType::Point:
			if (point >= 8)break;
			_shader->SetVec3("pointLight[" + KhSTL::ToString(point) + "].color", t->GetColorToVec());
			_shader->SetVec3("pointLight[" + KhSTL::ToString(point) + "].position", ligthtPosition);
			_shader->SetFloat("pointLight[" + KhSTL::ToString(point) + "].constant", t->constant);
			_shader->SetFloat("pointLight[" + KhSTL::ToString(point) + "].linear", t->linear);
			_shader->SetFloat("pointLight[" + KhSTL::ToString(point) + "].quadratic", t->quadratic);
			_shader->SetVec3("pointLight[" + KhSTL::ToString(point) + "].ambient", t->ambient);
			_shader->SetVec3("pointLight[" + KhSTL::ToString(point) + "].diffuse", t->diffuse);
			_shader->SetVec3("pointLight[" + KhSTL::ToString(point) + "].specular", t->specular);
			point++;
			break;
		case LightType::Spot:

			if (spot >= 4)break;
			_shader->SetVec3("spotLight[" + KhSTL::ToString(spot) + "].color", t->GetColorToVec());
			_shader->SetVec3("spotLight[" + KhSTL::ToString(spot) + "].position", ligthtPosition);
			_shader->SetVec3("spotLight[" + KhSTL::ToString(spot) + "].direction", t->GetDirection());
			_shader->SetFloat("spotLight[" + KhSTL::ToString(spot) + "].cutOff", t->cutOff);
			_shader->SetFloat("spotLight[" + KhSTL::ToString(spot) + "].outerCutOff", t->outerCutOff);
			_shader->SetFloat("spotLight[" + KhSTL::ToString(spot) + "].constant", t->constant);
			_shader->SetFloat("spotLight[" + KhSTL::ToString(spot) + "].linear", t->linear);
			_shader->SetFloat("spotLight[" + KhSTL::ToString(spot) + "].quadratic", t->quadratic);
			_shader->SetVec3("spotLight[" + KhSTL::ToString(spot) + "].ambient", t->ambient);
			_shader->SetVec3("spotLight[" + KhSTL::ToString(spot) + "].diffuse", t->diffuse);
			_shader->SetVec3("spotLight[" + KhSTL::ToString(spot) + "].specular", t->specular);
			spot++;
			break;
		default:
			WarningString("Fail to set light!");
		}
	}
	_shader->SetBool("dirBool", (bool)dir);
	_shader->SetBool("pointBool", (bool)point);
	_shader->SetBool("spotBool", (bool)spot);

	_shader->SetInt("dirNum", dir);
	_shader->SetInt("pointNum", point);
	_shader->SetInt("spotNum", spot);
}
}