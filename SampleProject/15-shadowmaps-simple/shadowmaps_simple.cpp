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

#include "Serialization/ToJson.h"
#include "Serialization/FromJson.h"

#include "IO/FileSystem.h"
#include "IO/Stream.h"
#include "IO/File.h"
#include "Resource/ResourceCache.h"
#include "Resource/Material.h"
#include "Time/Time.h"
#include "Resource/Mesh.h"


using namespace Auto3D;

namespace
{
#define RENDER_SHADOW_PASS_ID 0
#define RENDER_SCENE_PASS_ID  1

	struct PosNormalVertex
	{
		float    m_x;
		float    m_y;
		float    m_z;
		uint32_t m_normal;

		static void init()
		{
			ms_layout
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
				.end();
		};

		static bgfx::VertexLayout ms_layout;
	};

	bgfx::VertexLayout PosNormalVertex::ms_layout;

	static PosNormalVertex s_hplaneVertices[] =
	{
		{ -1.0f, 0.0f,  1.0f, encodeNormalRgba8(0.0f, 1.0f, 0.0f) },
		{  1.0f, 0.0f,  1.0f, encodeNormalRgba8(0.0f, 1.0f, 0.0f) },
		{ -1.0f, 0.0f, -1.0f, encodeNormalRgba8(0.0f, 1.0f, 0.0f) },
		{  1.0f, 0.0f, -1.0f, encodeNormalRgba8(0.0f, 1.0f, 0.0f) },
	};

	static const uint16_t s_planeIndices[] =
	{
		0, 1, 2,
		1, 3, 2,
	};

class ExampleShadowmapsSimple : public Auto3D::IAppInstance
{
public:
	ExampleShadowmapsSimple(const char* _name, const char* _description, const char* _url)
		: Auto3D::IAppInstance(_name, _description, _url)
	{
	}

	void init(uint32_t _width, uint32_t _height) override
	{
		/*GResourceModule::Get().AddResourceDir(ExecutableDir() + "Data");
		_mesh = GResourceModule::Get().LoadResource<OMesh>("Meshes/bunny.bin");

		_material = GResourceModule::Get().LoadResource<OMaterial>("Material/Test.json");

		AWorld* world = FWorldContext::Get().NewWorld();
		world->SetName("world");
		world->DefineLayer(0, "Default");
		world->DefineLayer(1, "UI");
		world->DefineTag(0, "Default");
		world->DefineTag(1, "Player");

		AActor* actor = world->CreateChild<AActor>();
		ACameraComponent* camera = actor->CreateComponent<ACameraComponent>();
		actor->CreateComponent<ADefaultControllerComponent>();
		actor->GetTransform()->SetPosition({ 0.0f, 1.0f, -2.5f });*/

		_material = GResourceModule::Get().LoadResource<OMaterial>("Material/MeshShadowTest.json");
		// Uniforms.
		s_shadowMap = bgfx::createUniform("s_shadowMap", bgfx::UniformType::Sampler);
		u_lightPos = bgfx::createUniform("u_lightPos", bgfx::UniformType::Vec4);
		u_lightMtx = bgfx::createUniform("u_lightMtx", bgfx::UniformType::Mat4);

		// Get renderer capabilities info.
		const bgfx::Caps* caps = bgfx::getCaps();

		// Create vertex stream declaration.
		PosNormalVertex::init();

		// Meshes.
		m_bunny = GResourceModule::Get().LoadResource<OMesh>("Meshes/bunny.bin");
		m_cube = GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin");
		m_hollowcube = GResourceModule::Get().LoadResource<OMesh>("Meshes/hollowcube.bin");

		m_vbh = bgfx::createVertexBuffer(
			bgfx::makeRef(s_hplaneVertices, sizeof(s_hplaneVertices))
			, PosNormalVertex::ms_layout
		);

		m_ibh = bgfx::createIndexBuffer(
			bgfx::makeRef(s_planeIndices, sizeof(s_planeIndices))
		);

		// Render targets.
		m_shadowMapSize = 512;

		// Shadow samplers are supported at least partially supported if texture
		// compare less equal feature is supported.
		m_shadowSamplerSupported = 0 != (caps->supported & BGFX_CAPS_TEXTURE_COMPARE_LEQUAL);
		m_useShadowSampler = m_shadowSamplerSupported;

		m_shadowMapFB = BGFX_INVALID_HANDLE;
		m_progShadow = BGFX_INVALID_HANDLE;
		m_progMesh = BGFX_INVALID_HANDLE;

		m_state[0] = OMesh::meshStateCreate();
		m_state[0]->_state = 0;
		m_state[0]->_program = m_progShadow;
		m_state[0]->_viewId = RENDER_SHADOW_PASS_ID;
		m_state[0]->_numTextures = 0;

		m_state[1] = OMesh::meshStateCreate();
		m_state[1]->_state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CCW
			| BGFX_STATE_MSAA
			;

		m_state[1]->_program = m_progMesh;
		m_state[1]->_viewId = RENDER_SCENE_PASS_ID;
		m_state[1]->_numTextures = 1;
		m_state[1]->_textures[0]._flags = UINT32_MAX;
		m_state[1]->_textures[0]._stage = 0;
		m_state[1]->_textures[0]._sampler = s_shadowMap;
		m_state[1]->_textures[0]._texture = BGFX_INVALID_HANDLE;

		// Set view and projection matrices.

		const bx::Vec3 at = { 0.0f,  5.0f,   0.0f };
		const bx::Vec3 eye = { 0.0f, 30.0f, -60.0f };
		bx::mtxLookAt(m_view, eye, at);

		const float aspect = float(int32_t(AUTO_DEFAULT_WIDTH)) / float(int32_t(AUTO_DEFAULT_HEIGHT));
		bx::mtxProj(m_proj, 60.0f, aspect, 0.1f, 1000.0f, bgfx::getCaps()->homogeneousDepth);

		m_timeOffset = bx::getHPCounter();
	}

