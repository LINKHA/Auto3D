#include "Level_0.h"
#include "Mesh.h"
#include "SpriteRenderer.h"
#include "../FreeCamera2D.h"
#include "Time.h"
#include "ResourceSystem.h"
#include "Image.h"
#include "Light.h"

void Level_0::Awake()
{
	Super::Awake();
}
void Level_0::Start()
{
	Super::Start();

	auto camObj = CreateNode();
	SharedPtr<FreeCamera2D> camera = MakeShared<FreeCamera2D>(_ambient);
	camObj->AddComponent(camera);
	//camera->cameraNode->SetPosition(0.0f, 0.0f, 10.0f);
	


	auto imageLogo = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/logo.png");
	auto imageWindow = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/window.png");

	obj = CreateNode();
	obj->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, -2.0f);
	obj->CreateComponent<SpriteRenderer>()->SetImage(imageLogo);
	//tex1->SetColor(Color(0.5f, 0.5f, 0.5f));


	auto obj5 = CreateNode();
	auto tex5 = obj5->CreateComponent<SpriteRenderer>();
	tex5->SetImage(imageWindow);
	//tex5->EnableBlend(true);
	obj5->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, -3.0f);


	auto obj2 = CreateNode();
	auto tex2 = obj2->CreateComponent<SpriteRenderer>();
	tex2->SetImage(imageWindow);
	tex2->GetImage()->SetImageType(ImageType::Translucent);
	tex2->EnableBlend(true);
	//tex2->EnableDepth(false);
	obj2->GetComponent<Transform>()->SetPosition(1.0f, 0.0f, -2.0f);
}
void Level_0::Update()
{
	Super::Update();

	float scaleAmount = (float)sin(GetSubSystem<Time>()->GetCurTime());
	auto transform = obj->GetComponent<Transform>();
	transform->SetRotation(Vector3(0.0f, 0.0f, 90.0f));
	//obj->GetComponent(Transform).setRotation(-55.0f, Vector3::xAxis);
	transform->SetRotation(90.0f, Vector3::zAxis);
	transform->SetScale(Vector3(scaleAmount));

}