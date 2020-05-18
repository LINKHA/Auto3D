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
		AWorld* world = FWorldContext::Get().NewWorld();
		world->SetName("world");
		world->DefineLayer(0, "Default");
		world->DefineLayer(1, "UI");
		world->DefineTag(0, "Default");
		world->DefineTag(1, "Player");

	}

	int shutdown() override
	{

		return 0;
	}

	bool update() override
	{

		showExampleDialog(this);
		
		return true;
	}

};

} // namespace

AUTO_IMPLEMENT_MAIN(
	ExampleAudio
	,"01-Audio"
	, "audio test."
	, "https://github.com/LINKHA/Auto3D"
	);