	int shutdown() override
	{
		return 0;
	}

	bool update() override
	{
			int64_t now = bx::getHPCounter();
			const double freq = double(bx::getHPFrequency());
			float time = float((now - m_timeOffset) / freq);

			if (!bgfx::isValid(m_shadowMapFB) /*|| shadowSamplerModeChanged*/)
			{
				bgfx::TextureHandle shadowMapTexture;

				if (bgfx::isValid(m_progShadow))
					bgfx::destroy(m_progShadow);
				if (bgfx::isValid(m_progMesh))
					bgfx::destroy(m_progMesh);

				if (bgfx::isValid(m_shadowMapFB))
					bgfx::destroy(m_shadowMapFB);

				if (m_useShadowSampler)
				{
					// Depth textures and shadow samplers are supported.
					m_progShadow = loadProgram("vs_sms_shadow", "fs_sms_shadow");
					m_progMesh = loadProgram("vs_sms_mesh", "fs_sms_mesh");

					bgfx::TextureHandle fbtextures[] =
					{
						bgfx::createTexture2D(
							  m_shadowMapSize
							, m_shadowMapSize
							, false
							, 1
							, bgfx::TextureFormat::D16
							, BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
							),
					};
					shadowMapTexture = fbtextures[0];
					m_shadowMapFB = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);
				}
				

				m_state[0]->_program = m_progShadow;
				m_state[0]->_state = 0
					| (m_useShadowSampler ? 0 : BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A)
					| BGFX_STATE_WRITE_Z
					| BGFX_STATE_DEPTH_TEST_LESS
					| BGFX_STATE_CULL_CCW
					| BGFX_STATE_MSAA
					;

				m_state[1]->_program = m_progMesh;
				m_state[1]->_textures[0]._texture = shadowMapTexture;
			}

			// Setup lights.
			float lightPos[4];
			lightPos[0] = -bx::cos(time);
			lightPos[1] = -1.0f;
			lightPos[2] = -bx::sin(time);
			lightPos[3] = 0.0f;

			bgfx::setUniform(u_lightPos, lightPos);

			// Setup instance matrices.
			float mtxFloor[16];
			bx::mtxSRT(mtxFloor
				, 30.0f, 30.0f, 30.0f
				, 0.0f, 0.0f, 0.0f
				, 0.0f, 0.0f, 0.0f
			);

			float mtxBunny[16];
			bx::mtxSRT(mtxBunny
				, 5.0f, 5.0f, 5.0f
				, 0.0f, bx::kPi - time, 0.0f
				, 15.0f, 5.0f, 0.0f
			);

			float mtxHollowcube[16];
			bx::mtxSRT(mtxHollowcube
				, 2.5f, 2.5f, 2.5f
				, 0.0f, 1.56f - time, 0.0f
				, 0.0f, 10.0f, 0.0f
			);

			float mtxCube[16];
			bx::mtxSRT(mtxCube
				, 2.5f, 2.5f, 2.5f
				, 0.0f, 1.56f - time, 0.0f
				, -15.0f, 5.0f, 0.0f
			);

			// Define matrices.
			float lightView[16];
			float lightProj[16];

			const bx::Vec3 at = { 0.0f,  0.0f,   0.0f };
			const bx::Vec3 eye = { -lightPos[0], -lightPos[1], -lightPos[2] };
			bx::mtxLookAt(lightView, eye, at);

			const bgfx::Caps* caps = bgfx::getCaps();
			const float area = 30.0f;
			bx::mtxOrtho(lightProj, -area, area, -area, area, -100.0f, 100.0f, 0.0f, caps->homogeneousDepth);

			bgfx::setViewRect(RENDER_SHADOW_PASS_ID, 0, 0, m_shadowMapSize, m_shadowMapSize);
			bgfx::setViewFrameBuffer(RENDER_SHADOW_PASS_ID, m_shadowMapFB);
			bgfx::setViewTransform(RENDER_SHADOW_PASS_ID, lightView, lightProj);

