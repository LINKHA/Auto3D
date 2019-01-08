#include "Level_0.h"
#include "../FreeCamera2D.h"
#include "Node.h"
#include "Image.h"
#include "SpriteRenderer.h"
#include "ResourceSystem.h"


void Level_0::Awake()
{
	Super::Awake();
}
void Level_0::Start()
{
	Super::Start();

	auto imageLogo = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/logo.png");

	GameNode cameraNode = CreateNode();
	SharedPtr<FreeCamera2D> camera = MakeShared<FreeCamera2D>(_ambient);
	cameraNode->AddComponent(camera);

	GameNode imageNode = CreateNode();
	imageNode->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, -2.0f);
	imageNode->CreateComponent<SpriteRenderer>()->SetImage(imageLogo);
}
void Level_0::Update()
{
	Super::Update();
}