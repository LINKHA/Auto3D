#include "RHI/bgfx_utils.h"
#include "Application.h"
#include "UI/UI.h"
#include "Platform/ProcessWindow.h"
#include "Resource/Mesh.h"
#include "Resource/ResourceCache.h"
#include "Renderer/ForwardShadingRenderer.h"
#include "Gameplay/Actor.h"
#include "Component/CameraComponent.h"
#include "Gameplay/World.h"
#include "Gameplay/WorldContext.h"
#include "Component/DefaultControllerComponent.h"
#include "Component/TransformComponent.h"

#include <bx/rng.h>
using namespace Auto3D;

namespace
{
	
class ExampleHDR : public Auto3D::IAppInstance
{
public:
	ExampleHDR(const char* _name, const char* _description, const char* _url)
		: Auto3D::IAppInstance(_name, _description, _url)
	{}

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
		skybox->SetHdrTexture(resourceModule.LoadResource<OTexture>("textures/uffizi.ktx"));

		AActor* actor = world->CreateChild<AActor>();
		ACameraComponent* camera = actor->CreateComponent<ACameraComponent>();
		camera->SetFov(60.0f);
		camera->SetNearClip(0.1f);
		camera->SetFarClip(2000.0f);
		actor->CreateComponent<ADefaultControllerComponent>();
		actor->GetTransform()->SetPosition({ 0.0f, 0.0f, -3.0f });
		actor->GetTransform()->SetRotation({ 0.0f,0.0f,0.0f });

		AActor* pbrActor = world->CreateChild<AActor>();
		pbrActor->GetTransform()->SetPosition({ 0.0f, -0.5f, 0.0f });
		pbrActor->GetTransform()->SetRotation(FQuaternion(0.0f, 0.0f, 0.0f));
		AMeshComponent* meshComponent = pbrActor->CreateComponent<AMeshComponent>();
		//meshComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/hollowcube.bin"));
		meshComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/bunny.bin"));
		meshComponent->SetMaterial(GResourceModule::Get().LoadResource<OMaterial>("Material/Pbr.json"));


		AActor* pbrActor2 = world->CreateChild<AActor>();
		pbrActor2->GetTransform()->SetPosition({ 5.0f, -0.5f, 0.0f });
		pbrActor2->GetTransform()->SetRotation(FQuaternion(0.0f, 0.0f, 0.0f));
		AMeshComponent* meshComponent2 = pbrActor2->CreateComponent<AMeshComponent>();
		meshComponent2->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/hollowcube.bin"));
		meshComponent2->SetMaterial(GResourceModule::Get().LoadResource<OMaterial>("Material/Pbr.json"));
	}

	virtual int shutdown() override
	{
		return 0;
	}

	bool update() override
	{
		GProcessWindow& processWindow = GProcessWindow::Get();
	
		HDRSettings& currentSettings = FForwardShadingRenderer::_hdrPipline._settings;

		showExampleDialog(this);

		ImGui::SetNextWindowPos(
				ImVec2(processWindow._width - processWindow._width / 5.0f - 10.0f, 10.0f)
			, ImGuiCond_FirstUseEver
			);
		ImGui::SetNextWindowSize(
				ImVec2(processWindow._width / 5.0f, processWindow._height / 2.0f)
			, ImGuiCond_FirstUseEver
			);
		ImGui::Begin("Settings"
			, NULL
			, 0
			);
			
		ImGui::SliderFloat("Speed", &currentSettings.m_speed, 0.0f, 1.0f);
		ImGui::Separator();

		ImGui::SliderFloat("Middle gray", &currentSettings.m_middleGray, 0.1f, 1.0f);
		ImGui::SliderFloat("White point", &currentSettings.m_white,      0.1f, 2.0f);
		ImGui::SliderFloat("Threshold",   &currentSettings.m_threshold,  0.1f, 2.0f);

		if (bgfx::isValid(FForwardShadingRenderer::_hdrPipline.m_rb) )
		{
			union { uint32_t color; uint8_t bgra[4]; } cast = { FForwardShadingRenderer::_hdrPipline.m_lumBgra8 };
			float exponent = cast.bgra[3]/255.0f * 255.0f - 128.0f;
			float lumAvg   = cast.bgra[2]/255.0f * bx::exp2(exponent);
			ImGui::SliderFloat("Lum Avg", &lumAvg, 0.0f, 1.0f);
		}

		ImGui::End();

		return true;

	}

};

} // namespace
int Auto3D_main(int _argc, char** _argv)
{
	ExampleHDR app(
		"09-hdr"
		, "Using multiple views with frame buffers, and view order remapping."
		, "https://bkaradzic.github.io/bgfx/examples.html#hdr"
	);
	return GApplication::Get().RunAppInstance(&app, _argc, _argv);
}

