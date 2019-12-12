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
	scene->SetupShadowMap(3, 4096);
	scene->CreateChild<AOctree>();
	camera = scene->CreateChild<ACamera>();
	camera->SetPosition(TVector3F(0.0f, 5.0f, -15.0f));
	camera->SetAmbientColor(FColor(0.1f, 0.1f, 0.1f));

	AStaticModel* plane = scene->CreateChild<AStaticModel>();
	plane->SetScale(TVector3F(50.0f, 0.1f, 50.0f));
	plane->SetCastShadows(true);
	plane->SetModel(cache->LoadResource<AModel>("Model/Box.mdl"));
	plane->SetMaterial(cache->LoadResource<AMaterial>("Stone.json"));

	AStaticModel* teaPot = scene->CreateChild<AStaticModel>();
	teaPot->SetPosition(TVector3F(0.0f, 0.0f, 0.0f));
	teaPot->SetScale(10.0f);
	teaPot->SetModel(cache->LoadResource<AModel>("Model/TeaPot.mdl"));
	teaPot->SetCastShadows(true);

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
		// Serialize scene to json
		TAutoPtr<FStream> streamJson(new FFile(exePath + fileJsonName, EFileMode::WRITE));
		scene->SaveJSON(*streamJson);
		//scene->_SaveJSON(*streamJson);

		// Save data to file
		TAutoPtr<FStream> streamSave(new FFile(exePath + fileSavName, EFileMode::WRITE));
		streamSave->Write<int>(1);
		streamSave->Write<float>(1.0f);
		streamSave->Write<FString>("stringhash");
	}

	// Deserialize
	{
		//// Serialize scene to json
		//TAutoPtr<FStream> streamJson(new FFile(exePath + fileJsonName, EFileMode::READ));
		//scene->LoadJSON(*streamJson);

		//// Save data to file
		//TAutoPtr<FStream> streamSave(new FFile(exePath + fileSavName, EFileMode::READ));
		//streamSave->Read<int>();
		//streamSave->Read<float>();
		//streamSave->Read<FString>();
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