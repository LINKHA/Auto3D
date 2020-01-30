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

#include "Serialization/ToJson.h"
#include "Serialization/FromJson.h"

using namespace Auto3D;

namespace
{

class ExampleMesh : public Auto3D::IAppInstance
{
public:
	ExampleMesh(const char* _name, const char* _description, const char* _url)
		: Auto3D::IAppInstance(_name, _description, _url)
	{
	}

	void init(uint32_t _width, uint32_t _height) override
	{
		FArgs args = FArgs::Get();

		m_width  = _width;
		m_height = _height;
		m_debug  = BGFX_DEBUG_NONE;
		m_reset  = BGFX_RESET_VSYNC;

		u_time = bgfx::createUniform("u_time", bgfx::UniformType::Vec4);
		GBox::_time = u_time;

		// Create program from shaders.
		m_program = loadProgram("vs_mesh", "fs_mesh");
		GBox::_program = m_program;

		m_mesh = meshLoad("D:/Project/MyProject/Auto3D/Bin/meshes/bunny.bin");
		//m_mesh = meshLoad("meshes/bunny.bin");
		//m_mesh = meshLoad("Data/Model/TeaPot.mdl");
		GBox::_mesh = m_mesh;

		m_timeOffset = bx::getHPCounter();
		GBox::_timeOffset = m_timeOffset;

		AWorld* world = FWorldContext::Get().NewWorld();
		world->SetName("world");

		AActor* actor = world->CreateChild<AActor>();
		ACameraComponent* camera = actor->CreateComponent<ACameraComponent>();
		camera->SetPosition({ 0.0f, 1.0f, -2.5f });
		camera->SetVerticalAngle(-0.3f);

		AActor* meshActor = world->CreateChild<AActor>();
		AMeshComponent* meshComponent = meshActor->CreateComponent<AMeshComponent>();

		actor->SetName("asd");

		ToJson("D:/Project/MyProject/Auto3D/Bin/output_utf32le.json",world); // serialize the circle to 'json_string'

		FromJson("D:/Project/MyProject/Auto3D/Bin/output_utf32le.json");
	}

	int shutdown() override
	{
		//imguiDestroy();

		meshUnload(m_mesh);

		// Cleanup.
		bgfx::destroy(m_program);

		bgfx::destroy(u_time);

		// Shutdown bgfx.
		//bgfx::shutdown();

		return 0;
	}

	bool update() override
	{
		showExampleDialog(this);

		return true;
	}

	Auto3D::MouseState m_mouseState;

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;

	int64_t m_timeOffset;
	Mesh* m_mesh;
	bgfx::ProgramHandle m_program;
	bgfx::UniformHandle u_time;
};

} // namespace

int Auto3D_main(int _argc, char** _argv)
{
	ExampleMesh app(
		 "04-mesh"
		, "Loading meshes."
		, "https://bkaradzic.github.io/bgfx/examples.html#mesh"
	);
	return FApplication::Get().RunAppInstance(&app, _argc, _argv);
}

//ENTRY_IMPLEMENT_MAIN(
//	  ExampleMesh
//	, "04-mesh"
//	, "Loading meshes."
//	, "https://bkaradzic.github.io/bgfx/examples.html#mesh"
//	);