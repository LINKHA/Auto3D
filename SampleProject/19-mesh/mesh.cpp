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
#include "Component/LightComponent.h"

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

	void init() override
	{
		GResourceModule::Get().AddResourceDir(ExecutableDir() + "Data");
		m_mesh = GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin");

		_material = GResourceModule::Get().LoadResource<OMaterial>("Material/Test.json");
		_material2 = GResourceModule::Get().LoadResource<OMaterial>("Material/MeshShadowTest.json");

		AWorld* world = FWorldContext::Get().NewWorld();
		world->SetName("world");
		world->DefineLayer(0, "Default");
		world->DefineLayer(1, "UI");
		world->DefineTag(0, "Default");
		world->DefineTag(1, "Player");

		AActor* actor = world->CreateChild<AActor>();
		ACameraComponent* camera = actor->CreateComponent<ACameraComponent>();
		actor->CreateComponent<ADefaultControllerComponent>();
		actor->GetTransform()->SetPosition({ 0.0f, 30.0f, -60.0f });

		AActor* cube = world->CreateChild<AActor>();
		cube->GetTransform()->SetPosition({ 0.0f, 10.0f, 0.0f });
		cube->GetTransform()->SetRotation(FQuaternion(0.0f, 0.0f, 0.0f));
		cube->GetTransform()->SetScale({ 4.0f, 4.0f, 4.0f });
		AMeshComponent* meshComponent = cube->CreateComponent<AMeshComponent>();
		meshComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin"));
		meshComponent->SetMaterial(_material2);

		AActor* plane = world->CreateChild<AActor>();
		plane->GetTransform()->SetPosition({ 0.0f, 0.0f, 0.0f });
		plane->GetTransform()->SetRotation(FQuaternion(0.0f, 0.0f, 0.0f));
		plane->GetTransform()->SetScale({ 50.0f, 1.0f, 50.0f });
		AMeshComponent* planeMeshComponent = plane->CreateComponent<AMeshComponent>();
		planeMeshComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube2.bin"));
		planeMeshComponent->SetMaterial(_material2);

		AActor* light = world->CreateChild<AActor>();
		light->GetTransform()->SetPosition({ 10.0f, 10.0f, 10.0f });
		light->GetTransform()->SetRotation(FQuaternion(45.0f, -45.0f, 45.0f));
		auto lightMeshComponent = light->CreateComponent<ALightComponent>();
		lightMeshComponent->SetupShadowMap();


		/*FString fileJsonName = "Serialize_SerializeFile.json";
		UPtr<FStream> streamJson(new FFile(ExecutableDir() + fileJsonName, EFileMode::WRITE));
		world->SaveJson(*streamJson);*/

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

	OMesh* m_mesh;
	OMaterial* _material;
	OMaterial* _material2;

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
	return GApplication::Get().RunAppInstance(&app, _argc, _argv);
}

//ENTRY_IMPLEMENT_MAIN(
//	  ExampleMesh
//	, "04-mesh"
//	, "Loading meshes."
//	, "https://bkaradzic.github.io/bgfx/examples.html#mesh"
//	);
