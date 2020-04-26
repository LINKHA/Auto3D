#include <vector>
#include <string>
#include "Resource/ResourceCache.h"
#include "Application.h"
#include "Gameplay/World.h"
#include "Gameplay/WorldContext.h"

#include "RHI/RHI.h"
#include "RHI/bgfx_utils.h"
#include "UI/UI.h"
#include "Resource/Mesh.h"
#include "Platform/ProcessWindow.h"
#include "Renderer/ShaderProgram.h"
#include "Component/CameraComponent.h"
#include "Component/DefaultControllerComponent.h"
#include "Gameplay/Actor.h"
#include "Component/TransformComponent.h"
#include "Renderer/IBLRenderer.h"
#include "Component/SkyboxComponent.h"
#include "Component/MeshComponent.h"

#include "Renderer/ForwardShadingRenderer.h"

#include <bx/readerwriter.h>
#include <bx/string.h>

namespace
{
using namespace Auto3D;

class ExampleIbl : public  Auto3D::IAppInstance
{
public:
	ExampleIbl(const char* _name, const char* _description, const char* _url)
		: Auto3D::IAppInstance(_name, _description, _url)
	{
		//IAppInstance::_useCustomRender = true;
	}

	void init() override
	{
		GResourceModule& resourceModule = GResourceModule::Get();
		_bolongaLod = resourceModule.LoadResource<OTexture>("Textures/bolonga_lod.dds");
		_bolongaIrr = resourceModule.LoadResource<OTexture>("Textures/bolonga_irr.dds");
		_kyotoLod = resourceModule.LoadResource<OTexture>("Textures/kyoto_lod.dds");
		_kyotoIrr = resourceModule.LoadResource<OTexture>("Textures/kyoto_irr.dds");

		AWorld* world = FWorldContext::Get().NewWorld();
		world->SetName("world");
		world->DefineLayer(0, "Default");
		world->DefineLayer(1, "UI");
		world->DefineTag(0, "Default");
		world->DefineTag(1, "Player");
		_skyboxComponent = world->CreateComponent<ASkyboxComponent>();
		_skyboxComponent->SetTexture(_bolongaLod);
		_skyboxComponent->SetIrranceTexture(_bolongaIrr);

		AActor* actor = world->CreateChild<AActor>();
		ACameraComponent* camera = actor->CreateComponent<ACameraComponent>();
		camera->SetFov(60.0f);
		camera->SetNearClip(0.1f);
		camera->SetFarClip(2000.0f);
		actor->CreateComponent<ADefaultControllerComponent>();
		actor->GetTransform()->SetPosition({ 0.0f, 0.0f, -3.0f });
		actor->GetTransform()->SetRotation({ 0.0f,0.0f,0.0f });

		AActor* pbrActor = world->CreateChild<AActor>();
		pbrActor->GetTransform()->SetPosition({ 0.0f, 10.0f, 0.0f });
		pbrActor->GetTransform()->SetRotation(FQuaternion(0.0f, 0.0f, 0.0f));
		pbrActor->GetTransform()->SetScale({ 4.0f, 4.0f, 4.0f });
		AMeshComponent* meshComponent = pbrActor->CreateComponent<AMeshComponent>();
		meshComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/bunny.bin"));
		meshComponent->SetMaterial(GResourceModule::Get().LoadResource<OMaterial>("Material/Pbr.json"));
	}

	virtual int shutdown() override
	{
		return 0;
	}

