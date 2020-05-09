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
#include "Component/DirectionalLightComponent.h"
#include "Component/SpotLightComponent.h"
#include "Component/PointLightComponent.h"

#include "IO/FileSystem.h"
#include "IO/Stream.h"
#include "IO/File.h"
#include "Resource/ResourceCache.h"
#include "Resource/Material.h"
#include "Time/Time.h"
#include "Resource/Mesh.h"
#include "Resource/Sound.h"
#include "Component/ColliderBoxComponent.h"
#include "Component/RigidBodyComponent.h"

using namespace Auto3D;

namespace
{

class ExamplePhysics : public Auto3D::IAppInstance
{
public:
	ExamplePhysics(const char* _name, const char* _description, const char* _url)
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
		ASkyboxComponent* skyboxComponent = world->CreateComponent<ASkyboxComponent>();
		skyboxComponent->SetIBLTexture(resourceModule.LoadResource<OTexture>("Textures/bolonga_lod.dds"),
			resourceModule.LoadResource<OTexture>("Textures/bolonga_irr.dds"));


		AActor* actor = world->CreateChild<AActor>();
		ACameraComponent* camera = actor->CreateComponent<ACameraComponent>();
		camera->SetFov(60.0f);
		camera->SetNearClip(0.1f);
		camera->SetFarClip(2000.0f);
		ADefaultControllerComponent* controller = actor->CreateComponent<ADefaultControllerComponent>();
		controller->SetMoveSpeed(50.0f);
		actor->GetTransform()->SetPosition({ 0.0f, 30.0f, -60.0f });

		AActor* cube = world->CreateChild<AActor>();
		cube->GetTransform()->SetPosition({ 0.0f, 50.0f, 0.0f });
		cube->GetTransform()->SetRotation(FQuaternion(0.0f, 0.0f, 0.0f));
		cube->GetTransform()->SetScale({ 4.0f, 4.0f, 4.0f });
		AMeshComponent* meshComponent = cube->CreateComponent<AMeshComponent>();
		meshComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin"));
		ARigidBodyComponent* cubeRigidBody = cube->CreateComponent<ARigidBodyComponent>();
		cubeRigidBody->SetMass(1.0f);
		AColliderBoxComponent* cubeColliderBox = cube->CreateComponent<AColliderBoxComponent>();
		cubeColliderBox->SetSize({ 4.0f, 4.0f, 4.0f });


		AActor* plane = world->CreateChild<AActor>();
		plane->GetTransform()->SetScale({ 500.0f, 500.0f, 500.0f });
		plane->GetTransform()->SetPosition({ 0.0f, -500.0f, 0.0f });
		AMeshComponent* planeMeshComponent = plane->CreateComponent<AMeshComponent>();
		planeMeshComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin"));
		ARigidBodyComponent* fenceRigidBody = plane->CreateComponent<ARigidBodyComponent>();
		fenceRigidBody->SetMass(0.0f);
		AColliderBoxComponent* fenceColliderBox = plane->CreateComponent<AColliderBoxComponent>();
		fenceColliderBox->SetSize({ 250.0f, 500.0f, 250.0f });


		AActor* directionalLightActor = world->CreateChild<AActor>();
		directionalLightActor->GetTransform()->SetPosition({ 25.0f, 25.0f, 25.0f });
		directionalLightActor->CreateComponent<ADirectionalLightComponent>();
	
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

int Auto3D_main(int _argc, char** _argv)
{
	ExamplePhysics app(
		 "01-mesh"
		, "Loading meshes."
		, "https://bkaradzic.github.io/bgfx/examples.html#mesh"
	);
	return GApplication::Get().RunAppInstance(&app, _argc, _argv);
}

//ENTRY_IMPLEMENT_MAIN(
//	  ExamplePhysics
//	, "04-mesh"
//	, "Loading meshes."
//	, "https://bkaradzic.github.io/bgfx/examples.html#mesh"
//	);
