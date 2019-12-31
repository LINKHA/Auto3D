#include "SerializeSample.h"

FString fileJsonName = "12_Serialize_SerializeFile.json";
FString fileDataName = "12_Serialize_SerializeFile.dat";
FString fileSavName = "12_Serialize_SerializeFile.sav";

void SerializeSample::Init()
{
	Super::Init();
	auto* graphics = GModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("Serialize Sample");
}
void SerializeSample::Start()
{
	Super::Start();
	auto* cache = GModuleManager::Get().CacheModule();
	auto* graphics = GModuleManager::Get().GraphicsModule();
	auto* ui = GModuleManager::Get().UiModule();

	AFont* msyh = cache->LoadResource<AFont>("Font/msyh.ttc");
	ui->AddFont(msyh, 26, "Msyh_26");
	scene = AObject::Create<AWorld>();
	scene->DefineLayer(1, "Layer1");
	scene->DefineLayer(2, "Layer2");
	scene->DefineTag(1, "Tag1");
	scene->DefineTag(2, "Tag2");

	//If no scenario has been created,create it.
	TAutoPtr<FFile> jsonFile(new FFile());
	if (jsonFile->Open(ExecutableDir() + fileJsonName, EFileMode::READ) == false)
	{
		scene->SetupShadowMap(1, 4096);
		scene->CreateChildNode<AOctree>();
		camera = scene->CreateComponent<ACamera>();
		camera->SetPosition(TVector3F(0.0f, 5.0f, -15.0f));
		camera->SetAmbientColor(FColor(0.1f, 0.1f, 0.1f));

		AStaticModel* plane = scene->CreateComponent<AStaticModel>();
		plane->SetTagName("Tag1");
		plane->SetScale(TVector3F(50.0f, 0.1f, 50.0f));
		plane->SetCastShadows(true);
		plane->SetModel(cache->LoadResource<AModel>("Model/Box.mdl"));
		plane->SetMaterial(cache->LoadResource<AMaterial>("Stone.json"));

		AStaticModel* teaPot = scene->CreateComponent<AStaticModel>();
		teaPot->SetLayerName("Layer1");
		teaPot->SetPosition(TVector3F(0.0f, 0.0f, 0.0f));
		teaPot->SetScale(TVector3F(10.0f, 10.0f, 10.0f));
		teaPot->SetModel(cache->LoadResource<AModel>("Model/TeaPot.mdl"));
		teaPot->SetCastShadows(true);

		ALight* lightDir = scene->CreateComponent<ALight>();
		lightDir->SetLightType(ELightType::DIRECTIONAL);
		lightDir->SetCastShadows(true);
		lightDir->SetColor(FColor(1.0f, 1.0f, 1.0f));
		lightDir->SetDirection(TVector3F(0.0f, -1.0f, 0.5f));
		lightDir->SetShadowMapSize(2048);
	}
}
void SerializeSample::Update()
{
	Super::Update();
	auto* serialization = GModuleManager::Get().SerializationModule();

	GUI::Begin("Serialization");
	GUI::Text(FString(FString("JSON File : ") + ExecutableDir() + fileJsonName).CString());
	GUI::Text(FString(FString("Data File : ") + ExecutableDir() + fileDataName).CString());
	GUI::Text(FString(FString("Sav File : ") + ExecutableDir() + fileSavName).CString());

	//if (GUI::Button("Clear scene"))
	//{
	//	scene->Clear();
	//}

	if (GUI::Button("Save scene to JSON"))
	{
		// Serialize scene to json
		TAutoPtr<FStream> streamJson(new FFile(ExecutableDir() + fileJsonName, EFileMode::WRITE));
		serialization->SaveRootJSON(*streamJson, scene);
	}

	if (GUI::Button("Load scene from JSON"))
	{
		// Deserialization scene to json
		TAutoPtr<FStream> streamJson(new FFile(ExecutableDir() + fileJsonName, EFileMode::READ));
		serialization->LoadRootJSON(*streamJson, scene);
	}

	if (GUI::Button("Save scene to data"))
	{
		// Serialize scene to json
		TAutoPtr<FStream> streamJson(new FFile(ExecutableDir() + fileDataName, EFileMode::WRITE));
		serialization->SaveRoot(*streamJson, scene);
	}

	if (GUI::Button("Load scene from data"))
	{
		// Deserialization scene to json
		TAutoPtr<FStream> streamJson(new FFile(ExecutableDir() + fileDataName, EFileMode::READ));
		serialization->LoadRoot(*streamJson, scene);
	}

	if (GUI::Button("Save file test"))
	{
		// Save data to file
		TAutoPtr<FStream> streamSave(new FFile(ExecutableDir() + fileSavName, EFileMode::WRITE));
		streamSave->Write<int>(1);
		streamSave->Write<float>(1.0f);
		streamSave->Write<FString>("stringhash");
	}

	if (GUI::Button("Load file test"))
	{
		// Save data to file
		TAutoPtr<FStream> streamSave(new FFile(ExecutableDir() + fileSavName, EFileMode::READ));
		streamSave->Read<int>();
		streamSave->Read<float>();
		streamSave->Read<FString>();
	}

	GUI::End();
}

void SerializeSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(SerializeSample)