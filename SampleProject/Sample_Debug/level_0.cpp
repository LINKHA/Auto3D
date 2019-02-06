#include "Level_0.h"
#include "Mesh.h"
#include "SpriteRenderer.h"
#include "../FreeCamera2D.h"
#include "Time.h"
#include "ResourceSystem.h"
#include "Image.h"
#include "Light.h"
#include "MeshRenderer.h"


void Level_0::Awake()
{
	//Super::Awake();
}
void Level_0::Start()
{
	Super::Start();

	GameNode camObj = CreateNode();
	SharedPtr<FreeCamera2D> camera = MakeShared<FreeCamera2D>(_ambient);
	camObj->AddComponent(camera);

	auto imageLogo = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/logo.png");
	for (int i = 0; i < 1000; i++)
	{
		auto obj = CreateNode();
		obj->SetPosition(0.0f, i, -2.0f);
		obj->CreateComponent<SpriteRenderer>()->SetImage(imageLogo);
	}
	

}
void Level_0::Update()
{
	Super::Update();


}