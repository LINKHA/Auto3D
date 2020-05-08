#include <string>
#include <vector>
#include <algorithm>

#include <bgfx/bgfx.h>
#include <bx/timer.h>
#include <bx/math.h>
#include <bx/file.h>
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
#include "Component/SpotLightComponent.h"
#include "Component/DirectionalLightComponent.h"
#include "Component/PointLightComponent.h"

#include "IO/FileSystem.h"
#include "IO/Stream.h"
#include "IO/File.h"
#include "Resource/ResourceCache.h"
#include "Resource/Material.h"
#include "Time/Time.h"
#include "Resource/Mesh.h"
#include "Platform/Dbg.h"
#include "Platform/ProcessWindow.h"
#include "Renderer/ShadowPipline.h"

using namespace Auto3D;

namespace
{
class ExampleShadowmaps : public Auto3D::IAppInstance
{
public:
	ExampleShadowmaps(const char* _name, const char* _description, const char* _url)
		: Auto3D::IAppInstance(_name, _description, _url)
	{
		//IAppInstance::_useCustomRender = true;
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
		skybox->SetIBLTexture(resourceModule.LoadResource<OTexture>("Textures/kyoto_lod.dds")
			, resourceModule.LoadResource<OTexture>("Textures/kyoto_irr.dds"));


		AActor* actor = world->CreateChild<AActor>();
		_camera = actor->CreateComponent<ACameraComponent>();		//const float camFovy = 60.0f;
		_camera->SetFov(60.0f);
		_camera->SetNearClip(0.1f);
		_camera->SetFarClip(2000.0f);

		ADefaultControllerComponent* controller = actor->CreateComponent<ADefaultControllerComponent>();
		controller->SetMoveSpeed(50.0f);
		actor->GetTransform()->SetPosition({ 0.0f, 100.0f, -60.0f });
		actor->GetTransform()->SetRotation({ 45.0f, 0.0f, 0.0f });

		AActor* hollowcubePlaneActor = world->CreateChild<AActor>();
		AMeshComponent* hollowcubePlaneComponent = hollowcubePlaneActor->CreateComponent<AMeshComponent>();
		//hollowcubePlaneComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/hollowcube.bin"));
		//hollowcubePlaneComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/bunny.bin"));
		hollowcubePlaneComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube2.bin"));
		hollowcubePlaneActor->GetTransform()->SetPosition({ 0.0f, 10.0f, 0.0f });
		hollowcubePlaneActor->GetTransform()->SetScale({ 5.5f, 5.5f, 5.5f });

		//AActor* meshActor = world->CreateChild<AActor>();
		//AMeshComponent* meshComponent = meshActor->CreateComponent<AMeshComponent>();
		//meshComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin"));
		//meshActor->GetTransform()->SetPosition({ -10.0f, 10.0f, 0.0f });
		//meshActor->GetTransform()->SetScale({ 2.5f, 2.5f, 2.5f });
		//AActor* meshActor2 = world->CreateChild<AActor>();
		//AMeshComponent* meshComponent2 = meshActor2->CreateComponent<AMeshComponent>();
		//meshComponent2->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin"));
		//meshActor2->GetTransform()->SetPosition({ 10.0f, 10.0f, 0.0f });
		//meshActor2->GetTransform()->SetScale({ 2.5f, 2.5f, 2.5f });

		AActor* planeActor = world->CreateChild<AActor>();
		AMeshComponent* planeComponent = planeActor->CreateComponent<AMeshComponent>();
		planeComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin"));
		planeActor->GetTransform()->SetScale({ 500.0f, 500.0f, 500.0f });
		planeActor->GetTransform()->SetPosition({ 0.0f, -500.0f, 0.0f });

		_directionalLightActor = world->CreateChild<AActor>();
		_directionalLightActor->SetEnabled(false);
		_directionalLightActor->GetTransform()->SetPosition({ 25.0f, 25.0f, 25.0f });
		_directionalLightActor->CreateComponent<ADirectionalLightComponent>();

		_spotLightActor = world->CreateChild<AActor>();
		_spotLightActor->SetEnabled(true);
		_spotLightActor->GetTransform()->SetPosition({ 25.0f, 25.0f, 25.0f });
		_spotLightActor->CreateComponent<ASpotLightComponent>();

		_pointLightActor = world->CreateChild<AActor>();
		_pointLightActor->SetEnabled(false);
		_pointLightActor->GetTransform()->SetPosition({ 25.0f, 25.0f, 25.0f });
		_pointLightActor->CreateComponent<APointLightComponent>();



	}

	virtual int shutdown() override
	{
		
		return 0;
	}

