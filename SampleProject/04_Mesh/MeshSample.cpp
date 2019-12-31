#include "MeshSample.h"

void MeshSample::Init()
{
	Super::Init();
	auto* graphics = GModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("Mesh Sample");
}
void MeshSample::Start()
{
	Super::Start();
	auto* cache = GModuleManager::Get().CacheModule();
	auto* graphics = GModuleManager::Get().GraphicsModule();

	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	scene = AObject::Create<AWorld>();
	scene->DefineLayer(1,"StaticModel");
	scene->DefineTag(1, "StaticModelTag");
	scene->SetupShadowMap(3, 4096);
	scene->CreateChildNode<AOctree>();

	
	AActor* player = Create<AActor>();
	player->SetPosition(TVector3F(0.0f, 5.0f, -15.0f));
	player->CreateComponent<ADefaultController>();
	camera = player->CreateComponent<ACamera>();
	//camera->SetPosition(TVector3F(0.0f, 5.0f, -15.0f));
	camera->SetAmbientColor(FColor(0.1f, 0.1f, 0.1f));
	camera->SetLayoutMaskName("StaticModel"); // The default mask contains all the bits, and this does not change the mask bit, only if the mask view is missing
	//camera->SetLayoutMaskOutName("StaticModel"); //Remove the bits for the view mask
	scene->AddChild(player);

	AStaticModel* plane = scene->CreateComponent<AStaticModel>();
	plane->SetTagName("StaticModelTag");
	plane->SetScale(TVector3F(50.0f, 0.1f, 50.0f));
	plane->SetCastShadows(true);
	plane->SetModel(cache->LoadResource<AModel>("Model/Box.mdl"));
	plane->SetMaterial(cache->LoadResource<AMaterial>("Stone.json"));

	AStaticModel* teaPot = scene->CreateComponent<AStaticModel>();
	teaPot->SetLayerName("StaticModel");
	teaPot->SetPosition(TVector3F(0.0f, 0.0f, 0.0f));
	teaPot->SetScale(10.0f);
	teaPot->SetModel(cache->LoadResource<AModel>("Model/TeaPot.mdl"));
	teaPot->SetCastShadows(true);

	//Light directional point and spot
	{
		// Dir light
		ALight* lightDir = scene->CreateComponent<ALight>();
		lightDir->SetLightType(ELightType::DIRECTIONAL);
		lightDir->SetCastShadows(true);
		lightDir->SetColor(FColor(1.0f, 1.0f, 1.0f));
		lightDir->SetDirection(TVector3F(0.0f, -1.0f, 0.5f));
		lightDir->SetShadowMapSize(2048);

		// Point light
		/*ALight* lightPoint = scene->CreateComponent<ALight>();
		lightPoint->SetLightType(ELightType::POINT);
		lightPoint->SetCastShadows(true);
		lightPoint->SetColor(FColor(0.0f, 1.0f, 0.0f));
		lightPoint->SetRange(20.0f);
		lightPoint->SetPosition(TVector3F(-10.0f, 10.0f, 0.0f));
		lightPoint->SetShadowMapSize(2048);

		// Spot light
		ALight* lightSpot = scene->CreateComponent<ALight>();
		lightSpot->SetLightType(ELightType::SPOT);
		lightSpot->SetCastShadows(true);
		lightSpot->SetColor(FColor(1.0f, 1.0f, 1.0f));
		lightSpot->SetPosition(TVector3F(10.0f, 10.0f, 0.0f));
		lightSpot->SetDirection(TVector3F(-0.5f, -1.0f, 0.0f));
		lightSpot->SetFov(90.0f);
		lightSpot->SetRange(20.0f);
		lightSpot->SetShadowMapSize(2048);*/
	}
	

}
void MeshSample::Update()
{
	Super::Update();
	/*auto input = GModuleManager::Get().InputModule();
	auto graphics = GModuleManager::Get().GraphicsModule();
	auto renderer = GModuleManager::Get().RendererModule();
	auto time = GModuleManager::Get().TimeModule();

	pitch += input->GetMouseMove()._y * 0.25f;
	yaw += input->GetMouseMove()._x * 0.25f;
	pitch = Clamp(pitch, -90.0f, 90.0f);

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 50 : 10.0f;

	camera->SetRotation(FQuaternion(pitch, yaw, 0.0f));
	if (input->IsKeyDown(KEY_W))
		camera->Translate(TVector3F::FORWARD * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		camera->Translate(TVector3F::BACK * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		camera->Translate(TVector3F::LEFT * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		camera->Translate(TVector3F::RIGHT * time->GetDeltaTime() * moveSpeed);*/
}

void MeshSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(MeshSample)