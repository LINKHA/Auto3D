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
#include "Renderer/ViewPass.h"

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
		
		AActor* emitter1 = world->CreateChild<AActor>();
		_particleComponents.Push(emitter1->CreateComponent<AParticleComponent>());

		AActor* emitter2 = world->CreateChild<AActor>();
		_particleComponents.Push(emitter2->CreateComponent<AParticleComponent>());

		AActor* emitter3 = world->CreateChild<AActor>();
		_particleComponents.Push(emitter3->CreateComponent<AParticleComponent>());

		AActor* emitter4 = world->CreateChild<AActor>();
		_particleComponents.Push(emitter4->CreateComponent<AParticleComponent>());

		ddInit();
	}

	virtual int shutdown() override
	{
		ddShutdown();

		return 0;
	}

	bool update() override
	{
		GProcessWindow& processWindow = GProcessWindow::Get();
	
		TMatrix3x4F viewMatrix = _camera->GetViewMatrix();
		TMatrix4x4F transposeViewMatrix = viewMatrix.ToMatrix4().Transpose();
		TMatrix4x4F projectionMatrix = _camera->GetProjectionMatrix();

		showExampleDialog(this);

		ImGui::SetNextWindowPos(
				ImVec2(processWindow._width - processWindow._width / 4.0f - 10.0f, 10.0f)
			, ImGuiCond_FirstUseEver
			);
		ImGui::SetNextWindowSize(
				ImVec2(processWindow._width / 4.0f, processWindow._height - 20.0f)
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
		for (uint32_t ii = 0; ii < _particleComponents.Size(); ++ii)
		{
			ImGui::SameLine();

			char name[16];
			bx::snprintf(name, BX_COUNTOF(name), "%d", ii);

			ImGui::RadioButton(name, &currentEmitter, ii);
		}
		_particleComponents[currentEmitter]->_emitter.imgui();
			
		ImGui::End();

		_particleComponents[currentEmitter]->_emitter.gizmo(transposeViewMatrix.Data(), projectionMatrix.Data());
			
		DebugDrawEncoder dde;

		//int viewId = ViewIdOrdinary();
		int viewId = 1;
		{
			bgfx::setViewClear(viewId
				, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
				, 0x202020ff
				, 1.0f
				, 0
			);
			bgfx::setViewTransform(viewId, transposeViewMatrix.Data(), projectionMatrix.Data());
			bgfx::setViewRect(viewId, 0, 0, uint16_t(processWindow._width), uint16_t(processWindow._height));
		}

		dde.begin(viewId);

		dde.drawGrid(Axis::Y, { 0.0f, 0.0f, 0.0f }, 300);

		_particleComponents[currentEmitter]->_emitter.Update();

		if (showBounds)
		{
			Aabb aabb;
			psGetAabb(_particleComponents[currentEmitter]->_emitter._handle, aabb);
				
			dde.push();
				dde.setWireframe(true);
				dde.setColor(0xff0000ff);
				dde.draw(aabb);
			dde.pop();
		}

		dde.end();

		return true;
	}

	TVector<AParticleComponent*> _particleComponents;
};

} // namespace

AUTO_IMPLEMENT_MAIN(
	ExampleParticles
	, "32-particles"
	, "Particles."
	, "https://bkaradzic.github.io/bgfx/examples.html#particles"
);
