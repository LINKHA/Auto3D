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
		GResourceModule::Get().AddResourceDir(ExecutableDir() + "Data");
		_mesh = GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin");

		_material = GResourceModule::Get().LoadResource<OMaterial>("Material/MeshShadowTest.json");

		AWorld* world = FWorldContext::Get().NewWorld();
		world->SetName("world");
		world->DefineLayer(0, "Default");
		world->DefineLayer(1, "UI");
		world->DefineTag(0, "Default");
		world->DefineTag(1, "Player");

		AActor* actor = world->CreateChild<AActor>();
		ACameraComponent* camera = actor->CreateComponent<ACameraComponent>();
		actor->CreateComponent<ADefaultControllerComponent>();
		actor->GetTransform()->SetPosition({ 0.0f, 1.0f, -2.5f });

		_cube = world->CreateChild<AActor>();
		_cube->GetTransform()->SetPosition({ 0.0f, 10.0f, 0.0f });
		_cube->GetTransform()->SetRotation(FQuaternion(0.0f, 0.0f, 0.0f));
		_cube->GetTransform()->SetScale({ 4.0f, 4.0f, 4.0f });
		AMeshComponent* meshComponent = _cube->CreateComponent<AMeshComponent>();
		meshComponent->SetMesh(_mesh);
		meshComponent->SetMaterial(_material);

		_plane = world->CreateChild<AActor>();
		_plane->GetTransform()->SetPosition({ 0.0f, 0.0f, 0.0f });
		_plane->GetTransform()->SetRotation(FQuaternion(0.0f, 0.0f, 0.0f));
		_plane->GetTransform()->SetScale({ 50.0f, 1.0f, 50.0f });
		AMeshComponent* planeMeshComponent = _plane->CreateComponent<AMeshComponent>();
		planeMeshComponent->SetMesh(_mesh);
		planeMeshComponent->SetMaterial(_material);

		_light = world->CreateChild<AActor>();
		_light->GetTransform()->SetPosition({ 10.0f, 10.0f, 10.0f });
		_light->GetTransform()->SetRotation(FQuaternion(45.0f, -45.0f, 45.0f));
		_lightMeshComponent = _light->CreateComponent<ALightComponent>();

		// Uniforms.
		s_shadowMap = bgfx::createUniform("s_shadowMap", bgfx::UniformType::Sampler);
		u_lightPos = bgfx::createUniform("u_lightPos", bgfx::UniformType::Vec4);
		u_lightMtx = bgfx::createUniform("u_lightMtx", bgfx::UniformType::Mat4);

		// Get renderer capabilities info.
		const bgfx::Caps* caps = bgfx::getCaps();


		// Render targets.
		m_shadowMapSize = 1024;

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
			TVector3F lightPosition =  _light->GetTransform()->GetWorldPosition();
			bgfx::setUniform(u_lightPos, TVector4F(-lightPosition, 0.0f).Data());

			TMatrix4x4F& lightProj_ = _lightMeshComponent->GetLightProj();
			TMatrix4x4F& lightView_ = _lightMeshComponent->GetLightView();

			bgfx::setViewRect(RENDER_SHADOW_PASS_ID, 0, 0, m_shadowMapSize, m_shadowMapSize);
			bgfx::setViewFrameBuffer(RENDER_SHADOW_PASS_ID, m_shadowMapFB);
			bgfx::setViewTransform(RENDER_SHADOW_PASS_ID, lightView_.Data(), lightProj_.Data());

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
			float lightMtx[16];


			TMatrix4x4F& mtxShadow_ = _lightMeshComponent->GetMtxShadow();
			// Floor.
			TMatrix4x4F& floorMatrix = _plane->GetTransform()->GetWorldTransform().ToMatrix4().Transpose();
			bx::mtxMul(lightMtx, floorMatrix.Data(), mtxShadow_.Data());
			bgfx::setUniform(u_lightMtx, lightMtx);
			_mesh->submit(&m_state[0], 1, floorMatrix.Data());
			bgfx::setUniform(u_lightMtx, lightMtx);
			_mesh->submit(&m_state[1], 1, floorMatrix.Data());

			// Cube.
			TMatrix4x4F& modelMatrix = _cube->GetTransform()->GetWorldTransform().ToMatrix4().Transpose();
			bx::mtxMul(lightMtx, modelMatrix.Data(), mtxShadow_.Data());
			bgfx::setUniform(u_lightMtx, lightMtx);
			_mesh->submit(&m_state[0], 1, modelMatrix.Data());
			bgfx::setUniform(u_lightMtx, lightMtx);
			_mesh->submit(&m_state[1], 1, modelMatrix.Data());

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

	AActor* _cube;
	AActor* _plane;
	AActor* _light;
	ALightComponent* _lightMeshComponent;
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
