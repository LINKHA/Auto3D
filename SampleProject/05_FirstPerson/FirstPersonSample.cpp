#include "FirstPersonSample.h"

void FirstPersonSample::Init()
{
	Super::Init();
	auto* graphics = Object::Module<Graphics>();
	graphics->RenderWindow()->SetTitle("FirstPerson Sample");

}
void FirstPersonSample::Start()
{
	Super::Start();
	auto* cache = Object::Module<ResourceCache>();
	auto* graphics = Object::Module<Graphics>();
	auto* renderer = Object::Module<Renderer>();
	auto* input = Object::Module<Input>();
	auto* profiler = Object::Module<Profiler>();

	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	scene = new Scene();
	scene->SetupShadowMap(1, 4096);
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	//camera->SetPosition(Vector3F(0.0f, 20.0f, -75.0f));
	camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));

	for (int y = -5; y <= 5; ++y)
	{
		for (int x = -5; x <= 5; ++x)
		{
			StaticModel* object = scene->CreateChild<StaticModel>();
			object->SetPosition(Vector3F(10.5f * x, -0.1f, 10.5f * y));
			object->SetScale(Vector3F(10.0f, 0.1f, 10.0f));
			object->SetModel(cache->LoadResource<Model>("Model/Box.mdl"));
			object->SetMaterial(cache->LoadResource<Material>("Stone.json"));
		}
	}

	for (unsigned i = 0; i < 435; ++i)
	{
		StaticModel* object = scene->CreateChild<StaticModel>();
		object->SetPosition(Vector3F(Random() * 100.0f - 50.0f, 0.0f, Random() * 100.0f - 50.0f));
		object->SetScale(1.5f);
		object->SetModel(cache->LoadResource<Model>("Model/Mushroom.mdl"));
		object->SetMaterial(cache->LoadResource<Material>("Mushroom.json"));
		object->SetCastShadows(true);
		object->SetLodBias(2.0f);
	}

	for (unsigned i = 0; i < 20; ++i)
	{
		Light* light = scene->CreateChild<Light>();
		light->SetLightType(LightType::POINT);
		light->SetCastShadows(true);
		Vector3F colorVec = 2.0f * Vector3F(Random(), Random(), Random()).Normalized();
		light->SetColor(Color(colorVec._x, colorVec._y, colorVec._z));
		light->SetFov(90.0f);
		light->SetRange(15.0f);
		Vector3F positionVec = Vector3F(Random() * 120.0f - 60.0f, 2.0f, Random() * 120.0f - 60.0f);
		light->SetPosition(positionVec);
		light->SetDirection(Vector3F(0.0f, -1.0f, 0.0f));
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
	auto input = ModuleManager::Get().InputModule();
	auto graphics = ModuleManager::Get().GraphicsModule();
	auto renderer = ModuleManager::Get().RendererModule();
	auto time = ModuleManager::Get().TimeModule();

	pitch += input->GetMouseMove()._y * 0.25f;
	yaw += input->GetMouseMove()._x * 0.25f;
	pitch = Clamp(pitch, -90.0f, 90.0f);

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 10.0f : 5.0f;

	camera->SetRotation(Quaternion(pitch, yaw, 0.0f));
	if (input->IsKeyDown(KEY_W))
		camera->Translate(Vector3F::FORWARD * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		camera->Translate(Vector3F::BACK * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		camera->Translate(Vector3F::LEFT * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		camera->Translate(Vector3F::RIGHT * time->GetDeltaTime()  * moveSpeed);

	Vector3F cameraVec = camera->GetPosition();
	camera->SetPosition(Vector3F(cameraVec._x, 1.0f, cameraVec._z));
	
	float scaleAmountx = (float)sin(time->GetCurTime());

	float scaleAmounty = (float)cos(time->GetCurTime());

	for (auto it = lights.Begin(); it != lights.End(); it++)
	{	
		Vector3F pos((*it).position._x + scaleAmountx * (*it).xRand,
			(*it).position._y + scaleAmountx * (*it).yRand,
			(*it).position._z + scaleAmounty * (*it).zRand);
		(*it).light->SetPosition(pos);
		Vector3F colorf = (*it).color + Vector3F(scaleAmountx, scaleAmounty, scaleAmountx);
		(*it).light->SetColor(Color(colorf._x, colorf._y, colorf._z));
	}
}

void FirstPersonSample::Stop()
{
	Super::Stop();
}
AUTO_APPLICATION_MAIN(FirstPersonSample)