	bool update() override
	{
		GProcessWindow& processWindow = GProcessWindow::Get();
		IBLSettings& currentSettings = FForwardShadingRenderer::_iblPipline._settings;

		showExampleDialog(this);

		ImGui::SetNextWindowPos(
				ImVec2(processWindow._width - processWindow._width / 5.0f - 10.0f, 10.0f)
			, ImGuiCond_FirstUseEver
			);
		ImGui::SetNextWindowSize(
				ImVec2(processWindow._width / 5.0f, processWindow._height - 20.0f)
			, ImGuiCond_FirstUseEver
			);
		ImGui::Begin("Settings"
			, NULL
			, 0
			);
		ImGui::PushItemWidth(180.0f);

		ImGui::Text("Environment light:");
		ImGui::Indent();
		ImGui::Checkbox("IBL Diffuse",  &currentSettings._doDiffuseIbl);
		ImGui::Checkbox("IBL Specular", &currentSettings._doSpecularIbl);

		if (ImGui::BeginTabBar("Cubemap", ImGuiTabBarFlags_None) )
		{
			if (ImGui::BeginTabItem("Bolonga") )
			{
				_skyboxComponent->SetTexture(_bolongaLod);
				_skyboxComponent->SetIrranceTexture(_bolongaIrr);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Kyoto") )
			{
				_skyboxComponent->SetTexture(_kyotoLod);
				_skyboxComponent->SetIrranceTexture(_kyotoIrr);

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::SliderFloat("Texture LOD", &currentSettings._lod, 0.0f, 10.1f);
		ImGui::Unindent();

		ImGui::Separator();
		ImGui::Text("Directional light:");
		ImGui::Indent();
		ImGui::Checkbox("Diffuse",  &currentSettings._doDiffuse);
		ImGui::Checkbox("Specular", &currentSettings._doSpecular);
		const bool doDirectLighting = currentSettings._doDiffuse || currentSettings._doSpecular;
		if (doDirectLighting)
		{
			ImGui::SliderFloat("Light direction X", &currentSettings._lightDirection[0], -1.0f, 1.0f);
			ImGui::SliderFloat("Light direction Y", &currentSettings._lightDirection[1], -1.0f, 1.0f);
			ImGui::SliderFloat("Light direction Z", &currentSettings._lightDirection[2], -1.0f, 1.0f);
			ImGui::ColorWheel("Color:", currentSettings._lightColor, 0.6f);
		}
		ImGui::Unindent();

		ImGui::Separator();
		//ImGui::Text("Background:");
		//ImGui::Indent();
		//{
		//	if (ImGui::BeginTabBar("CubemapSelection", ImGuiTabBarFlags_None) )
		//	{
		//		if (ImGui::BeginTabItem("Irradiance") )
		//		{
		//			ImGui::EndTabItem();
		//		}

		//		if (ImGui::BeginTabItem("Radiance") )
		//		{
		//			//ImGui::SliderFloat("Mip level", &currentSettings.m_radianceSlider, 1.0f, 6.0f);

		//			ImGui::EndTabItem();
		//		}

		//		if (ImGui::BeginTabItem("Skybox") )
		//		{		//			ImGui::EndTabItem();
		//		}

		//		ImGui::EndTabBar();
		//	}
		//}
		//ImGui::Unindent();

		ImGui::Separator();
		ImGui::Text("Post processing:");
		ImGui::Indent();
		ImGui::SliderFloat("Exposure",& currentSettings._exposure, -4.0f, 4.0f);
		ImGui::Unindent();

		ImGui::PopItemWidth();
		ImGui::End();

		ImGui::SetNextWindowPos(
				ImVec2(10.0f, 260.0f)
			, ImGuiCond_FirstUseEver
			);
		ImGui::SetNextWindowSize(
				ImVec2(processWindow._width / 5.0f, 450.0f)
			, ImGuiCond_FirstUseEver
			);
		ImGui::Begin("Mesh"
			, NULL
			, 0
			);

		ImGui::Text("Mesh:");
		ImGui::Indent();
		ImGui::RadioButton("Bunny", &currentSettings._meshSelection, 0);
		ImGui::RadioButton("Orbs",  &currentSettings._meshSelection, 1);
		ImGui::Unindent();

		const bool isBunny = (0 == currentSettings._meshSelection);
		if (!isBunny)
		{
			currentSettings._metalOrSpec = 0;
		}
		else
		{
			ImGui::Separator();
			ImGui::Text("Workflow:");
			ImGui::Indent();
			ImGui::RadioButton("Metalness", &currentSettings._metalOrSpec, 0);
			ImGui::RadioButton("Specular", &currentSettings._metalOrSpec, 1);
			ImGui::Unindent();

			ImGui::Separator();
			ImGui::Text("Material:");
			ImGui::Indent();
			ImGui::PushItemWidth(130.0f);
			ImGui::SliderFloat("Glossiness", &currentSettings._glossiness, 0.0f, 1.0f);
			ImGui::SliderFloat(0 == currentSettings._metalOrSpec ? "Metalness" : "Diffuse - Specular", &currentSettings._reflectivity, 0.0f, 1.0f);
			ImGui::PopItemWidth();
			ImGui::Unindent();
		}


		ImGui::ColorWheel("Diffuse:", &currentSettings._rgbDiff[0], 0.7f);
		ImGui::Separator();
		if ( (1 == currentSettings._metalOrSpec) && isBunny )
		{
			ImGui::ColorWheel("Specular:", &currentSettings._rgbSpec[0], 0.7f);
		}

		ImGui::End();

		return true;
	
	}

	ASkyboxComponent* _skyboxComponent;
	OTexture* _bolongaIrr;
	OTexture* _bolongaLod;
	OTexture* _kyotoIrr;
	OTexture* _kyotoLod;
};

} // namespace

int Auto3D_main(int _argc, char** _argv)
{
	ExampleIbl app(
		"18-ibl"
		, "Image-based lighting."
		, "https://bkaradzic.github.io/bgfx/examples.html#shadowmaps"
	);
	return GApplication::Get().RunAppInstance(&app, _argc, _argv);
}
