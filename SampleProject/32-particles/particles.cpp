#include "RHI/bgfx_utils.h"
#include "Platform/Cmd.h"
#include "Platform/input.h"
#include "Application.h"
#include "Platform/ProcessWindow.h"
#include "Gameplay/World.h"
#include "Gameplay/WorldContext.h"
#include "Resource/ResourceCache.h"
#include "Component/CameraComponent.h"
#include "Component/DefaultControllerComponent.h"
#include "Component/TransformComponent.h"
#include "Gameplay/Actor.h"
#include "Debug/Debugdraw/DebugDraw.h"

#include "Component/ParticleComponent.h"

#include "UI/UI.h"

#include <bx/rng.h>
#include <bx/easing.h>
#include <bx/timer.h>


using namespace Auto3D;
namespace
{


class ExampleParticles : public Auto3D::IAppInstance
{
public:
	ExampleParticles(const char* _name, const char* _description, const char* _url)
		: Auto3D::IAppInstance(_name, _description, _url)
	{
		//IAppInstance::_useCustomRender = true;
	}
	ACameraComponent* _camera;
	void init() override
	{

		GResourceModule& resourceModule = GResourceModule::Get();

		AWorld* world = FWorldContext::Get().NewWorld();
		world->SetName("world");
		world->DefineLayer(0, "Default");
		world->DefineLayer(1, "UI");
		world->DefineTag(0, "Default");
		world->DefineTag(1, "Player");

		AActor* actor = world->CreateChild<AActor>();
		_camera = actor->CreateComponent<ACameraComponent>();
		_camera->SetFov(60.0f);
		_camera->SetNearClip(0.1f);
		_camera->SetFarClip(2000.0f);
		ADefaultControllerComponent* controller = actor->CreateComponent<ADefaultControllerComponent>();
		controller->SetMoveSpeed(50.0f);
		actor->GetTransform()->SetPosition({ 0.0f, 2.0f, -12.0f });
		
		ddInit();

		psInit();

		bimg::ImageContainer* image = imageLoad(
			  "textures/particle.ktx"
			, bgfx::TextureFormat::BGRA8
			);

		FEmitterSpriteHandle sprite = psCreateSprite(
				  uint16_t(image->m_width)
				, uint16_t(image->m_height)
				, image->m_data
				);

		bimg::imageFree(image);

		for (uint32_t ii = 0; ii < BX_COUNTOF(m_emitter); ++ii)
		{
			m_emitter[ii].Create();
			m_emitter[ii]._uniforms.m_handle = sprite;
			m_emitter[ii].Update();
		}

		m_timeOffset = bx::getHPCounter();
	}

	virtual int shutdown() override
	{
		for (uint32_t ii = 0; ii < BX_COUNTOF(m_emitter); ++ii)
		{
			m_emitter[ii].Destroy();
		}

		psShutdown();

		ddShutdown();

		return 0;
	}

	bool update() override
	{
		//if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState) )
		GProcessWindow& processWindow = GProcessWindow::Get();
		m_width = processWindow._width;
		m_height = processWindow._height;
		m_debug = processWindow._debug;
		m_reset = processWindow._reset;
		m_mouseState = processWindow._mouseState;
		
		{
			// Set view 0 default viewport.
			bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height) );

			bgfx::touch(0);

			int64_t now = bx::getHPCounter() - m_timeOffset;
			static int64_t last = now;
			const int64_t frameTime = now - last;
			last = now;
			const double freq = double(bx::getHPFrequency() );
			const float deltaTime = float(frameTime/freq);


			float* view/*[16]*/;
			float* proj/*[16]*/;
			//bx::mtxIdentity(view);

			TMatrix3x4F viewMatrix = _camera->GetViewMatrix();
			TMatrix4x4F transposeViewMatrix = viewMatrix.ToMatrix4().Transpose();
			TMatrix4x4F projectionMatrix = _camera->GetProjectionMatrix();
			view = transposeViewMatrix.Data();
			proj = projectionMatrix.Data();

			// Set view and projection matrix for view 0.
			{
				bx::mtxProj(proj, 60.0f, float(m_width)/float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

				bgfx::setViewTransform(0, view, proj);
				bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height) );
			}

			/*imguiBeginFrame(
				   m_mouseState.m_mx
				,  m_mouseState.m_my
				, (m_mouseState.m_buttons[entry::MouseButton::Left  ] ? IMGUI_MBUT_LEFT   : 0)
				| (m_mouseState.m_buttons[entry::MouseButton::Right ] ? IMGUI_MBUT_RIGHT  : 0)
				| (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0)
				,  m_mouseState.m_mz
				, uint16_t(m_width)
				, uint16_t(m_height)
				);*/

			showExampleDialog(this);

			ImGui::SetNextWindowPos(
				  ImVec2(m_width - m_width / 4.0f - 10.0f, 10.0f)
				, ImGuiCond_FirstUseEver
				);
			ImGui::SetNextWindowSize(
				  ImVec2(m_width / 4.0f, m_height - 20.0f)
				, ImGuiCond_FirstUseEver
				);
			ImGui::Begin("Settings"
				, NULL
				);

			static float timeScale = 1.0f;
			ImGui::SliderFloat("Time scale"
				, &timeScale
				, 0.0f
				, 1.0f
				);

			static bool showBounds;
			ImGui::Checkbox("Show bounds", &showBounds);

			ImGui::Text("Emitter:");
			static int currentEmitter = 0;
			for (uint32_t ii = 0; ii < BX_COUNTOF(m_emitter); ++ii)
			{
				ImGui::SameLine();

				char name[16];
				bx::snprintf(name, BX_COUNTOF(name), "%d", ii);

				ImGui::RadioButton(name, &currentEmitter, ii);
			}

			m_emitter[currentEmitter].imgui();

			ImGui::End();

			m_emitter[currentEmitter].gizmo(view, proj);

			//imguiEndFrame();

			DebugDrawEncoder dde;
			int viewId = 0;

			dde.begin(viewId);

			dde.drawGrid(Axis::Y, { 0.0f, 0.0f, 0.0f }, 10000);

			TVector3F pos = _camera->GetOwner()->GetTransform()->GetPosition();
			const bx::Vec3 eye = { pos._x, pos._y, pos._z };
			
			m_emitter[currentEmitter].Update();

			psUpdate(deltaTime * timeScale);
			psRender(viewId, view, eye);

			if (showBounds)
			{
				Aabb aabb;
				psGetAabb(m_emitter[currentEmitter]._handle, aabb);
				dde.push();
					dde.setWireframe(true);
					dde.setColor(0xff0000ff);
					dde.draw(aabb);
				dde.pop();
			}

			dde.end();


			return true;
		}

		return false;
	}

	MouseState m_mouseState;

	int64_t m_timeOffset;

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;

	FEmitter m_emitter[4];
};

} // namespace

AUTO_IMPLEMENT_MAIN(
	ExampleParticles
	, "32-particles"
	, "Particles."
	, "https://bkaradzic.github.io/bgfx/examples.html#particles"
);
