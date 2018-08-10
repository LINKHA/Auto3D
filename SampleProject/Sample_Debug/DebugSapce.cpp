#include "DebugSapce.h"
#include "Application.h"
#include "LightDirectional.h"
#include "../FreeCamera.h"
#include "Mesh.h"
#include "Sprite.h"
#include "Input.h"
#include "Time.h"
DebugSapce::DebugSapce(Ambient* ambient)
	:MotionSpace(ambient)
{}
DebugSapce::~DebugSapce()
{}

void DebugSapce::Start()
{
	GameObject* cameraObj = new GameObject(_ambient);
	FreeCamera* camera = new FreeCamera(_ambient);

	//camera->freeCamera->SetSortMode(kSortOrthographic);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);


	GameObject* lightObj = new GameObject(_ambient);
	Light* light = new LightDirectional(_ambient);
	lightObj->AddComponent(light);


	GameObject* meshObj = new GameObject(_ambient);
	Mesh* mesh = new Mesh(_ambient, "../Resource/object/base/Sphere.3DS");
	mesh->GetMaterial().color = Color(1.0f, 1.0f, 1.0f);

	meshObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, -3.0f);
	meshObj->AddComponent(mesh);

	meshObj2 = new GameObject(_ambient);
	Mesh* mesh2 = new Mesh(_ambient, "../Resource/object/base/Pipe.3DS");
	mesh2->GetMaterial().color = Color(0.0f, 0.0f, 1.0f);
	meshObj2->GetComponent(Transform).SetPosition(0.0f, 0.0f, -3.0f);
	meshObj2->AddComponent(mesh2);
}
void DebugSapce::Update()
{
	Transform& t = meshObj2->GetComponent(Transform);

	if (GetSubSystem<Input>()->GetKeyPress(SDLK_UP))
	{
		meshScale += GetSubSystem<Time>()->GetDeltaTime();
		t.SetScale(meshScale, meshScale, meshScale);
	}
	if (GetSubSystem<Input>()->GetKeyPress(SDLK_DOWN))
	{
		meshScale -= GetSubSystem<Time>()->GetDeltaTime();
		t.SetScale(meshScale, meshScale, meshScale);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////
	if (GetSubSystem<Input>()->GetKeyPress(SDLK_t))
	{
		meshScale += GetSubSystem<Time>()->GetDeltaTime();
		t.SetScale(meshScale, 1.0f, 1.0f);
	}
	if (GetSubSystem<Input>()->GetKeyPress(SDLK_g))
	{
		meshScale -= GetSubSystem<Time>()->GetDeltaTime();
		t.SetScale(meshScale, 1.0f, 1.0f);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////
	if (GetSubSystem<Input>()->GetKeyPress(SDLK_y))
	{
		meshScale += GetSubSystem<Time>()->GetDeltaTime();
		t.SetScale(1.0f, meshScale, 1.0f);
	}
	if (GetSubSystem<Input>()->GetKeyPress(SDLK_h))
	{
		meshScale -= GetSubSystem<Time>()->GetDeltaTime();
		t.SetScale(1.0f, meshScale, 1.0f);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////
	if (GetSubSystem<Input>()->GetKeyPress(SDLK_u))
	{
		meshScale += GetSubSystem<Time>()->GetDeltaTime();
		t.SetScale(1.0f, 1.0f, meshScale);
	}
	if (GetSubSystem<Input>()->GetKeyPress(SDLK_j))
	{
		meshScale -= GetSubSystem<Time>()->GetDeltaTime();
		t.SetScale(1.0f, 1.0f, meshScale);
	}
}
int DebugSapce::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(DebugSapce)