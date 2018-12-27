#include "Level_0.h"
#include "Light.h"
#include "Mesh.h"
#include "../FreeCamera.h"
#include "ResourceSystem.h"
#include "MeshRenderer.h"


void Level_0::Start()
{
	auto* nanosuit = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/nanosuit/nanosuit.obj");
	auto* shaderVS = GetSubSystem<ResourceSystem>()->GetResource<Shader>("shader/au_vertex_explode.glvs");
	auto* shaderFS = GetSubSystem<ResourceSystem>()->GetResource<Shader>("shader/au_vertex_explode.glfs");
	auto* shaderGS = GetSubSystem<ResourceSystem>()->GetResource<Shader>("shader/au_vertex_explode.glgs");
	
	Node* cameraObj = CreateNode("camera");
	FreeCamera* camera = new FreeCamera(_ambient);
	cameraObj->AddComponent(camera);
	camera->cameraNode->SetPosition(-5.0f, 8.0f, 20.0f);

	Node* lightObj = CreateNode("light");
	auto* light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Directional);

	Node* meshObj = CreateNode("mesh");
	auto* meshRenderer = meshObj->CreateComponent<MeshRenderer>();
	meshRenderer->SetMesh(nanosuit);
	ShaderVariation* variation = new ShaderVariation(shaderVS, shaderFS, shaderGS);
	meshRenderer->SetShaderVariation(variation);

	Node* meshObj2 = CreateNode("mesh2");
	meshObj2->SetPosition(10.0f, 0.0f, 0.0f);
	auto* meshRenderer2 = meshObj2->CreateComponent<MeshRenderer>();
	meshRenderer2->SetMesh(nanosuit);
	meshRenderer2->GetMaterial()->isTexture = true;

	
}

void Level_0::Update()
{
}

