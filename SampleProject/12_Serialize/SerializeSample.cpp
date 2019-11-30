#include "SerializeSample.h"

void SerializeSample::Init()
{
	Super::Init();
	auto* graphics = GModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("Mesh Sample");
}
void SerializeSample::Start()
{
	Super::Start();
	auto* cache = GModuleManager::Get().CacheModule();
	auto* graphics = GModuleManager::Get().GraphicsModule();

	scene = AObject::Create<AScene>();
	/*scene->SetupShadowMap(3, 4096);
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	camera->SetPosition(Vector3F(0.0f, 5.0f, -15.0f));
	camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));

	StaticModel* plane = scene->CreateChild<StaticModel>();
	plane->SetScale(Vector3F(50.0f, 0.1f, 50.0f));
	plane->SetCastShadows(true);
	plane->SetModel(cache->LoadResource<Model>("Model/Box.mdl"));
	plane->SetMaterial(cache->LoadResource<Material>("Stone.json"));

	StaticModel* teaPot = scene->CreateChild<StaticModel>();
	teaPot->SetPosition(Vector3F(0.0f, 0.0f, 0.0f));
	teaPot->SetScale(10.0f);
	teaPot->SetModel(cache->LoadResource<Model>("Model/TeaPot.mdl"));
	teaPot->SetCastShadows(true);*/

	//Light* lightDir = scene->CreateChild<Light>();
	//lightDir->SetLightType(LightType::DIRECTIONAL);
	//lightDir->SetCastShadows(true);
	//lightDir->SetColor(Color(1.0f, 1.0f, 1.0f));
	//lightDir->SetDirection(Vector3F(0.0f, -1.0f, 0.5f));
	//lightDir->SetShadowMapSize(2048);
	//

	FString exePath = ExecutableDir();
	FString fileJsonName = "12_Serialize_SerializeFile.json";
	FString fileSavName = "12_Serialize_SerializeFile.sav";
	// Serialize
	{
		//// Serialize scene to json
		//AutoPtr<Stream> streamJson(new File(exePath + fileJsonName, FileMode::WRITE));
		//scene->SaveJSON(*streamJson);

		//// Save data to file
		//AutoPtr<Stream> streamSave(new File(exePath + fileSavName, FileMode::WRITE));
		//streamSave->Write<int>(1);
		//streamSave->Write<float>(1.0f);
		//streamSave->Write<String>("stringhash");
	}

	// Deserialize
	{
		// Serialize scene to json
		TAutoPtr<FStream> streamJson(new FFile(exePath + fileJsonName, EFileMode::READ));
		scene->LoadJSON(*streamJson);

		// Save data to file
		TAutoPtr<FStream> streamSave(new FFile(exePath + fileSavName, EFileMode::READ));
		streamSave->Read<int>();
		streamSave->Read<float>();
		streamSave->Read<FString>();
	}
}
void SerializeSample::Update()
{
	Super::Update();

	//GUI::Begin("Hello, world!");

	//if (GUI::Button("counter+"))
	//	counter++;
	//GUI::SameLine();

	//GUI::Text("counter = %d", counter);

	//GUI::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//GUI::End();

}

void SerializeSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(SerializeSample)