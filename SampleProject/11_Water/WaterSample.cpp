#include "WaterSample.h"

void WaterSample::Init()
{
	//Super::Init();
	auto* graphics = GModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("Water Sample");
}

ADynamicModel* water;

void WaterSample::Start()
{
	//Super::Start();
	auto* cache = GModuleManager::Get().CacheModule();
	auto* graphics = GModuleManager::Get().GraphicsModule();

	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	scene = AObject::Create<AWorld>();
	scene->CreateChildNode<AOctree>();
	//camera = scene->CreateChildNode<Camera>();
	//camera->SetPosition(Vector3F(0.0f, 0.0f, 0.0f));
	//camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));

	ASkyBox* skybox = scene->CreateChildNode<ASkyBox>();
	skybox->SetMaterial(cache->LoadResource<AMaterial>("SkyBox.json"));
	
	AActor* waterActor = scene->CreateChild<AActor>();
	water = waterActor->CreateComponent<ADynamicModel>();
}

void WaterSample::Update()
{
	Super::Update();
}

void WaterSample::UIDraw()
{

}

void WaterSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(WaterSample)