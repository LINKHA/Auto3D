#include "Level_0.h"
#include "GameObject.h"
#include "Sprite.h"
#include "LightPoint.h"
#include "Mesh.h"
#include "Time.h"


Level_0::Level_0(Ambient* ambient, int levelNumber)
	:LevelScene(ambient, levelNumber)
{}

void Level_0::Start()
{

	GameObject* camObj = new GameObject(_ambient,_levelNumber);
	camera = new FreeCamera(_ambient, _levelNumber);
	camera->freeCamera->AllowOffScreen(true);
	camera->freeCamera->AllowMSAA(true);
	camera->freeCamera->AllowLateEffect(true);

	camObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	camObj->AddComponent(camera);

	Sprite* tex2 = new Sprite(_ambient, "../Resource/texture/window.png");
	tex2->EnableBlend(true);
	tex2->EnableDepth(false);
	GameObject * obj2 = new GameObject(_ambient, _levelNumber);
	obj2->GetComponent(Transform).SetPosition(0.0f, 0.0f, 0.0f);
	obj2->AddComponent(tex2);


	Sprite* tex5 = new Sprite(_ambient, "../Resource/texture/grass.png");
	tex5->EnableBlend(true);
	GameObject * obj5 = new GameObject(_ambient, _levelNumber);
	obj5->GetComponent(Transform).SetPosition(-0.2f, 0.0f, -3.0f);
	obj5->AddComponent(tex5);

	GameObject* lightObj = new GameObject(_ambient, _levelNumber);
	Light* light = new LightPoint(_ambient);
	lightObj->AddComponent(light);
	//////////////////////////////////////////////////////////////////////////
	Mesh* mesh = new Mesh(_ambient, "../Resource/object/base/Cube.3DS");
	mesh->GetMaterial()->color.Set(0.5f, 0.8f, 0.3f);
	GameObject * meshObj = new GameObject(_ambient, _levelNumber);
	meshObj->GetComponent(Transform).SetPosition(1.0f, 0.0f, 0.0f);
	meshObj->AddComponent(mesh);

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

