#include "Level_0.h"
#include "Mesh.h"
#include "SpriteRenderer.h"
#include "../FreeCamera2D.h"
#include "Time.h"
#include "ResourceSystem.h"
#include "Image.h"
#include "MeshRenderer.h"
#include "IO.h"

void Level_0::Awake()
{
	Super::Awake();
}
void Level_0::Start()
{
	Super::Start();

	GameNode camObj = CreateNode();
	SharedPtr<FreeCamera2D> camera = MakeShared<FreeCamera2D>(_ambient);
	camObj->AddComponent(camera);

	auto imageLogo = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/logo.png");
	for (int i = 0; i < 1; i++)
	{
		auto obj = CreateNode();
		obj->SetScale(0.2f);
		obj->SetPosition(0.0f, i/3.0f, -2.0f);
		obj->CreateComponent<SpriteRenderer>()->SetImage(imageLogo);
	}
	

}
void Level_0::Update()
{
	Super::Update();
	//Print(GetSubSystem<Time>()->GetFramesPerSecond());

}