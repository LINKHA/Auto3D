#include "TestSample.h"
#include "../../Auto3D/ThirdParty/glad/glad.h"
#include "../../Auto3D/ThirdParty/STB/stb_image.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

unsigned int cubemapTexture;
unsigned int skyboxVBO;

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

unsigned int loadCubemap(Vector<String> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.Size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].CString(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			ErrorString("Cubemap texture failed to load at path: ");
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


void TestSample::Init()
{
	Super::Init();
}

void TestSample::Start()
{
	Super::Start();
	auto* cache = Object::Subsystem<ResourceCache>();
	auto* graphics = Object::Subsystem<Graphics>();

	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	// Register scene to scene system use to render
	//Object::Subsystem<RegisteredBox>()->RegisterScene(scene, camera);
	{
		//glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		//glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		Vector<Constant> constants;
		_vsFrameConstantBuffer = new ConstantBuffer();
		constants.Push(Constant(ElementType::MATRIX3X4, "viewMatrix"));
		constants.Push(Constant(ElementType::MATRIX4, "projectionMatrix"));
		constants.Push(Constant(ElementType::MATRIX4, "viewProjMatrix"));
		constants.Push(Constant(ElementType::VECTOR4, "depthParameters"));
		_vsFrameConstantBuffer->Define(ResourceUsage::DEFAULT, constants);

		SharedPtr<Shader> vs = new Shader();
		SharedPtr<Shader> ps = new Shader();
		vs = cache->LoadResource<Shader>("SkyBox.vert");
		ps = cache->LoadResource<Shader>("SkyBox.frag");
		_vsv = vs->CreateVariation();
		_psv = ps->CreateVariation();

		Image* right = cache->LoadResource<Image>("skybox/arrakisday_ft.tga");
		Image* left = cache->LoadResource<Image>("skybox/arrakisday_bk.tga");
		Image* top = cache->LoadResource<Image>("skybox/arrakisday_up.tga");
		Image* bottom = cache->LoadResource<Image>("skybox/arrakisday_dn.tga");
		Image* front = cache->LoadResource<Image>("skybox/arrakisday_rt.tga");
		Image* back = cache->LoadResource<Image>("skybox/arrakisday_lf.tga");

		skyBoxBuffer = new SkyBoxBuffer(right, left, top, bottom, front, back);
		SkyBox* skyBox = new SkyBox();
		skyBox->SetImage(skyBoxBuffer);
		Vector<ImageLevel> faces;
		faces.Push(right->GetLevel(0));
		faces.Push(left->GetLevel(0));
		faces.Push(top->GetLevel(0));
		faces.Push(bottom->GetLevel(0));
		faces.Push(front->GetLevel(0));
		faces.Push(back->GetLevel(0));

		textureCube = new Texture();
		textureCube->Define(TextureType::TEX_CUBE, ResourceUsage::DEFAULT, right->GetLevel(0)._size, right->GetFormat(), 1, &faces[0]);
		textureCube->DefineSampler(TextureFilterMode::FILTER_POINT, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP);
		textureCube->SetDataLost(false);

	}
	
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
	{
		// Set per-frame values to the frame constant buffers
		Matrix3x4F viewMatrix = camera->GetViewMatrix();
		// Remove translation from the view matrix
		viewMatrix._m03 = 0;
		viewMatrix._m13 = 0;
		viewMatrix._m23 = 0;

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

		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		
		_vsFrameConstantBuffer->SetConstant(VS_FRAME_VIEW_MATRIX, viewMatrix);
		_vsFrameConstantBuffer->SetConstant(VS_FRAME_PROJECTION_MATRIX, projectionMatrix);
		_vsFrameConstantBuffer->SetConstant(VS_FRAME_VIEWPROJ_MATRIX, viewProjMatrix);
		_vsFrameConstantBuffer->SetConstant(VS_FRAME_DEPTH_PARAMETERS, depthParameters);
		_vsFrameConstantBuffer->Apply();

		auto graphics = Subsystem<Graphics>();
		graphics->SetConstantBuffer(ShaderStage::VS, UIConstantBuffer::FRAME, _vsFrameConstantBuffer);
		//graphics->SetConstantBuffer(ShaderStage::PS, UIConstantBuffer::FRAME, _psFrameConstantBuffer);

		graphics->SetShaders(_vsv, _psv);

		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

		graphics->SetTexture(0, textureCube);


		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS); // set depth function back to default
	}

}

void TestSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(TestSample)