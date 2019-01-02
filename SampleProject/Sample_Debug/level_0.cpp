#include "Level_0.h"
#include "Mesh.h"
#include "SpriteRenderer.h"
#include "../FreeCamera.h"
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

	auto imageWindow = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/window.png");
	
	GameNode obj1 = CreateNode();
	auto plane = obj1->CreateComponent<SpriteRenderer>();
	plane->SetImage(imageWindow);
}
void Level_0::Update()
{
	Super::Update();


}