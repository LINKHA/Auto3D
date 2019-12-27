#include "FirstPersonSample.h"

void FirstPersonSample::Init()
{
	Super::Init();
	auto* graphics = GModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("FirstPerson Sample");

}
void FirstPersonSample::Start()
{
	Super::Start();
	auto* cache = GModuleManager::Get().CacheModule();
	auto* graphics = GModuleManager::Get().GraphicsModule();

	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	scene = AObject::Create<AWorld>();
	scene->SetupShadowMap(1, 4096);
	scene->CreateChildNode<AOctree>();
	camera = scene->CreateChildNode<ACamera>();
	//camera->SetPosition(Vector3F(0.0f, 20.0f, -75.0f));
	camera->SetAmbientColor(FColor(0.1f, 0.1f, 0.1f));

	for (int y = -5; y <= 5; ++y)
	{
		for (int x = -5; x <= 5; ++x)
		{
			AStaticModel* object = scene->CreateChildNode<AStaticModel>();
			object->SetPosition(TVector3F(10.5f * x, -0.1f, 10.5f * y));
			object->SetScale(TVector3F(10.0f, 0.1f, 10.0f));
			object->SetModel(cache->LoadResource<AModel>("Model/Box.mdl"));
			object->SetMaterial(cache->LoadResource<AMaterial>("Stone.json"));
		}
	}

	for (unsigned i = 0; i < 435; ++i)
	{
		AStaticModel* object = scene->CreateChildNode<AStaticModel>();
		object->SetPosition(TVector3F(Random() * 100.0f - 50.0f, 0.0f, Random() * 100.0f - 50.0f));
		object->SetScale(1.5f);
		object->SetModel(cache->LoadResource<AModel>("Model/Mushroom.mdl"));
		object->SetMaterial(cache->LoadResource<AMaterial>("Mushroom.json"));
		object->SetCastShadows(true);
		object->SetLodBias(2.0f);
	}

	for (unsigned i = 0; i < 20; ++i)
	{
		ALight* light = scene->CreateChildNode<ALight>();
		light->SetLightType(ELightType::POINT);
		light->SetCastShadows(true);
		TVector3F colorVec = 2.0f * TVector3F(Random(), Random(), Random()).Normalized();
		light->SetColor(FColor(colorVec._x, colorVec._y, colorVec._z));
		light->SetFov(90.0f);
		light->SetRange(15.0f);
		TVector3F positionVec = TVector3F(Random() * 120.0f - 60.0f, 2.0f, Random() * 120.0f - 60.0f);
		light->SetPosition(positionVec);
		light->SetDirection(TVector3F(0.0f, -1.0f, 0.0f));
		light->SetShadowMapSize(256);
		RandMSG randMsg;
		randMsg.light = light;
		randMsg.color = colorVec;
		randMsg.position = positionVec;
		randMsg.xRand = Random() * 40 - 20;
		randMsg.yRand = Random();
		randMsg.zRand = Random() * 40 - 20;
		lights.Push(randMsg);
	}
}
void FirstPersonSample::Update()
{
	Super::Update();
	auto input = GModuleManager::Get().InputModule();
	auto graphics = GModuleManager::Get().GraphicsModule();
	auto renderer = GModuleManager::Get().RendererModule();
	auto time = GModuleManager::Get().TimeModule();

	pitch += input->GetMouseMove()._y * 0.25f;
	yaw += input->GetMouseMove()._x * 0.25f;
	pitch = Clamp(pitch, -90.0f, 90.0f);

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 10.0f : 5.0f;

	camera->SetRotation(FQuaternion(pitch, yaw, 0.0f));
	if (input->IsKeyDown(KEY_W))
		camera->Translate(TVector3F::FORWARD * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		camera->Translate(TVector3F::BACK * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		camera->Translate(TVector3F::LEFT * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		camera->Translate(TVector3F::RIGHT * time->GetDeltaTime()  * moveSpeed);

	TVector3F cameraVec = camera->GetPosition();
	camera->SetPosition(TVector3F(cameraVec._x, 1.0f, cameraVec._z));
	
	float scaleAmountx = (float)sin(time->GetCurTime());

	float scaleAmounty = (float)cos(time->GetCurTime());

	for (auto it = lights.Begin(); it != lights.End(); it++)
	{	
		TVector3F pos((*it).position._x + scaleAmountx * (*it).xRand,
			(*it).position._y + scaleAmountx * (*it).yRand,
			(*it).position._z + scaleAmounty * (*it).zRand);
		(*it).light->SetPosition(pos);
		TVector3F colorf = (*it).color + TVector3F(scaleAmountx, scaleAmounty, scaleAmountx);
		(*it).light->SetColor(FColor(colorf._x, colorf._y, colorf._z));
	}
}

void FirstPersonSample::Stop()
{
	Super::Stop();
}
AUTO_APPLICATION_MAIN(FirstPersonSample)