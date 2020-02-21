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
#include "Audio/AudioListener.h"
#include "Audio/AudioSource.h"

#include "Serialization/ToJson.h"
#include "Serialization/FromJson.h"

#include "IO/FileSystem.h"
#include "IO/Stream.h"
#include "IO/File.h"
#include "Resource/ResourceCache.h"
#include "Resource/Material.h"
#include "Time/Time.h"
#include "Resource/Mesh.h"
#include "Resource/Sound.h"


using namespace Auto3D;

namespace
{

class ExampleAudio : public Auto3D::IAppInstance
{
public:
	ExampleAudio(const char* _name, const char* _description, const char* _url)
		: Auto3D::IAppInstance(_name, _description, _url)
	{
	}

	void init() override
	{
		GResourceModule::Get().AddResourceDir(ExecutableDir() + "Data");

		auto sound = GResourceModule::Get().LoadResource<OSound>("Sound/SoundTest.wav");

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

	/*	AActor* cube = world->CreateChild<AActor>();
		cube->GetTransform()->SetPosition({ 0.0f, 10.0f, 0.0f });
		cube->GetTransform()->SetRotation(FQuaternion(0.0f, 0.0f, 0.0f));
		cube->GetTransform()->SetScale({ 4.0f, 4.0f, 4.0f });
		AMeshComponent* meshComponent = cube->CreateComponent<AMeshComponent>();
		meshComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin"));
		meshComponent->SetMaterial(GResourceModule::Get().LoadResource<OMaterial>("Material/Test.json"));

		AActor* light = world->CreateChild<AActor>();
		light->GetTransform()->SetPosition({ 10.0f, 10.0f, 10.0f });
		light->GetTransform()->SetRotation(FQuaternion(45.0f, -45.0f, 45.0f));
		auto lightMeshComponent = light->CreateComponent<ALightComponent>();
		lightMeshComponent->SetupShadowMap();*/

		AActor* listenerActor = world->CreateChild<AActor>();
		listener = listenerActor->CreateComponent<AAudioListener>();

		AActor* sourceActor1 = world->CreateChild<AActor>();
		source1 = sourceActor1->CreateComponent<AAudioSource>();
		source1->SetSound(sound);

		AActor* sourceActor2 = world->CreateChild<AActor>();
		source2 = sourceActor2->CreateComponent<AAudioSource>();
		source2->SetSound(sound);

	
	}

	int shutdown() override
	{

		return 0;
	}

	bool update() override
	{

		showExampleDialog(this);
		ImGui::SetNextWindowPos(
			ImVec2(400.0, 50.0f)
			, ImGuiCond_FirstUseEver
		);
		ImGui::SetNextWindowSize(
			ImVec2(90.0f, 150.0f)
			, ImGuiCond_FirstUseEver
		);
		ImGui::Begin("Sound-1");
		if (ImGui::Button("Play"))
			source1->Play();
		if (ImGui::Button("Pause"))
			source1->Pause();
		if (ImGui::Button("Stop"))
			source1->Stop();
		if (ImGui::Button("Rewind"))
			source1->Rewind();
		ImGui::End();

		ImGui::SetNextWindowPos(
			ImVec2(500.0f, 50.0f)
			, ImGuiCond_FirstUseEver
		);
		ImGui::SetNextWindowSize(
			ImVec2(90.0f, 150.0f)
			, ImGuiCond_FirstUseEver
		);
		ImGui::Begin("Sound-2");
		if (ImGui::Button("Play"))
			source2->Play();
		if (ImGui::Button("Pause"))
			source2->Pause();
		if (ImGui::Button("Stop"))
			source2->Stop();
		if (ImGui::Button("Rewind"))
			source2->Rewind();
		ImGui::End();
		return true;
	}
	AAudioListener* listener;
	AAudioSource* source1;
	AAudioSource* source2;
};

} // namespace

int Auto3D_main(int _argc, char** _argv)
{
	ExampleAudio app(
		 "01-mesh"
		, "Loading meshes."
		, "https://bkaradzic.github.io/bgfx/examples.html#mesh"
	);
	return GApplication::Get().RunAppInstance(&app, _argc, _argv);
}

//ENTRY_IMPLEMENT_MAIN(
//	  ExampleAudio
//	, "04-mesh"
//	, "Loading meshes."
//	, "https://bkaradzic.github.io/bgfx/examples.html#mesh"
//	);