#include "Level_0.h"
#include "Node.h"
#include "SpriteRenderer.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "Time.h"
#include "ResourceSystem.h"
#include "Image.h"
#include "../FreeCamera.h"
void Level_0::Start()
{
	auto cube = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Cube.3DS");
	auto imageGrass = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/grass.png");
	auto imageWindow = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/window.png");

	GameNode camObj = CreateNode();
	freeCamera = MakeShared<FreeCamera>(_ambient);
	camObj->AddComponent(freeCamera);
	freeCamera->camera->AllowOffScreen(true);
	freeCamera->camera->AllowMSAA(true);
	freeCamera->camera->AllowLateEffect(true);
	freeCamera->cameraNode->SetPosition(0.0f, 0.0f, 3.0f);
	


	GameNode obj2 = CreateNode();
	obj2->SetPosition(0.0f, 0.0f, 0.0f);
	auto tex2 = obj2->CreateComponent<SpriteRenderer>();
	tex2->SetImage(imageWindow);
	tex2->EnableBlend(true);
	tex2->EnableDepth(false);



	GameNode obj5 = CreateNode();
	obj5->SetPosition(-1.0f, 0.0f, 0.0f);
	auto tex5 = obj5->CreateComponent<SpriteRenderer>();
	tex5->SetImage(imageGrass);
	tex5->EnableBlend(true);


	GameNode lightObj = CreateNode();
	auto light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Point);
	//////////////////////////////////////////////////////////////////////////

	GameNode meshObj = CreateNode();
	meshObj->SetPosition(1.0f, 0.0f, 0.0f);
	auto mesh = meshObj->CreateComponent<MeshRenderer>();
	mesh->SetMesh(cube);
	mesh->GetMaterial()->color.Set(0.5f, 0.8f, 0.3f);
}

void Level_0::Update()
{
	int i = (int)GetSubSystem<Time>()->GetCurTime() % 6;
	if (oldi != i)
	{
		oldi = i;
		switch (i)
		{
		case 0:
			freeCamera->camera->SetLateEffect(POST_DEFAULT);
			break;
		case 1:
			freeCamera->camera->SetLateEffect(POST_BULR);
			break;
		case 2:
			freeCamera->camera->SetLateEffect(POST_EDGE_DETECTION);
			break;
		case 3:
			freeCamera->camera->SetLateEffect(POST_GRAYSCALE);
			break;
		case 4:
			freeCamera->camera->SetLateEffect(POST_INVERSION);
			break;
		case 5:
			freeCamera->camera->SetLateEffect(POST_SHARPEN);
			break;
		}
	}


}

