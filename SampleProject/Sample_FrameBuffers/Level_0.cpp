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
	auto* cube = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Cube.3DS");
	auto* imageGrass = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/grass.png");
	auto* imageWindow = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/window.png");

	Node* camObj = CreateNode();
	camera = new FreeCamera(_ambient, _sceneID);
	camera->freeCamera->AllowOffScreen(true);
	camera->freeCamera->AllowMSAA(true);
	camera->freeCamera->AllowLateEffect(true);
	camObj->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 3.0f);
	camObj->AddComponent(camera);


	Node* obj2 = CreateNode();
	obj2->SetPosition(0.0f, 0.0f, 0.0f);
	auto* tex2 = obj2->CreateComponent<SpriteRenderer>();
	tex2->SetImage(imageWindow);
	tex2->EnableBlend(true);
	tex2->EnableDepth(false);



	Node* obj5 = CreateNode();
	obj5->SetPosition(-0.2f, 0.0f, -3.0f);
	auto* tex5 = obj5->CreateComponent<SpriteRenderer>();
	tex5->SetImage(imageGrass);
	tex5->EnableBlend(true);


	Node* lightObj = CreateNode();
	auto* light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Point);
	//////////////////////////////////////////////////////////////////////////

	Node* meshObj = CreateNode();
	meshObj->SetPosition(1.0f, 0.0f, 0.0f);
	MeshRenderer* mesh = meshObj->CreateComponent<MeshRenderer>();
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
			camera->freeCamera->SetLateEffect(POST_DEFAULT);
			break;
		case 1:
			camera->freeCamera->SetLateEffect(POST_BULR);
			break;
		case 2:
			camera->freeCamera->SetLateEffect(POST_EDGE_DETECTION);
			break;
		case 3:
			camera->freeCamera->SetLateEffect(POST_GRAYSCALE);
			break;
		case 4:
			camera->freeCamera->SetLateEffect(POST_INVERSION);
			break;
		case 5:
			camera->freeCamera->SetLateEffect(POST_SHARPEN);
			break;
		}
	}


}

