#include "RHI/RHI.h"
#include "RHI/bgfx_utils.h"
#include "UI/UI.h"
#include "Application.h"
#include "Component/MeshComponent.h"

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

		// Create program from shaders.
		m_program = loadProgram("vs_mesh", "fs_mesh");

		m_mesh = meshLoad("meshes/bunny.bin");

		m_timeOffset = bx::getHPCounter();

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

			// Set view 0 default viewport.
			bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height) );

			// This dummy draw call is here to make sure that view 0 is cleared
			// if no other draw calls are submitted to view 0.
			bgfx::touch(0);

			float time = (float)( (bx::getHPCounter()-m_timeOffset)/double(bx::getHPFrequency() ) );
			bgfx::setUniform(u_time, &time);

			const bx::Vec3 at  = { 0.0f, 1.0f,  0.0f };
			const bx::Vec3 eye = { 0.0f, 1.0f, -2.5f };

			// Set view and projection matrix for view 0.
			{
				float view[16];
				bx::mtxLookAt(view, eye, at);

				float proj[16];
				bx::mtxProj(proj, 60.0f, float(m_width)/float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
				bgfx::setViewTransform(0, view, proj);

				// Set view 0 default viewport.
				bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height) );
			}

			float mtx[16];
			bx::mtxRotateXY(mtx
				, 0.0f
				, time*0.37f
				);

			meshSubmit(m_mesh, 0, m_program, mtx);

			// Advance to next frame. Rendering thread will be kicked to
			// process submitted rendering primitives.
			bgfx::frame();

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
