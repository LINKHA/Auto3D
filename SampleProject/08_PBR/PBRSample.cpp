#include "PBRSample.h"

void PBRSample::Init()
{
	Super::Init();
	auto* graphics = Object::Module<Graphics>();
	graphics->RenderWindow()->SetTitle("PBR Sample");
}
IBLMaterial* ibl;
/*
void PBRSample::Start()
{
	Super::Start();
	auto* cache = Object::Module<ResourceCache>();
	auto* graphics = Object::Module<Graphics>();

	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	camera->SetPosition(Vector3F(0.0f, 10.0f, -20.0f));
	camera->SetAmbientColor(Color(1.0f, 0.1f, 0.1f));

	StaticModel* sphere = scene->CreateChild<StaticModel>();
	sphere->SetPosition(Vector3F(5.0f, 5.0f, 0.0f));
	sphere->SetScale(Vector3F(2.0f, 2.0f,2.0f));
	sphere->SetCastShadows(true);
	sphere->SetModel(cache->LoadResource<Model>("Model/Sphere.mdl"));
	sphere->SetMaterial(cache->LoadResource<Material>("PBRNoTexture.json"));

	StaticModel* teaPot = scene->CreateChild<StaticModel>();
	teaPot->SetPosition(Vector3F(-5.0f, 0.0f, 0.0f));
	teaPot->SetScale(Vector3F(10.0f, 10.0f, 10.0f));
	teaPot->SetCastShadows(true);
	teaPot->SetModel(cache->LoadResource<Model>("Model/TeaPot.mdl"));
	teaPot->SetMaterial(cache->LoadResource<Material>("PBRTitanium.json"));


	StaticModel* plane = scene->CreateChild<StaticModel>();
	plane->SetPosition(Vector3F(-5.0f, 0.0f, 0.0f));
	plane->SetScale(Vector3F(100.0f, 1.0f, 100.0f));
	plane->SetCastShadows(true);
	plane->SetModel(cache->LoadResource<Model>("Model/Box.mdl"));
	plane->SetMaterial(cache->LoadResource<Material>("PBRWood.json"));


	SkyBox* skybox = scene->CreateChild<SkyBox>();
	skybox->SetMaterial(cache->LoadResource<Material>("HdrSkyBox.json"));

	Light* light = scene->CreateChild<Light>();
	light->SetLightType(LightType::POINT);
	light->SetCastShadows(true);
	light->SetColor(Color(500.0f, 500.0f, 500.0f));
	light->SetRange(100.0f);
	light->SetPosition(Vector3F(-10.0f, 10.0f, 10.0f));

	Light* light2 = scene->CreateChild<Light>();
	light2->SetLightType(LightType::POINT);
	light2->SetCastShadows(true);
	light2->SetColor(Color(500.0f, 500.0f, 500.0f));
	light2->SetRange(100.0f);
	light2->SetPosition(Vector3F(10.0f, 10.0f, 10.0f));

	Light* light3 = scene->CreateChild<Light>();
	light3->SetLightType(LightType::POINT);
	light3->SetCastShadows(true);
	light3->SetColor(Color(500.0f, 500.0f, 500.0f));
	light3->SetRange(100.0f);
	light3->SetPosition(Vector3F(-10.0f, -10.0f, 10.0f));

	Light* light4 = scene->CreateChild<Light>();
	light4->SetLightType(LightType::POINT);
	light4->SetCastShadows(true);
	light4->SetColor(Color(500.0f, 500.0f, 500.0f));
	light4->SetRange(100.0f);
	light4->SetPosition(Vector3F(10.0f, -10.0f, 10.0f));
}
*/
Texture* iblCube;
void PBRSample::Start()
{
	//Super::Start();
	auto* cache = Object::Module<ResourceCache>();
	auto* graphics = Object::Module<Graphics>();

	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	camera->SetPosition(Vector3F(0.0f, 5.0f, -5.0f));
	camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));

	//StaticModel* sphere = scene->CreateChild<StaticModel>();
	//sphere->SetModel(cache->LoadResource<Model>("Model/Sphere.mdl"));
	//sphere->SetMaterial(cache->LoadResource<Material>("PBRTitanium.json"));

	SkyBox* skybox = scene->CreateChild<SkyBox>();
	skybox->SetMaterial(cache->LoadResource<Material>("HdrSkyBox.json"));









	iblCube = skybox->GetMaterial(0)->_textures[0];

	ibl = cache->LoadResource<IBLMaterial>("IBLPBRTitanium.json");
	//ibl->SetupIBL(skybox);
	//ibl->SetAAA(iblCube);

	StaticModel* sphere = scene->CreateChild<StaticModel>();
	sphere->SetScale(10);
	sphere->SetModel(cache->LoadResource<Model>("Model/TeaPot.mdl"));
	sphere->SetMaterial(ibl);


	////
	



	//ibl->CreatePass(camera);
}

void PBRSample::UIDraw()
{
		//ibl->CreatePass(camera);

}

void PBRSample::Update()
{

	Super::Update();
	auto input = ModuleManager::Get().InputModule();
	auto graphics = ModuleManager::Get().GraphicsModule();
	auto renderer = ModuleManager::Get().RendererModule();
	auto time = ModuleManager::Get().TimeModule();

	pitch += input->GetMouseMove()._y * 0.25f;
	yaw += input->GetMouseMove()._x * 0.25f;
	pitch = Clamp(pitch, -90.0f, 90.0f);

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 50 : 10.0f;

	camera->SetRotation(Quaternion(pitch, yaw, 0.0f));
	if (input->IsKeyDown(KEY_W))
		camera->Translate(Vector3F::FORWARD * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		camera->Translate(Vector3F::BACK * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		camera->Translate(Vector3F::LEFT * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		camera->Translate(Vector3F::RIGHT * time->GetDeltaTime()  * moveSpeed);
}

void PBRSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(PBRSample)