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
		GResourceModule& resourceModule = GResourceModule::Get();

		AWorld* world = FWorldContext::Get().NewWorld();
		world->SetName("world");
		world->DefineLayer(0, "Default");
		world->DefineLayer(1, "UI");
		world->DefineTag(0, "Default");
		world->DefineTag(1, "Player");
		ASkyboxComponent* skybox = world->CreateComponent<ASkyboxComponent>();
		skybox->SetHdrTexture(resourceModule.LoadResource<OTexture>("textures/skybox/sculpture_exhibition_2k.hdr"));
		OTexture* bolongaLod = resourceModule.LoadResource<OTexture>("Textures/bolonga_lod.dds");
		OTexture* bolongaIrr = resourceModule.LoadResource<OTexture>("Textures/bolonga_irr.dds");
		skybox->SetIBLTexture(bolongaLod, bolongaIrr);

		AActor* actor = world->CreateChild<AActor>();
		ACameraComponent* camera = actor->CreateComponent<ACameraComponent>();
		camera->SetFov(60.0f);
		camera->SetNearClip(0.1f);
		camera->SetFarClip(2000.0f);
		ADefaultControllerComponent* controller = actor->CreateComponent<ADefaultControllerComponent>();
		controller->SetMoveSpeed(3.0f);
		actor->GetTransform()->SetPosition({ 0.0f, 0.0f, -3.0f });
		actor->GetTransform()->SetRotation({ 0.0f,0.0f,0.0f });

		AActor* pbrActor = world->CreateChild<AActor>();
		pbrActor->GetTransform()->SetPosition({ 0.0f, -0.5f, 0.0f });
		pbrActor->GetTransform()->SetRotation(FQuaternion(0.0f, 0.0f, 0.0f));
		AMeshComponent* meshComponent = pbrActor->CreateComponent<AMeshComponent>();
		meshComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube_uv.bin"));
		//meshComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/sphere_uv.bin"));

		meshComponent->SetMaterial(GResourceModule::Get().LoadResource<OMaterial>("Material/Pbr.json"));

		FForwardShadingRenderer::s_albedoMap = GResourceModule::Get().LoadResource<OTexture>("Textures/PBR/Gold/albedo.png");
		FForwardShadingRenderer::s_normalMap = GResourceModule::Get().LoadResource<OTexture>("Textures/PBR/Gold/normal.png");
		FForwardShadingRenderer::s_metallicMap = GResourceModule::Get().LoadResource<OTexture>("Textures/PBR/Gold/metallic.png");
		FForwardShadingRenderer::s_roughnessMap = GResourceModule::Get().LoadResource<OTexture>("Textures/PBR/Gold/roughness.png");
		FForwardShadingRenderer::s_aoMap = GResourceModule::Get().LoadResource<OTexture>("Textures/PBR/Gold/ao.png");


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
