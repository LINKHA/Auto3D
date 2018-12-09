#include "Level_0.h"
#include "Mesh.h"
#include "SpriteTranslucent.h"
#include "SpriteRenderer.h"
#include "../FreeCamera.h"
#include "Time.h"
#include "ResourceSystem.h"
#include "Image.h"
#include "Light.h"

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


	auto* imageLogo = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/logo.png");
	auto* imageGrass = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/grass.png");

	obj = CreateNode();
	obj->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, -2.0f);
	obj->CreateComponent<SpriteRenderer>()->SetImage(imageLogo);
	//tex1->SetColor(Color(0.5f, 0.5f, 0.5f));


	Node* obj5 = CreateNode();
	auto* tex5 = obj5->CreateComponent<SpriteRenderer>();
	tex5->SetImage(imageGrass);
	//tex5->EnableBlend(true);
	obj5->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, -3.0f);


	Node* obj6 = CreateNode();
	auto* tex6 = obj6->CreateComponent<SpriteRenderer>();
	tex6->SetImage(imageGrass);
	//tex6->EnableBlend(true);
	obj6->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, -4.0f);

}
void Level_0::Update()
{
	float scaleAmount = (float)sin(GetSubSystem<Time>()->GetCurTime());
	Transform* t = obj->GetComponent<Transform>();
	t->SetRotation(Vector3(0.0f, 0.0f, 90.0f));
	//obj->GetComponent(Transform).setRotation(-55.0f, Vector3::xAxis);
	t->SetRotation(90.0f, Vector3::zAxis);
	t->SetScale(Vector3(scaleAmount));

}