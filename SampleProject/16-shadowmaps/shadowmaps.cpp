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

#include "IO/FileSystem.h"
#include "IO/Stream.h"
#include "IO/File.h"
#include "Resource/ResourceCache.h"
#include "Resource/Material.h"
#include "Time/Time.h"
#include "Resource/Mesh.h"
#include "Platform/Dbg.h"
#include "Platform/ProcessWindow.h"
#include "Renderer/ShadowRenderer.h"

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

		AWorld* world = FWorldContext::Get().NewWorld();
		world->SetName("world");
		world->DefineLayer(0, "Default");
		world->DefineLayer(1, "UI");
		world->DefineTag(0, "Default");
		world->DefineTag(1, "Player");

		AActor* actor = world->CreateChild<AActor>();
		_camera = actor->CreateComponent<ACameraComponent>();		//const float camFovy = 60.0f;
		_camera->SetFov(60.0f);
		_camera->SetNearClip(0.1f);
		_camera->SetFarClip(2000.0f);

		ADefaultControllerComponent* controller = actor->CreateComponent<ADefaultControllerComponent>();
		controller->SetMoveSpeed(10.0f);
		actor->GetTransform()->SetPosition({ 0.0f, 60.0f, -105.0f });
		actor->GetTransform()->SetRotation({ -45.0f, 0.0f, 0.0f });

		_meshActor = world->CreateChild<AActor>();
		_meshComponent = _meshActor->CreateComponent<AMeshComponent>();
		_meshComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin"));
		_meshActor->GetTransform()->SetPosition({ -15.0f, 5.0f, 0.0f });
		_meshActor->GetTransform()->SetScale({ 2.5f, 2.5f, 2.5f });

		AActor* _planeActor2 = world->CreateChild<AActor>();
		AMeshComponent* _planeComponent2 = _planeActor2->CreateComponent<AMeshComponent>();
		_planeComponent2->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/hollowcube.bin"));
		_planeActor2->GetTransform()->SetPosition({ -15.0f, 5.0f, 10.0f });
		_planeActor2->GetTransform()->SetScale({ 2.5f, 2.5f, 2.5f });

		_planeActor = world->CreateChild<AActor>();
		_planeComponent = _planeActor->CreateComponent<AMeshComponent>();
		_planeComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube2.bin"));
		_planeActor->GetTransform()->SetScale({ 500.0f, 500.0f, 500.0f });
		_planeActor->GetTransform()->SetPosition({ 0.0f, -500.0f, 0.0f });

		

		AActor* lightActor = world->CreateChild<AActor>();
		lightActor->GetTransform()->SetPosition({ 25.0f, 25.0f, 25.0f });
		_lightComponent = lightActor->CreateComponent<ALightComponent>();
		_pointLightComponent = lightActor->CreateComponent<ALightComponent>();
		
		FShadowRenderer::s_pointLight = _pointLightComponent;
		FShadowRenderer::s_directionalLight = _lightComponent;

		FShadowRenderer::Get().Init();

	}

	virtual int shutdown() override
	{
		
		return 0;
	}

	bool update() override
	{

#pragma region gui
			//		FShadowRenderer::s_uniforms.submitConstUniforms();
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
			IMGUI_RADIO_BUTTON("InvZ", FShadowRenderer::s_settings.m_depthImpl, DepthImpl::InvZ);
			IMGUI_RADIO_BUTTON("Linear", FShadowRenderer::s_settings.m_depthImpl, DepthImpl::Linear);

			ShadowMapSettings* currentSmSettings = &FShadowRenderer::s_smSettings[FShadowRenderer::s_settings.m_lightType][FShadowRenderer::s_settings.m_depthImpl][FShadowRenderer::s_settings.m_smImpl];

			ImGui::Separator();
			ImGui::Checkbox("Draw depth buffer", &FShadowRenderer::s_settings.m_drawDepthBuffer);
			if (FShadowRenderer::s_settings.m_drawDepthBuffer)
			{
				IMGUI_FLOAT_SLIDER("Depth value pow", currentSmSettings->m_depthValuePow);
			}

			ImGui::Separator();
			ImGui::Text("Shadow Map implementation");
			IMGUI_RADIO_BUTTON("Hard", FShadowRenderer::s_settings.m_smImpl, SmImpl::Hard);
			IMGUI_RADIO_BUTTON("PCF", FShadowRenderer::s_settings.m_smImpl, SmImpl::PCF);
			IMGUI_RADIO_BUTTON("VSM", FShadowRenderer::s_settings.m_smImpl, SmImpl::VSM);
			IMGUI_RADIO_BUTTON("ESM", FShadowRenderer::s_settings.m_smImpl, SmImpl::ESM);
			currentSmSettings = &FShadowRenderer::s_smSettings[FShadowRenderer::s_settings.m_lightType][FShadowRenderer::s_settings.m_depthImpl][FShadowRenderer::s_settings.m_smImpl];

			ImGui::Separator();
			IMGUI_FLOAT_SLIDER("Bias", currentSmSettings->m_bias);
			IMGUI_FLOAT_SLIDER("Normal offset", currentSmSettings->m_normalOffset);
			ImGui::Separator();
			if (LightType::DirectionalLight != FShadowRenderer::s_settings.m_lightType)
			{
				IMGUI_FLOAT_SLIDER("Near plane", currentSmSettings->m_near);
			}
			IMGUI_FLOAT_SLIDER("Far plane", currentSmSettings->m_far);

			ImGui::Separator();
			switch(FShadowRenderer::s_settings.m_smImpl)
			{
				case SmImpl::Hard:
					//ImGui::Text("Hard");
					break;

				case SmImpl::PCF:
					ImGui::Text("PCF");
					IMGUI_FLOAT_SLIDER("X Offset", currentSmSettings->m_xOffset);
					IMGUI_FLOAT_SLIDER("Y Offset", currentSmSettings->m_yOffset);
					break;

				case SmImpl::VSM:
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

				case SmImpl::ESM:
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
			if ( ImGui::RadioButton("Spot light", FShadowRenderer::s_settings.m_lightType == LightType::SpotLight ))
			{
				FShadowRenderer::s_settings.m_lightType = LightType::SpotLight; bLtChanged = true;
			}
			if ( ImGui::RadioButton("Point light", FShadowRenderer::s_settings.m_lightType == LightType::PointLight ))
			{
				FShadowRenderer::s_settings.m_lightType = LightType::PointLight; bLtChanged = true;
			}
			if ( ImGui::RadioButton("Directional light", FShadowRenderer::s_settings.m_lightType == LightType::DirectionalLight ))
			{
				FShadowRenderer::s_settings.m_lightType = LightType::DirectionalLight; bLtChanged = true;
			}

			ImGui::Separator();
			ImGui::Checkbox("Show shadow map coverage.", &FShadowRenderer::s_settings.m_showSmCoverage);

			ImGui::Separator();
			ImGui::Text("Shadow map resolution: %ux%u", FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			ImGui::SliderFloat("##SizePwrTwo", &currentSmSettings->m_sizePwrTwo,
							   currentSmSettings->m_sizePwrTwoMin,
							   currentSmSettings->m_sizePwrTwoMax, "%.0f");

			ImGui::Separator();
			if (LightType::SpotLight == FShadowRenderer::s_settings.m_lightType)
			{
				ImGui::Text("Spot light");
				ImGui::SliderFloat("Shadow map area", &FShadowRenderer::s_settings.m_coverageSpotL, 45.0f, 120.0f);

				ImGui::Separator();
				ImGui::SliderFloat("Spot outer cone", &FShadowRenderer::s_settings.m_spotOuterAngle, 0.0f, 91.0f);
				ImGui::SliderFloat("Spot inner cone", &FShadowRenderer::s_settings.m_spotInnerAngle, 0.0f, 90.0f);
			}
			else if (LightType::PointLight == FShadowRenderer::s_settings.m_lightType)
			{
				ImGui::Text("Point light");
				ImGui::Checkbox("Stencil pack", &FShadowRenderer::s_settings.m_stencilPack);

				ImGui::SliderFloat("Fov X adjust", &FShadowRenderer::s_settings.m_fovXAdjust, -20.0f, 20.0f);
				ImGui::SliderFloat("Fov Y adjust", &FShadowRenderer::s_settings.m_fovYAdjust, -20.0f, 20.0f);
			}
			else if (LightType::DirectionalLight == FShadowRenderer::s_settings.m_lightType)
			{
				ImGui::Text("Directional light");
				ImGui::Checkbox("Stabilize cascades", &FShadowRenderer::s_settings.m_stabilize);
				ImGui::SliderInt("Cascade splits", &FShadowRenderer::s_settings.m_numSplits, 1, 4);
				ImGui::SliderFloat("Cascade distribution", &FShadowRenderer::s_settings.m_splitDistribution, 0.0f, 1.0f);
			}

#undef IMGUI_FLOAT_SLIDER

			ImGui::End();

#pragma endregion gui

		return true;
	}

	ACameraComponent* _camera;

	AActor* _meshActor;
	AMeshComponent* _meshComponent;

	AActor* _planeActor;
	AMeshComponent* _planeComponent;

	ALightComponent* _lightComponent;
	ALightComponent* _pointLightComponent;
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
