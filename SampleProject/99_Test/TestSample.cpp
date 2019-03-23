#include "TestSample.h"

void TestSample::Init()
{
	Super::Init();
}

void TestSample::Start()
{
	Super::Start();
	auto* cache = Object::Subsystem<ResourceCache>();

	scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	// Register scene to scene system use to render
	Object::Subsystem<RegisteredBox>()->RegisterScene(scene, camera);

	SharedPtr<Shader> vs = new Shader();
	SharedPtr<Shader> ps = new Shader();
	vs = cache->LoadResource<Shader>("SkyBox.vert");
	ps = cache->LoadResource<Shader>("SkyBox.frag");
	_vsv = vs->CreateVariation();
	_psv = ps->CreateVariation();

	Image* right = cache->LoadResource<Image>("skybox/right.jpg");
	Image* left = cache->LoadResource<Image>("skybox/left.jpg");
	Image* top = cache->LoadResource<Image>("skybox/top.jpg");
	Image* bottom = cache->LoadResource<Image>("skybox/bottom.jpg");
	Image* front = cache->LoadResource<Image>("skybox/front.jpg");
	Image* back = cache->LoadResource<Image>("skybox/back.jpg");

	AutoPtr<SkyBoxBuffer> skyBoxBuffer = new SkyBoxBuffer(right, left, top, bottom, front, back);

	SkyBox* skyBox = new SkyBox();
	skyBox->SetImage(skyBoxBuffer);

	AutoPtr<Texture> textureCube = new Texture();

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	Vector<ImageLevel> faces;
	for (size_t i = 0; i < MAX_CUBE_FACES; ++i)
	{
		ImageLevel level;
		level._rowSize = 3 * sizeof(float);
		level._data = (unsigned char*)&skyboxVertices[3 * i];
		faces.Push(level);
	}

	textureCube->Define(TextureType::TEX_CUBE, ResourceUsage::DEFAULT, Vector2I(1, 1), ImageFormat::RGBA32F, 1, &faces[0]);
	textureCube->DefineSampler(TextureFilterMode::FILTER_POINT, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP);
	textureCube->SetDataLost(false);


}

void TestSample::Update()
{
	Super::Update();
	auto* input = Subsystem<Input>();
	auto* time = Subsystem<Time>();

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
}

void TestSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(TestSample)