#include "TestSample.h"
#include "../../Auto3D/ThirdParty/glad/glad.h"

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
	//Object::Subsystem<RegisteredBox>()->RegisterScene(scene, camera);
	
	Vector<Constant> constants;
	_vsFrameConstantBuffer = new ConstantBuffer();
	constants.Push(Constant(ElementType::MATRIX3X4, "viewMatrix"));
	constants.Push(Constant(ElementType::MATRIX4, "projectionMatrix"));
	constants.Push(Constant(ElementType::MATRIX4, "viewProjMatrix"));
	_vsFrameConstantBuffer->Define(ResourceUsage::DEFAULT, constants);

	_psFrameConstantBuffer = new ConstantBuffer();
	constants.Clear();
	constants.Push(Constant(ElementType::VECTOR4, "color"));
	_psFrameConstantBuffer->Define(ResourceUsage::DEFAULT, constants);




	SharedPtr<Shader> vs = new Shader();
	SharedPtr<Shader> ps = new Shader();
	vs = cache->LoadResource<Shader>("SkyBox.vert");
	ps = cache->LoadResource<Shader>("SkyBox.frag");
	_vsv = vs->CreateVariation();
	_psv = ps->CreateVariation();

	Image* right = cache->LoadResource<Image>("skybox/arrakisday_bk.tga");
	Image* left = cache->LoadResource<Image>("skybox/arrakisday_bk.tga");
	Image* top = cache->LoadResource<Image>("skybox/arrakisday_bk.tga");
	Image* bottom = cache->LoadResource<Image>("skybox/arrakisday_bk.tga");
	Image* front = cache->LoadResource<Image>("skybox/arrakisday_bk.tga");
	Image* back = cache->LoadResource<Image>("skybox/arrakisday_bk.tga");

	skyBoxBuffer = new SkyBoxBuffer(right, left, top, bottom, front, back);

	SkyBox* skyBox = new SkyBox();
	skyBox->SetImage(skyBoxBuffer);

	textureCube = new Texture();
	Vector<ImageLevel> faces;
	faces.Push(right->GetLevel(0));
	faces.Push(left->GetLevel(0));
	faces.Push(top->GetLevel(0));
	faces.Push(bottom->GetLevel(0));
	faces.Push(front->GetLevel(0));
	faces.Push(back->GetLevel(0));


	textureCube->Define(TextureType::TEX_CUBE, ResourceUsage::DEFAULT, Vector2I(1, 1), ImageFormat::RGBA32F, 1, &faces[0]);
	textureCube->DefineSampler(TextureFilterMode::FILTER_POINT, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP);
	textureCube->SetDataLost(false);


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

	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

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

	///Render
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto* _graphics = Subsystem<Graphics>();

		// Set per-frame values to the frame constant buffers
		Matrix3x4F viewMatrix = camera->GetViewMatrix();
		Matrix4x4F projectionMatrix = camera->GetProjectionMatrix();
		Matrix4x4F viewProjMatrix = projectionMatrix * viewMatrix;
		Vector4F depthParameters(Vector4F::ZERO);
		depthParameters._x = camera->GetNearClip();
		depthParameters._y = camera->GetFarClip();
		if (camera->IsOrthographic())
		{
			depthParameters._z = 1.0f;
		}
		else
			depthParameters._w = 1.0f / camera->GetFarClip();

		_vsFrameConstantBuffer->SetConstant(VS_FRAME_VIEW_MATRIX, viewMatrix);
		_vsFrameConstantBuffer->SetConstant(VS_FRAME_PROJECTION_MATRIX, projectionMatrix);
		_vsFrameConstantBuffer->SetConstant(VS_FRAME_VIEWPROJ_MATRIX, viewProjMatrix);

		_vsFrameConstantBuffer->SetConstant(VS_FRAME_DEPTH_PARAMETERS, depthParameters);

		_vsFrameConstantBuffer->Apply();

		_psFrameConstantBuffer->SetConstant((size_t)0, Color::WHITE);
		_psFrameConstantBuffer->Apply();


		_graphics->SetConstantBuffer(ShaderStage::VS, UIConstantBuffer::FRAME, _vsFrameConstantBuffer);
		_graphics->SetConstantBuffer(ShaderStage::PS, UIConstantBuffer::FRAME, _psFrameConstantBuffer);
	}

}

void TestSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(TestSample)