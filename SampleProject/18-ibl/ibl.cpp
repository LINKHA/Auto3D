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
		AWorld* world = FWorldContext::Get().NewWorld();
		world->SetName("world");
		world->DefineLayer(0, "Default");
		world->DefineLayer(1, "UI");
		world->DefineTag(0, "Default");
		world->DefineTag(1, "Player");
		world->CreateComponent<ASkyboxComponent>();

		AActor* actor = world->CreateChild<AActor>();
		ACameraComponent* camera = actor->CreateComponent<ACameraComponent>();
		camera->SetFov(60.0f);
		camera->SetNearClip(0.1f);
		camera->SetFarClip(2000.0f);
		actor->CreateComponent<ADefaultControllerComponent>();
		actor->GetTransform()->SetPosition({ 0.0f, 0.0f, -3.0f });
		actor->GetTransform()->SetRotation({ 0.0f,0.0f,0.0f });

		//AActor* pbrActor = world->CreateChild<AActor>();
		//pbrActor->GetTransform()->SetPosition({ 0.0f, 10.0f, 0.0f });
		//pbrActor->GetTransform()->SetRotation(FQuaternion(0.0f, 0.0f, 0.0f));
		//pbrActor->GetTransform()->SetScale({ 4.0f, 4.0f, 4.0f });
		//AMeshComponent* meshComponent = pbrActor->CreateComponent<AMeshComponent>();
		//meshComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/bunny.bin"));
		//meshComponent->SetMaterial(GResourceModule::Get().LoadResource<OMaterial>("Material/Pbr.json"));
	}

	virtual int shutdown() override
	{
		return 0;
	}

	bool update() override
	{
		GProcessWindow& processWindow = GProcessWindow::Get();
		IBLSettings& currentSettings = FIBLRenderer::Get().m_settings;

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
		ImGui::Checkbox("IBL Diffuse",  &currentSettings.m_doDiffuseIbl);
		ImGui::Checkbox("IBL Specular", &currentSettings.m_doSpecularIbl);

		if (ImGui::BeginTabBar("Cubemap", ImGuiTabBarFlags_None) )
		{
			if (ImGui::BeginTabItem("Bolonga") )
			{
				FIBLRenderer::Get().m_currentLightProbe = LightProbe::Bolonga;
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Kyoto") )
			{
				FIBLRenderer::Get().m_currentLightProbe = LightProbe::Kyoto;
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::SliderFloat("Texture LOD", &currentSettings.m_lod, 0.0f, 10.1f);
		ImGui::Unindent();

		ImGui::Separator();
		ImGui::Text("Directional light:");
		ImGui::Indent();
		ImGui::Checkbox("Diffuse",  &currentSettings.m_doDiffuse);
		ImGui::Checkbox("Specular", &currentSettings.m_doSpecular);
		const bool doDirectLighting = currentSettings.m_doDiffuse || currentSettings.m_doSpecular;
		if (doDirectLighting)
		{
			ImGui::SliderFloat("Light direction X", &currentSettings.m_lightDir[0], -1.0f, 1.0f);
			ImGui::SliderFloat("Light direction Y", &currentSettings.m_lightDir[1], -1.0f, 1.0f);
			ImGui::SliderFloat("Light direction Z", &currentSettings.m_lightDir[2], -1.0f, 1.0f);
			ImGui::ColorWheel("Color:", currentSettings.m_lightCol, 0.6f);
		}
		ImGui::Unindent();

		ImGui::Separator();
		ImGui::Text("Background:");
		ImGui::Indent();
		{
			if (ImGui::BeginTabBar("CubemapSelection", ImGuiTabBarFlags_None) )
			{
				if (ImGui::BeginTabItem("Irradiance") )
				{
					currentSettings.m_bgType = currentSettings.m_radianceSlider;
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Radiance") )
				{
					currentSettings.m_bgType = 7.0f;

					ImGui::SliderFloat("Mip level", &currentSettings.m_radianceSlider, 1.0f, 6.0f);

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Skybox") )
				{
					currentSettings.m_bgType = 0.0f;
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		ImGui::Unindent();

		ImGui::Separator();
		ImGui::Text("Post processing:");
		ImGui::Indent();
		ImGui::SliderFloat("Exposure",& currentSettings.m_exposure, -4.0f, 4.0f);
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
		ImGui::RadioButton("Bunny", &currentSettings.m_meshSelection, 0);
		ImGui::RadioButton("Orbs",  &currentSettings.m_meshSelection, 1);
		ImGui::Unindent();

		const bool isBunny = (0 == currentSettings.m_meshSelection);
		if (!isBunny)
		{
			currentSettings.m_metalOrSpec = 0;
		}
		else
		{
			ImGui::Separator();
			ImGui::Text("Workflow:");
			ImGui::Indent();
			ImGui::RadioButton("Metalness", &currentSettings.m_metalOrSpec, 0);
			ImGui::RadioButton("Specular", &currentSettings.m_metalOrSpec, 1);
			ImGui::Unindent();

			ImGui::Separator();
			ImGui::Text("Material:");
			ImGui::Indent();
			ImGui::PushItemWidth(130.0f);
			ImGui::SliderFloat("Glossiness", &currentSettings.m_glossiness, 0.0f, 1.0f);
			ImGui::SliderFloat(0 == currentSettings.m_metalOrSpec ? "Metalness" : "Diffuse - Specular", &currentSettings.m_reflectivity, 0.0f, 1.0f);
			ImGui::PopItemWidth();
			ImGui::Unindent();
		}


		ImGui::ColorWheel("Diffuse:", &currentSettings.m_rgbDiff[0], 0.7f);
		ImGui::Separator();
		if ( (1 == currentSettings.m_metalOrSpec) && isBunny )
		{
			ImGui::ColorWheel("Specular:", &currentSettings.m_rgbSpec[0], 0.7f);
		}

		ImGui::End();

		return true;
	
	}
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
