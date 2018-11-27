#include "Level_0.h"
#include "Mesh.h"
#include "SpriteTranslucent.h"
#include "Sprite.h"
#include "../FreeCamera.h"
#include "Time.h"

Level_0::Level_0(Ambient* ambient, int levelNumber)
	:LevelScene(ambient, levelNumber)
{}
void Level_0::Awake()
{
	SceneSuper::Awake();
}
void Level_0::Start()
{
	Super::Start();
	Node* camObj = CreateNode();
	FreeCamera* freeCamera = new FreeCamera(_ambient, _sceneID);
	camObj->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 3.0f);
	camObj->AddComponent(freeCamera);


	//SpriteTranslucent* tex2 = new SpriteTranslucent(_ambient, "../Resource/texture/window.png");
	//tex2->EnableBlend(true);
	////tex2->EnableDepth(false);
	//GameObject* obj2 = new GameObject(_ambient, _levelNumber);
	//obj2->GetComponent(Transform).SetPosition(0.0f, 0.0f, 0.0f);
	//obj2->AddComponent(tex2);

	//SpriteTranslucent * tex3 = new SpriteTranslucent(_ambient, "../Resource/texture/window.png");
	//tex3->EnableBlend(true);
	////tex3->EnableDepth(false);
	//GameObject* obj3 = new GameObject(_ambient, _levelNumber);
	//obj3->GetComponent(Transform).SetPosition(0.2f, 0.0f, -1.0f);
	//obj3->AddComponent(tex3);

	//SpriteTranslucent * tex4 = new SpriteTranslucent(_ambient, "../Resource/texture/window.png");
	//tex4->EnableBlend(true);
	////tex4->EnableDepth(false);
	//GameObject* obj4 = new GameObject(_ambient, _levelNumber);
	//obj4->GetComponent(Transform).SetPosition(-0.2f, 0.0f, -2.0f);
	//obj4->AddComponent(tex4);


	obj = CreateNode();
	obj->CreateComponent<Sprite>()->SetImage("../Resource/texture/logo.png");
	//tex1->SetColor(Color(0.5f, 0.5f, 0.5f));

	Node* obj5 = CreateNode();
	obj5->CreateComponent<Sprite>()->SetImage("../Resource/texture/grass.png");
	//tex5->EnableBlend(true);
	obj5->GetComponent<Transform>()->SetPosition(-0.2f, 0.0f, -3.0f);


	Node* obj6 = CreateNode();
	obj6->CreateComponent<Sprite>()->SetImage("../Resource/texture/grass.png");
	//tex6->EnableBlend(true);
	obj6->GetComponent<Transform>()->SetPosition(-0.2f, 0.0f, -4.0f);

}
void Level_0::Update()
{
	float scaleAmount = (float)sin(GetSubSystem<Time>()->GetCurTime());
	Transform* t = obj->GetComponent<Transform>();

	t->SetPosition(1.0f, 1.0f, 0.0f);
	t->SetRotation(Vector3(0.0f, 0.0f, 90.0f));
	//obj->GetComponent(Transform).setRotation(-55.0f, Vector3::xAxis);
	t->SetRotation(90.0f, Vector3::zAxis);
	t->SetScale(Vector3(scaleAmount));

}
