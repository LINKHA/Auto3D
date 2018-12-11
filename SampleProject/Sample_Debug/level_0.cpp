#include "Level_0.h"
#include "Mesh.h"
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
	
	Node* obj4 = CreateNode();
	auto* tex4 = obj4->CreateComponent<SpriteRenderer>();
	tex4->SetImage(imageLogo);
	tex4->GetImage()->SetImageType(ImageType::Translucent);
	tex4->EnableBlend(true);
	obj4->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, -4.0f);
}
void Level_0::Update()
{
	
}