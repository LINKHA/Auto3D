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
#include "Component/AudioListenerComponent.h"
#include "Component/AudioSourceComponent.h"

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

		AActor* listenerActor = world->CreateChild<AActor>();
		listener = listenerActor->CreateComponent<AAudioListenerComponent>();

		AActor* sourceActor1 = world->CreateChild<AActor>();
		source1 = sourceActor1->CreateComponent<AAudioSourceComponent>();
		source1->SetSound(sound);

		AActor* sourceActor2 = world->CreateChild<AActor>();
		source2 = sourceActor2->CreateComponent<AAudioSourceComponent>();
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
	AAudioListenerComponent* listener;
	AAudioSourceComponent* source1;
	AAudioSourceComponent* source2;
};

} // namespace

AUTO_IMPLEMENT_MAIN(
	ExampleAudio
	,"01-Audio"
	, "audio test."
	, "https://github.com/LINKHA/Auto3D"
	);