			bgfx::setViewRect(RENDER_SCENE_PASS_ID, 0, 0, uint16_t(AUTO_DEFAULT_WIDTH), uint16_t(AUTO_DEFAULT_HEIGHT));
			bgfx::setViewTransform(RENDER_SCENE_PASS_ID, m_view, m_proj);

			// Clear backbuffer and shadowmap framebuffer at beginning.
			bgfx::setViewClear(RENDER_SHADOW_PASS_ID
				, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
				, 0x303030ff, 1.0f, 0
			);

			bgfx::setViewClear(RENDER_SCENE_PASS_ID
				, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
				, 0x303030ff, 1.0f, 0
			);

			// Render.
			float mtxShadow[16];
			float lightMtx[16];

			const float sy = caps->originBottomLeft ? 0.5f : -0.5f;
			const float sz = caps->homogeneousDepth ? 0.5f : 1.0f;
			const float tz = caps->homogeneousDepth ? 0.5f : 0.0f;
			const float mtxCrop[16] =
			{
				0.5f, 0.0f, 0.0f, 0.0f,
				0.0f,   sy, 0.0f, 0.0f,
				0.0f, 0.0f, sz,   0.0f,
				0.5f, 0.5f, tz,   1.0f,
			};

			float mtxTmp[16];
			bx::mtxMul(mtxTmp, lightProj, mtxCrop);
			bx::mtxMul(mtxShadow, lightView, mtxTmp);

			// Floor.
			bx::mtxMul(lightMtx, mtxFloor, mtxShadow);
			uint32_t cached = bgfx::setTransform(mtxFloor);
			for (uint32_t pass = 0; pass < 2; ++pass)
			{
				const FMeshState& st = *m_state[pass];
				bgfx::setTransform(cached);
				for (uint8_t tex = 0; tex < st._numTextures; ++tex)
				{
					const FMeshState::Texture& texture = st._textures[tex];
					bgfx::setTexture(texture._stage
						, texture._sampler
						, texture._texture
						, texture._flags
					);
				}
				bgfx::setUniform(u_lightMtx, lightMtx);
				bgfx::setIndexBuffer(m_ibh);
				bgfx::setVertexBuffer(0, m_vbh);
				bgfx::setState(st._state);
				bgfx::submit(st._viewId, st._program);
			}

			// Bunny.
			bx::mtxMul(lightMtx, mtxBunny, mtxShadow);
			bgfx::setUniform(u_lightMtx, lightMtx);
			m_bunny->submit(&m_state[0], 1, mtxBunny);
			bgfx::setUniform(u_lightMtx, lightMtx);
			m_bunny->submit(&m_state[1], 1, mtxBunny);

			//// Hollow cube.
			//bx::mtxMul(lightMtx, mtxHollowcube, mtxShadow);
			//bgfx::setUniform(u_lightMtx, lightMtx);
			//m_hollowcube->submit(&m_state[0], 1, mtxHollowcube);
			//bgfx::setUniform(u_lightMtx, lightMtx);
			//m_hollowcube->submit(&m_state[0], 1, mtxHollowcube);

			// Cube.
			bx::mtxMul(lightMtx, mtxCube, mtxShadow);
			bgfx::setUniform(u_lightMtx, lightMtx);
			m_cube->submit(&m_state[0], 1, mtxCube);
			bgfx::setUniform(u_lightMtx, lightMtx);
			m_cube->submit(&m_state[1], 1, mtxCube);

			// Advance to next frame. Rendering thread will be kicked to
			// process submitted rendering primitives.
			bgfx::frame();

		return true;
	}

	OMesh* _mesh;
	OMaterial* _material;

	bgfx::UniformHandle s_shadowMap;
	bgfx::UniformHandle u_lightPos;
	bgfx::UniformHandle u_lightMtx;

	bgfx::UniformHandle u_depthScaleOffset;

	OMesh* m_bunny;
	OMesh* m_cube;
	OMesh* m_hollowcube;

	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;

	uint16_t m_shadowMapSize;

	bgfx::ProgramHandle m_progShadow;
	bgfx::ProgramHandle m_progMesh;
	bgfx::FrameBufferHandle m_shadowMapFB;

	bool m_shadowSamplerSupported;
	bool m_useShadowSampler;

	FMeshState* m_state[2];

	float m_view[16];
	float m_proj[16];

	int64_t m_timeOffset;
};

} // namespace

int Auto3D_main(int _argc, char** _argv)
{
	ExampleShadowmapsSimple app(
		"04-mesh"
		, "Loading meshes."
		, "https://bkaradzic.github.io/bgfx/examples.html#mesh"
	);
	return FApplication::Get().RunAppInstance(&app, _argc, _argv);
}
