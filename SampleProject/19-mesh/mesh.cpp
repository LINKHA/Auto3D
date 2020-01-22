#include "RHI/RHI.h"
#include "RHI/bgfx_utils.h"
#include "UI/UI.h"
#include "Application.h"
#include "Component/MeshComponent.h"
#include "Renderer/ForwardShadingRenderer.h"

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

		//bgfx::Init init;
		//init.type     = args._type;
		//init.vendorId = args._pciId;
		//init.resolution.width  = m_width;
		//init.resolution.height = m_height;
		//init.resolution.reset  = m_reset;
		//bgfx::init(init);

		//// Enable debug text.
		//bgfx::setDebug(m_debug);

		//// Set view 0 clear state.
		//bgfx::setViewClear(0
		//		, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
		//		, 0x303030ff
		//		, 1.0f
		//		, 0
		//		);

		u_time = bgfx::createUniform("u_time", bgfx::UniformType::Vec4);
		GBox::_time = u_time;

		// Create program from shaders.
		m_program = loadProgram("vs_mesh", "fs_mesh");
		GBox::_program = m_program;

		m_mesh = meshLoad("meshes/bunny.bin");
		GBox::_mesh = m_mesh;

		m_timeOffset = bx::getHPCounter();
		GBox::_timeOffset = m_timeOffset;

		//imguiCreate();
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
		if (!Auto3D::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState) )
		{
			GBox::_width = m_width;
			GBox::_height = m_height;
			GBox::_mouseState = m_mouseState;

			imguiBeginFrame(m_mouseState._mx
				, m_mouseState._my
				, (m_mouseState._buttons[Auto3D::MouseButton::Left] ? IMGUI_MBUT_LEFT : 0)
				| (m_mouseState._buttons[Auto3D::MouseButton::Right] ? IMGUI_MBUT_RIGHT : 0)
				| (m_mouseState._buttons[Auto3D::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0)
				, m_mouseState._mz
				, uint16_t(m_width)
				, uint16_t(m_height)
				);

			showExampleDialog(this);

			imguiEndFrame();


			return true;
		}

		return false;
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