	bool update() override
	{

#pragma region gui
			//		FShadowPipline::s_uniforms.submitConstUniforms();
		GProcessWindow& processWindow = GProcessWindow::Get();

			showExampleDialog(this);
			ImGui::SetNextWindowPos(
				{ processWindow._width - processWindow._width / 5.0f - 10.0f, 10.0f }
				, ImGuiCond_FirstUseEver
				);
			ImGui::SetNextWindowSize(
				{ processWindow._width / 5.0f, processWindow._height - 20.0f }
				, ImGuiCond_FirstUseEver
				);
			ImGui::Begin("Settings"
				, NULL
				, 0
				);

#define IMGUI_FLOAT_SLIDER(_name, _val) \
	ImGui::SliderFloat(_name            \
			, &_val                     \
			, *( ((float*)&_val)+1)     \
			, *( ((float*)&_val)+2)     \
			)

#define IMGUI_RADIO_BUTTON(_name, _var, _val)     \
	if (ImGui::RadioButton(_name, _var == _val) ) \
	{                                             \
		_var = _val;                              \
	}

			ImGui::Separator();
			ImGui::Text("Shadow map depth:");
			IMGUI_RADIO_BUTTON("InvZ", FShadowPipline::s_shadowSceneSettings.m_depthImpl, EDepthImpl::InvZ);
			IMGUI_RADIO_BUTTON("Linear", FShadowPipline::s_shadowSceneSettings.m_depthImpl, EDepthImpl::Linear);

			FShadowMapSettings* currentSmSettings = &FShadowPipline::s_shadowMapSettings[FShadowPipline::s_shadowSceneSettings.m_lightType][FShadowPipline::s_shadowSceneSettings.m_depthImpl][FShadowPipline::s_shadowSceneSettings.m_smImpl];

			ImGui::Separator();
			ImGui::Checkbox("Draw depth buffer", &FShadowPipline::s_shadowSceneSettings.m_drawDepthBuffer);
			if (FShadowPipline::s_shadowSceneSettings.m_drawDepthBuffer)
			{
				IMGUI_FLOAT_SLIDER("Depth value pow", currentSmSettings->m_depthValuePow);
			}

			ImGui::Separator();
			ImGui::Text("Shadow Map implementation");
			IMGUI_RADIO_BUTTON("Hard", FShadowPipline::s_shadowSceneSettings.m_smImpl, EShadowMapImpl::Hard);
			IMGUI_RADIO_BUTTON("PCF", FShadowPipline::s_shadowSceneSettings.m_smImpl, EShadowMapImpl::PCF);
			IMGUI_RADIO_BUTTON("VSM", FShadowPipline::s_shadowSceneSettings.m_smImpl, EShadowMapImpl::VSM);
			IMGUI_RADIO_BUTTON("ESM", FShadowPipline::s_shadowSceneSettings.m_smImpl, EShadowMapImpl::ESM);
			currentSmSettings = &FShadowPipline::s_shadowMapSettings[FShadowPipline::s_shadowSceneSettings.m_lightType][FShadowPipline::s_shadowSceneSettings.m_depthImpl][FShadowPipline::s_shadowSceneSettings.m_smImpl];

			ImGui::Separator();
			IMGUI_FLOAT_SLIDER("Bias", currentSmSettings->m_bias);
			IMGUI_FLOAT_SLIDER("Normal offset", currentSmSettings->m_normalOffset);
			ImGui::Separator();
			if (ELightType::DirectionalLight != FShadowPipline::s_shadowSceneSettings.m_lightType)
			{
				IMGUI_FLOAT_SLIDER("Near plane", currentSmSettings->m_near);
			}
			IMGUI_FLOAT_SLIDER("Far plane", currentSmSettings->m_far);

			ImGui::Separator();
			switch(FShadowPipline::s_shadowSceneSettings.m_smImpl)
			{
				case EShadowMapImpl::Hard:
					//ImGui::Text("Hard");
					break;

				case EShadowMapImpl::PCF:
					ImGui::Text("PCF");
					IMGUI_FLOAT_SLIDER("X Offset", currentSmSettings->m_xOffset);
					IMGUI_FLOAT_SLIDER("Y Offset", currentSmSettings->m_yOffset);
					break;

				case EShadowMapImpl::VSM:
					ImGui::Text("VSM");
					IMGUI_FLOAT_SLIDER("Min variance", currentSmSettings->m_customParam0);
					IMGUI_FLOAT_SLIDER("Depth multiplier", currentSmSettings->m_customParam1);
					ImGui::Checkbox("Blur shadow map", &currentSmSettings->m_doBlur);
					if (currentSmSettings->m_doBlur)
					{
						IMGUI_FLOAT_SLIDER("Blur X Offset", currentSmSettings->m_xOffset);
						IMGUI_FLOAT_SLIDER("Blur Y Offset", currentSmSettings->m_yOffset);
					}
					break;

				case EShadowMapImpl::ESM:
					ImGui::Text("ESM");
					IMGUI_FLOAT_SLIDER("ESM Hardness", currentSmSettings->m_customParam0);
					IMGUI_FLOAT_SLIDER("Depth multiplier", currentSmSettings->m_customParam1);
					ImGui::Checkbox("Blur shadow map", &currentSmSettings->m_doBlur);
					if (currentSmSettings->m_doBlur)
					{
						IMGUI_FLOAT_SLIDER("Blur X Offset", currentSmSettings->m_xOffset);
						IMGUI_FLOAT_SLIDER("Blur Y Offset", currentSmSettings->m_yOffset);
					}
					break;

				default:
					break;
			};

			ImGui::End();
#undef IMGUI_RADIO_BUTTON

			ImGui::SetNextWindowPos(
				  ImVec2(10.0f, 260.0f)
				, ImGuiCond_FirstUseEver
				);
			ImGui::SetNextWindowSize(
				  ImVec2(processWindow._width / 5.0f, 350.0f)
				, ImGuiCond_FirstUseEver
				);
			ImGui::Begin("Light"
				, NULL
				, 0
				);
			ImGui::PushItemWidth(185.0f);

			bool bLtChanged = false;
			if ( ImGui::RadioButton("Spot light", FShadowPipline::s_shadowSceneSettings.m_lightType == ELightType::SpotLight ))
			{
				_directionalLightActor->SetEnabled(false);
				_spotLightActor->SetEnabled(true);
				_pointLightActor->SetEnabled(false);
			}
			if ( ImGui::RadioButton("Point light", FShadowPipline::s_shadowSceneSettings.m_lightType == ELightType::PointLight ))
			{
				_directionalLightActor->SetEnabled(false);
				_spotLightActor->SetEnabled(false);
				_pointLightActor->SetEnabled(true);
			}
			if ( ImGui::RadioButton("Directional light", FShadowPipline::s_shadowSceneSettings.m_lightType == ELightType::DirectionalLight ))
			{
				_directionalLightActor->SetEnabled(true);
				_spotLightActor->SetEnabled(false);
				_pointLightActor->SetEnabled(false);
			}

			ImGui::Separator();
			ImGui::Checkbox("Show shadow map coverage.", &FShadowPipline::s_shadowSceneSettings.m_showSmCoverage);

			ImGui::Separator();
			ImGui::Text("Shadow map resolution: %ux%u", FShadowPipline::s_currentShadowMapSize, FShadowPipline::s_currentShadowMapSize);
			ImGui::SliderFloat("##SizePwrTwo", &currentSmSettings->m_sizePwrTwo,
							   currentSmSettings->m_sizePwrTwoMin,
							   currentSmSettings->m_sizePwrTwoMax, "%.0f");

			ImGui::Separator();
			if (ELightType::SpotLight == FShadowPipline::s_shadowSceneSettings.m_lightType)
			{
				ImGui::Text("Spot light");
				ImGui::SliderFloat("Shadow map area", &FShadowPipline::s_shadowSceneSettings.m_coverageSpotL, 45.0f, 120.0f);

				ImGui::Separator();
				ImGui::SliderFloat("Spot outer cone", &FShadowPipline::s_shadowSceneSettings.m_spotOuterAngle, 0.0f, 91.0f);
				ImGui::SliderFloat("Spot inner cone", &FShadowPipline::s_shadowSceneSettings.m_spotInnerAngle, 0.0f, 90.0f);
			}
			else if (ELightType::PointLight == FShadowPipline::s_shadowSceneSettings.m_lightType)
			{
				ImGui::Text("Point light");
				ImGui::Checkbox("Stencil pack", &FShadowPipline::s_shadowSceneSettings.m_stencilPack);

				ImGui::SliderFloat("Fov X adjust", &FShadowPipline::s_shadowSceneSettings.m_fovXAdjust, -20.0f, 20.0f);
				ImGui::SliderFloat("Fov Y adjust", &FShadowPipline::s_shadowSceneSettings.m_fovYAdjust, -20.0f, 20.0f);
			}
			else if (ELightType::DirectionalLight == FShadowPipline::s_shadowSceneSettings.m_lightType)
			{
				ImGui::Text("Directional light");
				ImGui::Checkbox("Stabilize cascades", &FShadowPipline::s_shadowSceneSettings.m_stabilize);
				ImGui::SliderInt("Cascade splits", &FShadowPipline::s_shadowSceneSettings.m_numSplits, 1, 4);
				ImGui::SliderFloat("Cascade distribution", &FShadowPipline::s_shadowSceneSettings.m_splitDistribution, 0.0f, 1.0f);
			}

#undef IMGUI_FLOAT_SLIDER

			ImGui::End();

#pragma endregion gui

		return true;
	}

	ACameraComponent* _camera;

	AActor* _spotLightActor;
	AActor* _directionalLightActor;
	AActor* _pointLightActor;
};

} // namespace
int Auto3D_main(int _argc, char** _argv)
{
	ExampleShadowmaps app(
		 "16-shadowmaps"
		, "Shadow maps example."
		, "https://bkaradzic.github.io/bgfx/examples.html#shadowmaps"
		);
	return GApplication::Get().RunAppInstance(&app, _argc, _argv);
}
