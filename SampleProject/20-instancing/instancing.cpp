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
#include "Component/DefaultController.h"
#include "Component/Transform.h"

#include "Serialization/ToJson.h"
#include "Serialization/FromJson.h"

#include "IO/FileSystem.h"
#include "IO/Stream.h"
#include "IO/File.h"
#include "Resource/ResourceCache.h"

#include "Resource/Mesh.h"


using namespace Auto3D;

namespace
{
struct PosColorVertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_abgr;

	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	};

	static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex s_cubeVertices[8] =
{
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t s_cubeIndices[36] =
{
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
};

class ExampleMesh : public Auto3D::IAppInstance
{
public:
	ExampleMesh(const char* _name, const char* _description, const char* _url)
		: Auto3D::IAppInstance(_name, _description, _url)
	{
	}

	void init(uint32_t _width, uint32_t _height) override
	{
		m_width = _width;
		m_height = _height;
		m_debug = BGFX_DEBUG_NONE;
		m_reset = BGFX_RESET_VSYNC;

		u_time = bgfx::createUniform("u_time", bgfx::UniformType::Vec4);
		GBox::_time = u_time;

		// Create program from shaders.
		m_program = loadProgram("vs_mesh", "fs_mesh");
		GBox::_program = m_program;

		//m_mesh = meshLoad("D:/Project/MyProject/Auto3D/Bin/meshes/bunny.bin");
		//m_mesh = meshLoad("meshes/bunny.bin");

		//m_mesh = new OMesh();
		//m_mesh->Load("meshes/bunny.bin");

		m_mesh = GResourceModule::Get().LoadResource<OMesh>("meshes/bunny.bin");

		m_timeOffset = bx::getHPCounter();
		GBox::_timeOffset = m_timeOffset;

		AWorld* world = FWorldContext::Get().NewWorld();
		world->SetName("world");
		world->DefineLayer(0, "Default");
		world->DefineLayer(1, "UI");
		world->DefineTag(0, "Default");
		world->DefineTag(1, "Player");

		AActor* actor = world->CreateChild<AActor>();
		ACameraComponent* camera = actor->CreateComponent<ACameraComponent>();
		actor->CreateComponent<ADefaultController>();
		actor->GetTransform()->SetPosition({ 0.0f, 1.0f, -2.5f });


		// Create vertex stream declaration.
		PosColorVertex::init();

		// Create static vertex buffer.
		GBox::_vbh = bgfx::createVertexBuffer(
			bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices))
			, PosColorVertex::ms_layout
		);

		// Create static index buffer.
		GBox::_ibh = bgfx::createIndexBuffer(
			bgfx::makeRef(s_cubeIndices, sizeof(s_cubeIndices))
		);

		// Create program from shaders.
		m_program = loadProgram("vs_instancing", "fs_instancing");
		GBox::_program = m_program;

		/*AActor* meshActor = world->CreateChild<AActor>();
		meshActor->GetTransform()->SetPosition({ 0.0f, 0.0f, 0.0f });
		meshActor->GetTransform()->SetRotation(FQuaternion(0.0f, 0.0f, 0.0f));
		meshActor->GetTransform()->SetScale({ 1.0f, 1.0f, 1.0f });
		AMeshComponent* meshComponent = meshActor->CreateComponent<AMeshComponent>();
		meshComponent->SetMesh(m_mesh);

		FString fileJsonName = "Serialize_SerializeFile.json";
		UPtr<FStream> streamJson(new FFile(ExecutableDir() + fileJsonName, EFileMode::WRITE));
		world->SaveJson(*streamJson);*/
		//ToJson("D:/Project/MyProject/Auto3D/Bin/output_utf32le.json",world); // serialize the circle to 'json_string'

		//FromJson("D:/Project/MyProject/Auto3D/Bin/output_utf32le.json");
	}

	int shutdown() override
	{
		//imguiDestroy();

		m_mesh->unload();

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
	OMesh* m_mesh;
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
