#include "Level_0.h"
#include "ResourceSystem.h"
#include "../FreeCamera.h"
#include "Transform.h"
#include "Shader.h"
#include "SpriteRenderer.h"
#include "Image.h"
void Level_0::Awake()
{
	Super::Awake();
}
void Level_0::Start()
{
	Super::Start();
	Node* camObj = CreateNode();
	FreeCamera* freeCamera = new FreeCamera(_ambient, _sceneID);
	camObj->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 3.0f);
	camObj->AddComponent(freeCamera);


	auto* imageLogo = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/logo.png");
	//auto* shader = GetSubSystem<ResourceSystem>()->GetResource<Shader>("shader/au_texture_transform.glsl");
	
	Node* obj = CreateNode();
	obj->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, -2.0f);
	obj->CreateComponent<SpriteRenderer>()->SetImage(imageLogo);

}
void Level_0::Update()
{
	Super::Update();
}