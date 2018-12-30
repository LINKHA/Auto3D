#include "Level_0.h"
#include "Light.h"
#include "Mesh.h"
#include "../FreeCamera.h"
#include "ResourceSystem.h"
#include "MeshRenderer.h"


void Level_0::Start()
{
	auto nanosuit = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/nanosuit/nanosuit.obj");
	auto shaderVS = GetSubSystem<ResourceSystem>()->GetResource<Shader>("shader/au_vertex_explode.glvs");
	auto shaderFS = GetSubSystem<ResourceSystem>()->GetResource<Shader>("shader/au_vertex_explode.glfs");
	auto shaderGS = GetSubSystem<ResourceSystem>()->GetResource<Shader>("shader/au_vertex_explode.glgs");
	
	GameNode cameraObj = CreateNode("camera");
	SharedPtr<FreeCamera> freeCamera = MakeShared<FreeCamera>(_ambient);
	cameraObj->AddComponent(freeCamera);
	freeCamera->cameraNode->SetPosition(-5.0f, 8.0f, 20.0f);

	GameNode lightObj = CreateNode("light");
	auto light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Directional);

	GameNode meshObj = CreateNode("mesh");
	auto meshRenderer = meshObj->CreateComponent<MeshRenderer>();
	meshRenderer->SetMesh(nanosuit);
	auto variation = MakeShared<ShaderVariation>(shaderVS, shaderFS, shaderGS);
	meshRenderer->SetShaderVariation(variation);

	GameNode meshObj2 = CreateNode("mesh2");
	meshObj2->SetPosition(10.0f, 0.0f, 0.0f);
	auto meshRenderer2 = meshObj2->CreateComponent<MeshRenderer>();
	meshRenderer2->SetMesh(nanosuit);
	meshRenderer2->GetMaterial()->isTexture = true;

	
}

void Level_0::Update()
{
}

