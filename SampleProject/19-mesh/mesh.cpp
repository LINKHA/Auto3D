#include "RHI/RHI.h"
#include "RHI/bgfx_utils.h"
#include "UI/UI.h"
#include "Application.h"
#include "Component/MeshComponent.h"
#include "Renderer/ForwardShadingRenderer.h"
#include "Gameplay/Actor.h"
#include "Component/CameraComponent.h"
#include "Gameplay/World.h"
#include "Gameplay/WorldContext.h"
#include "Component/DefaultControllerComponent.h"
#include "Component/TransformComponent.h"

#include "Serialization/ToJson.h"
#include "Serialization/FromJson.h"

#include "IO/FileSystem.h"
#include "IO/Stream.h"
#include "IO/File.h"
#include "Resource/ResourceCache.h"
#include "Resource/Material.h"
#include "Time/Time.h"
#include "Resource/Mesh.h"


using namespace Auto3D;

namespace
{

class ExampleMesh : public Auto3D::IAppInstance
{
public:
	ExampleMesh(const char* _name, const char* _description, const char* _url)
		: Auto3D::IAppInstance(_name, _description, _url)
	{
	}

	void init(uint32_t _width, uint32_t _height) override
	{
		FArgs args = FArgs::Get();

		m_width  = _width;
		m_height = _height;
		m_debug  = BGFX_DEBUG_NONE;
		m_reset  = BGFX_RESET_VSYNC;

		GResourceModule::Get().AddResourceDir(ExecutableDir() + "Data");
		m_mesh = GResourceModule::Get().LoadResource<OMesh>("Meshes/bunny.bin");

		_material = GResourceModule::Get().LoadResource<OMaterial>("Material/Test.json");

		AWorld* world = FWorldContext::Get().NewWorld();
		world->SetName("world");
		world->DefineLayer(0, "Default");
		world->DefineLayer(1, "UI");
		world->DefineTag(0, "Default");
		world->DefineTag(1, "Player");

		AActor* actor = world->CreateChild<AActor>();
		ACameraComponent* camera = actor->CreateComponent<ACameraComponent>();
		actor->CreateComponent<ADefaultControllerComponent>();
		actor->GetTransform()->SetPosition({ 0.0f, 1.0f, -2.5f });

		AActor* meshActor = world->CreateChild<AActor>();
		meshActor->GetTransform()->SetPosition({ 0.0f, 0.0f, 1.0f });
		meshActor->GetTransform()->SetRotation(FQuaternion(0.0f, 0.0f, 0.0f));
		meshActor->GetTransform()->SetScale({ 1.0f, 1.0f, 1.0f });
		AMeshComponent* meshComponent = meshActor->CreateComponent<AMeshComponent>();
		meshComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin"));
		meshComponent->SetMaterial(_material);

		AActor* meshActor4 = world->CreateChild<AActor>();
		meshActor4->GetTransform()->SetPosition({ 0.0f, 0.0f, 5.0f });
		meshActor4->GetTransform()->SetScale({ 0.2f, 0.2f, 0.2f });
		AMeshComponent* meshComponent4 = meshActor4->CreateComponent<AMeshComponent>();
		meshComponent4->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin"));
		meshComponent4->SetMaterial(_material);

		AActor* meshActor3 = world->CreateChild<AActor>();
		meshActor3->GetTransform()->SetPosition({ -1.0f, 0.0f, 0.0f });
		AMeshComponent* meshComponent3 = meshActor3->CreateComponent<AMeshComponent>();
		meshComponent3->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin"));
		meshComponent3->SetMaterial(_material);

		AActor* meshActor5 = world->CreateChild<AActor>();
		meshActor5->GetTransform()->SetPosition({ -2.0f, 0.0f, 0.0f });
		AMeshComponent* meshComponent5 = meshActor5->CreateComponent<AMeshComponent>();
		meshComponent5->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin"));
		meshComponent5->SetMaterial(_material);

		/*
		AActor* meshActor7 = world->CreateChild<AActor>();
		meshActor7->GetTransform()->SetPosition({ -3.0f, 0.0f, 0.0f });
		AMeshComponent* meshComponent7 = meshActor7->CreateComponent<AMeshComponent>();
		meshComponent7->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/hollowcube.bin"));
		meshComponent7->SetMaterial(_material);*/

		FString fileJsonName = "Serialize_SerializeFile.json";
		UPtr<FStream> streamJson(new FFile(ExecutableDir() + fileJsonName, EFileMode::WRITE));
		world->SaveJson(*streamJson);
		//ToJson("D:/Project/MyProject/Auto3D/Bin/output_utf32le.json",world); // serialize the circle to 'json_string'

		//FromJson("D:/Project/MyProject/Auto3D/Bin/output_utf32le.json");
	}

	int shutdown() override
	{
		//imguiDestroy();

		m_mesh->unload();

		return 0;
	}

	bool update() override
	{

		showExampleDialog(this);

		return true;
	}

	Auto3D::MouseState m_mouseState;

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;

	int64_t m_timeOffset;
	OMesh* m_mesh;
	OMaterial* _material;

	bgfx::ProgramHandle m_program;
	bgfx::UniformHandle u_time;
};

} // namespace

int Auto3D_main(int _argc, char** _argv)
{
	ExampleMesh app(
		 "04-mesh"
		, "Loading meshes."
		, "https://bkaradzic.github.io/bgfx/examples.html#mesh"
	);
	return FApplication::Get().RunAppInstance(&app, _argc, _argv);
}

//ENTRY_IMPLEMENT_MAIN(
//	  ExampleMesh
//	, "04-mesh"
//	, "Loading meshes."
//	, "https://bkaradzic.github.io/bgfx/examples.html#mesh"
//	);
