#include "PBRSample.h"

void PBRSample::Init()
{
	Super::Init();
	auto* graphics = Object::Subsystem<Graphics>();
	graphics->RenderWindow()->SetTitle("PBR Sample");
}
void PBRSample::Start()
{
	Super::Start();

	auto resourceCache = Subsystem<ResourceCache>();
	auto texture = resourceCache->LoadResource<Texture>("Mt-Washington-Gold-Room_Ref.hdr");


	//Sprite* bakcground = canvas->CreateChild<Sprite>();
	//bakcground->SetTexture(texture);

	scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	//camera->SetPosition(Vector3F(0.0f, 20.0f, -75.0f));
	camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));
	// Register scene to scene system use to render
	Object::Subsystem<RegisteredBox>()->RegisterScene(scene, camera);


	SkyBox* skybox = camera->CreateSkyBox(texture);
	skybox->Init();

	Subsystem<Renderer>()->_skyBox = skybox;
		
}
void PBRSample::Update()
{
	Super::Update();
}

void PBRSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(PBRSample)