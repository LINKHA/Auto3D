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

	/*AActor* player = scene->CreateChild<AActor>();
	player->CreateComponent<ADefaultController>();
	camera = player->CreateComponent<ACamera>();
	camera->SetPosition(TVector3F(0.0f, 0.0f, 0.0f));
	camera->SetAmbientColor(FColor(0.1f, 0.1f, 0.1f));

	AStaticModel* plane = scene->CreateComponent<AStaticModel>();
	plane->SetPosition(TVector3F(0.0f, -5.1f, 0.0f));
	plane->SetTagName("StaticModelTag");
	plane->SetScale(TVector3F(50.0f, 0.1f, 50.0f));
	plane->SetCastShadows(true);
	plane->SetModel(cache->LoadResource<AModel>("Model/Box.mdl"));
	plane->SetMaterial(cache->LoadResource<AMaterial>("Stone.json"));*/

	/*AStaticModel* teaPot = scene->CreateComponent<AStaticModel>();
	teaPot->SetLayerName("StaticModel");
	teaPot->SetPosition(TVector3F(0.0f, 0.0f, 0.0f));
	teaPot->SetScale(10.0f);
	teaPot->SetModel(cache->LoadResource<AModel>("Model/TeaPot.mdl"));
	teaPot->SetCastShadows(true);*/

	ASkyBox* skybox = scene->CreateChildNode<ASkyBox>();
	skybox->SetMaterial(cache->LoadResource<AMaterial>("SkyBox.json"));
	
	/*AActor* waterActor = scene->CreateChild<AActor>();
	water = waterActor->CreateComponent<ADynamicModel>();*/
	water = new ADynamicModel;
	water->BeginPlay();

	
}

void WaterSample::Update()
{
	Super::Update();
}

void WaterSample::UIDraw()
{
	water->TickComponent(0);
}

void WaterSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(WaterSample)