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

#include "Serialization/ToJson.h"
#include "Serialization/FromJson.h"

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

namespace bgfx
{
	int32_t read(bx::ReaderI* _reader, bgfx::VertexLayout& _layout, bx::Error* _err = NULL);
}

namespace
{

#define RENDERVIEW_SHADOWMAP_0_ID 1
#define RENDERVIEW_SHADOWMAP_1_ID 2
#define RENDERVIEW_SHADOWMAP_2_ID 3
#define RENDERVIEW_SHADOWMAP_3_ID 4
#define RENDERVIEW_SHADOWMAP_4_ID 5
#define RENDERVIEW_VBLUR_0_ID     6
#define RENDERVIEW_HBLUR_0_ID     7
#define RENDERVIEW_VBLUR_1_ID     8
#define RENDERVIEW_HBLUR_1_ID     9
#define RENDERVIEW_VBLUR_2_ID     10
#define RENDERVIEW_HBLUR_2_ID     11
#define RENDERVIEW_VBLUR_3_ID     12
#define RENDERVIEW_HBLUR_3_ID     13
#define RENDERVIEW_DRAWSCENE_0_ID 14
#define RENDERVIEW_DRAWSCENE_1_ID 15
#define RENDERVIEW_DRAWDEPTH_0_ID 16
#define RENDERVIEW_DRAWDEPTH_1_ID 17
#define RENDERVIEW_DRAWDEPTH_2_ID 18
#define RENDERVIEW_DRAWDEPTH_3_ID 19


static const float s_texcoord = 5.0f;
static PosNormalTexcoordVertex s_hplaneVertices[] =
{
	{ -1.0f, 0.0f,  1.0f, encodeNormalRgba8(0.0f, 1.0f, 0.0f), s_texcoord, s_texcoord },
	{  1.0f, 0.0f,  1.0f, encodeNormalRgba8(0.0f, 1.0f, 0.0f), s_texcoord, 0.0f       },
	{ -1.0f, 0.0f, -1.0f, encodeNormalRgba8(0.0f, 1.0f, 0.0f), 0.0f,       s_texcoord },
	{  1.0f, 0.0f, -1.0f, encodeNormalRgba8(0.0f, 1.0f, 0.0f), 0.0f,       0.0f       },
};

static PosNormalTexcoordVertex s_vplaneVertices[] =
{
	{ -1.0f,  1.0f, 0.0f, encodeNormalRgba8(0.0f, 0.0f, -1.0f), 1.0f, 1.0f },
	{  1.0f,  1.0f, 0.0f, encodeNormalRgba8(0.0f, 0.0f, -1.0f), 1.0f, 0.0f },
	{ -1.0f, -1.0f, 0.0f, encodeNormalRgba8(0.0f, 0.0f, -1.0f), 0.0f, 1.0f },
	{  1.0f, -1.0f, 0.0f, encodeNormalRgba8(0.0f, 0.0f, -1.0f), 0.0f, 0.0f },
};

static const uint16_t s_planeIndices[] =
{
	0, 1, 2,
	1, 3, 2,
};


void mtxBillboard(float* __restrict _result
				  , const float* __restrict _view
				  , const float* __restrict _pos
				  , const float* __restrict _scale)
{
	_result[ 0] = _view[0]  * _scale[0];
	_result[ 1] = _view[4]  * _scale[0];
	_result[ 2] = _view[8]  * _scale[0];
	_result[ 3] = 0.0f;
	_result[ 4] = _view[1]  * _scale[1];
	_result[ 5] = _view[5]  * _scale[1];
	_result[ 6] = _view[9]  * _scale[1];
	_result[ 7] = 0.0f;
	_result[ 8] = _view[2]  * _scale[2];
	_result[ 9] = _view[6]  * _scale[2];
	_result[10] = _view[10] * _scale[2];
	_result[11] = 0.0f;
	_result[12] = _pos[0];
	_result[13] = _pos[1];
	_result[14] = _pos[2];
	_result[15] = 1.0f;
}

void mtxYawPitchRoll(float* __restrict _result
		            , float _yaw
		            , float _pitch
		            , float _roll
		            )
{
	float sroll  = bx::sin(_roll);
	float croll  = bx::cos(_roll);
	float spitch = bx::sin(_pitch);
	float cpitch = bx::cos(_pitch);
	float syaw   = bx::sin(_yaw);
	float cyaw   = bx::cos(_yaw);

	_result[ 0] = sroll * spitch * syaw + croll * cyaw;
	_result[ 1] = sroll * cpitch;
	_result[ 2] = sroll * spitch * cyaw - croll * syaw;
	_result[ 3] = 0.0f;
	_result[ 4] = croll * spitch * syaw - sroll * cyaw;
	_result[ 5] = croll * cpitch;
	_result[ 6] = croll * spitch * cyaw + sroll * syaw;
	_result[ 7] = 0.0f;
	_result[ 8] = cpitch * syaw;
	_result[ 9] = -spitch;
	_result[10] = cpitch * cyaw;
	_result[11] = 0.0f;
	_result[12] = 0.0f;
	_result[13] = 0.0f;
	_result[14] = 0.0f;
	_result[15] = 1.0f;
}


static RenderState s_renderStates[RenderState::Count] =
{
	{ // Default
		0
		| BGFX_STATE_WRITE_RGB
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_CULL_CCW
		| BGFX_STATE_MSAA
		, UINT32_MAX
		, BGFX_STENCIL_NONE
		, BGFX_STENCIL_NONE
	},
	{ // ShadowMap_PackDepth
		0
		| BGFX_STATE_WRITE_RGB
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_CULL_CCW
		| BGFX_STATE_MSAA
		, UINT32_MAX
		, BGFX_STENCIL_NONE
		, BGFX_STENCIL_NONE
	},
	{ // ShadowMap_PackDepthHoriz
		0
		| BGFX_STATE_WRITE_RGB
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_CULL_CCW
		| BGFX_STATE_MSAA
		, UINT32_MAX
		, BGFX_STENCIL_TEST_EQUAL
		| BGFX_STENCIL_FUNC_REF(1)
		| BGFX_STENCIL_FUNC_RMASK(0xff)
		| BGFX_STENCIL_OP_FAIL_S_KEEP
		| BGFX_STENCIL_OP_FAIL_Z_KEEP
		| BGFX_STENCIL_OP_PASS_Z_KEEP
		, BGFX_STENCIL_NONE
	},
	{ // ShadowMap_PackDepthVert
		0
		| BGFX_STATE_WRITE_RGB
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_CULL_CCW
		| BGFX_STATE_MSAA
		, UINT32_MAX
		, BGFX_STENCIL_TEST_EQUAL
		| BGFX_STENCIL_FUNC_REF(0)
		| BGFX_STENCIL_FUNC_RMASK(0xff)
		| BGFX_STENCIL_OP_FAIL_S_KEEP
		| BGFX_STENCIL_OP_FAIL_Z_KEEP
		| BGFX_STENCIL_OP_PASS_Z_KEEP
		, BGFX_STENCIL_NONE
	},
	{ // Custom_BlendLightTexture
		BGFX_STATE_WRITE_RGB
		| BGFX_STATE_WRITE_A
		| BGFX_STATE_WRITE_Z
		| BGFX_STATE_DEPTH_TEST_LESS
		| BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_COLOR, BGFX_STATE_BLEND_INV_SRC_COLOR)
		| BGFX_STATE_CULL_CCW
		| BGFX_STATE_MSAA
		, UINT32_MAX
		, BGFX_STENCIL_NONE
		, BGFX_STENCIL_NONE
	},
	{ // Custom_DrawPlaneBottom
		BGFX_STATE_WRITE_RGB
		| BGFX_STATE_CULL_CW
		| BGFX_STATE_MSAA
		, UINT32_MAX
		, BGFX_STENCIL_NONE
		, BGFX_STENCIL_NONE
	},
};


struct Aabb
{
	float m_min[3];
	float m_max[3];
};

struct Obb
{
	float m_mtx[16];
};

struct Sphere
{
	float m_center[3];
	float m_radius;
};

struct Primitive
{
	uint32_t m_startIndex;
	uint32_t m_numIndices;
	uint32_t m_startVertex;
	uint32_t m_numVertices;

	Sphere m_sphere;
	Aabb m_aabb;
	Obb m_obb;
};

typedef std::vector<Primitive> PrimitiveArray;

struct Group
{
	Group()
	{
		reset();
	}

	void reset()
	{
		m_vbh.idx = bgfx::kInvalidHandle;
		m_ibh.idx = bgfx::kInvalidHandle;
		m_prims.clear();
	}

	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
	Sphere m_sphere;
	Aabb m_aabb;
	Obb m_obb;
	PrimitiveArray m_prims;
};

struct Mesh
{
	void load(const void* _vertices, uint32_t _numVertices, const bgfx::VertexLayout _layout, const uint16_t* _indices, uint32_t _numIndices)
	{
		Group group;
		const bgfx::Memory* mem;
		uint32_t size;

		size = _numVertices*_layout.getStride();
		mem = bgfx::makeRef(_vertices, size);
		group.m_vbh = bgfx::createVertexBuffer(mem, _layout);

		size = _numIndices*2;
		mem = bgfx::makeRef(_indices, size);
		group.m_ibh = bgfx::createIndexBuffer(mem);

		m_groups.push_back(group);
	}

	void load(const char* _filePath)
	{
#define BGFX_CHUNK_MAGIC_VB  BX_MAKEFOURCC('V', 'B', ' ', 0x1)
#define BGFX_CHUNK_MAGIC_IB  BX_MAKEFOURCC('I', 'B', ' ', 0x0)
#define BGFX_CHUNK_MAGIC_PRI BX_MAKEFOURCC('P', 'R', 'I', 0x0)

		bx::FileReaderI* reader = FDefaultFileWriterReader::GetFileReader();
		bx::open(reader, _filePath);

		Group group;

		uint32_t chunk;
		while (4 == bx::read(reader, chunk) )
		{
			switch (chunk)
			{
			case BGFX_CHUNK_MAGIC_VB:
				{
					bx::read(reader, group.m_sphere);
					bx::read(reader, group.m_aabb);
					bx::read(reader, group.m_obb);

					bgfx::read(reader, m_layout);
					uint16_t stride = m_layout.getStride();

					uint16_t numVertices;
					bx::read(reader, numVertices);
					const bgfx::Memory* mem = bgfx::alloc(numVertices*stride);
					bx::read(reader, mem->data, mem->size);

					group.m_vbh = bgfx::createVertexBuffer(mem, m_layout);
				}
				break;

			case BGFX_CHUNK_MAGIC_IB:
				{
					uint32_t numIndices;
					bx::read(reader, numIndices);
					const bgfx::Memory* mem = bgfx::alloc(numIndices*2);
					bx::read(reader, mem->data, mem->size);
					group.m_ibh = bgfx::createIndexBuffer(mem);
				}
				break;

			case BGFX_CHUNK_MAGIC_PRI:
				{
					uint16_t len;
					bx::read(reader, len);

					std::string material;
					material.resize(len);
					bx::read(reader, const_cast<char*>(material.c_str() ), len);

					uint16_t num;
					bx::read(reader, num);

					for (uint32_t ii = 0; ii < num; ++ii)
					{
						bx::read(reader, len);

						std::string name;
						name.resize(len);
						bx::read(reader, const_cast<char*>(name.c_str() ), len);

						Primitive prim;
						bx::read(reader, prim.m_startIndex);
						bx::read(reader, prim.m_numIndices);
						bx::read(reader, prim.m_startVertex);
						bx::read(reader, prim.m_numVertices);
						bx::read(reader, prim.m_sphere);
						bx::read(reader, prim.m_aabb);
						bx::read(reader, prim.m_obb);

						group.m_prims.push_back(prim);
					}

					m_groups.push_back(group);
					group.reset();
				}
				break;

			default:
				DBG("%08x at %d", chunk, bx::seek(reader) );
				break;
			}
		}

		bx::close(reader);
	}

	void unload()
	{
		for (GroupArray::const_iterator it = m_groups.begin(), itEnd = m_groups.end(); it != itEnd; ++it)
		{
			const Group& group = *it;
			bgfx::destroy(group.m_vbh);

			if (bgfx::kInvalidHandle != group.m_ibh.idx)
			{
				bgfx::destroy(group.m_ibh);
			}
		}
		m_groups.clear();
	}

	void submit(uint8_t _viewId, float* _mtx, bgfx::ProgramHandle _program, const RenderState& _renderState, bool _submitShadowMaps = false)
	{
		bgfx::TextureHandle texture = BGFX_INVALID_HANDLE;
		submit(_viewId, _mtx, _program, _renderState, texture, _submitShadowMaps);
	}

	void submit(uint8_t _viewId, float* _mtx, bgfx::ProgramHandle _program, const RenderState& _renderState, bgfx::TextureHandle _texture, bool _submitShadowMaps = false)
	{
		for (GroupArray::const_iterator it = m_groups.begin(), itEnd = m_groups.end(); it != itEnd; ++it)
		{
			const Group& group = *it;

			// Set uniforms.
			FShadowRenderer::s_uniforms.submitPerDrawUniforms();

			// Set model matrix for rendering.
			bgfx::setTransform(_mtx);
			bgfx::setIndexBuffer(group.m_ibh);
			bgfx::setVertexBuffer(0, group.m_vbh);

			// Set textures.
			if (bgfx::kInvalidHandle != _texture.idx)
			{
				bgfx::setTexture(0, FShadowRenderer::s_texColor, _texture);
			}

			if (_submitShadowMaps)
			{
				for (uint8_t ii = 0; ii < ShadowMapRenderTargets::Count; ++ii)
				{
					bgfx::setTexture(4 + ii, FShadowRenderer::s_shadowMap[ii], bgfx::getTexture(FShadowRenderer::s_rtShadowMap[ii]) );
				}
			}

			// Apply render state.
			bgfx::setStencil(_renderState.m_fstencil, _renderState.m_bstencil);
			bgfx::setState(_renderState.m_state, _renderState.m_blendFactorRgba);

			// Submit.
			bgfx::submit(_viewId, _program);
		}
	}

	bgfx::VertexLayout m_layout;
	typedef std::vector<Group> GroupArray;
	GroupArray m_groups;
};

struct PosColorTexCoord0Vertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_rgba;
	float m_u;
	float m_v;

	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();
	}

	static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorTexCoord0Vertex::ms_layout;

void screenSpaceQuad(float _textureWidth, float _textureHeight, bool _originBottomLeft = true, float _width = 1.0f, float _height = 1.0f)
{
	if (3 == bgfx::getAvailTransientVertexBuffer(3, PosColorTexCoord0Vertex::ms_layout) )
	{
		bgfx::TransientVertexBuffer vb;
		bgfx::allocTransientVertexBuffer(&vb, 3, PosColorTexCoord0Vertex::ms_layout);
		PosColorTexCoord0Vertex* vertex = (PosColorTexCoord0Vertex*)vb.data;

		const float zz = 0.0f;

		const float minx = -_width;
		const float maxx =  _width;
		const float miny = 0.0f;
		const float maxy = _height*2.0f;

		const float texelHalfW = FShadowRenderer::s_texelHalf/_textureWidth;
		const float texelHalfH = FShadowRenderer::s_texelHalf/_textureHeight;
		const float minu = -1.0f + texelHalfW;
		const float maxu =  1.0f + texelHalfW;

		float minv = texelHalfH;
		float maxv = 2.0f + texelHalfH;

		if (_originBottomLeft)
		{
			std::swap(minv, maxv);
			minv -= 1.0f;
			maxv -= 1.0f;
		}

		vertex[0].m_x = minx;
		vertex[0].m_y = miny;
		vertex[0].m_z = zz;
		vertex[0].m_rgba = 0xffffffff;
		vertex[0].m_u = minu;
		vertex[0].m_v = minv;

		vertex[1].m_x = maxx;
		vertex[1].m_y = miny;
		vertex[1].m_z = zz;
		vertex[1].m_rgba = 0xffffffff;
		vertex[1].m_u = maxu;
		vertex[1].m_v = minv;

		vertex[2].m_x = maxx;
		vertex[2].m_y = maxy;
		vertex[2].m_z = zz;
		vertex[2].m_rgba = 0xffffffff;
		vertex[2].m_u = maxu;
		vertex[2].m_v = maxv;

		bgfx::setVertexBuffer(0, &vb);
	}
}

void worldSpaceFrustumCorners(float* _corners24f
	, float _near
	, float _far
	, float _projWidth
	, float _projHeight
	, const float* __restrict _invViewMtx
	)
{
	// Define frustum corners in view space.
	const float nw = _near * _projWidth;
	const float nh = _near * _projHeight;
	const float fw = _far  * _projWidth;
	const float fh = _far  * _projHeight;

	const uint8_t numCorners = 8;
	const bx::Vec3 corners[numCorners] =
	{
		{ -nw,  nh, _near },
		{  nw,  nh, _near },
		{  nw, -nh, _near },
		{ -nw, -nh, _near },
		{ -fw,  fh, _far  },
		{  fw,  fh, _far  },
		{  fw, -fh, _far  },
		{ -fw, -fh, _far  },
	};

	// Convert them to world space.
	float (*out)[3] = (float(*)[3])_corners24f;
	for (uint8_t ii = 0; ii < numCorners; ++ii)
	{
		bx::store(&out[ii], bx::mul(corners[ii], _invViewMtx) );
	}
}

/**
 * _splits = { near0, far0, near1, far1... nearN, farN }
 * N = _numSplits
 */
void splitFrustum(float* _splits, uint8_t _numSplits, float _near, float _far, float _splitWeight = 0.75f)
{
	const float l = _splitWeight;
	const float ratio = _far/_near;
	const int8_t numSlices = _numSplits*2;
	const float numSlicesf = float(numSlices);

	// First slice.
	_splits[0] = _near;

	for (uint8_t nn = 2, ff = 1; nn < numSlices; nn+=2, ff+=2)
	{
		float si = float(int8_t(ff) ) / numSlicesf;

		const float nearp = l*(_near*bx::pow(ratio, si) ) + (1 - l)*(_near + (_far - _near)*si);
		_splits[nn] = nearp;          //near
		_splits[ff] = nearp * 1.005f; //far from previous split
	}

	// Last slice.
	_splits[numSlices-1] = _far;
}

class ExampleShadowmaps : public Auto3D::IAppInstance
{
public:
	ExampleShadowmaps(const char* _name, const char* _description, const char* _url)
		: Auto3D::IAppInstance(_name, _description, _url)
	{
		IAppInstance::_useCustomRender = true;
	}

	void init() override
	{
		GProcessWindow& processWindow = GProcessWindow::Get();

		m_viewState = ViewState(uint16_t(processWindow._width), uint16_t(processWindow._height));
		m_clearValues = ClearValues(0x00000000, 1.0f, 0);


		AWorld* world = FWorldContext::Get().NewWorld();
		world->SetName("world");
		world->DefineLayer(0, "Default");
		world->DefineLayer(1, "UI");
		world->DefineTag(0, "Default");
		world->DefineTag(1, "Player");

		AActor* actor = world->CreateChild<AActor>();
		_camera = actor->CreateComponent<ACameraComponent>();
		actor->CreateComponent<ADefaultControllerComponent>();
		actor->GetTransform()->SetPosition({ 0.0f, 60.0f, -105.0f });
		actor->GetTransform()->SetRotation({ -45.0f, 0.0f, 0.0f });

		_meshActor = world->CreateChild<AActor>();
		_meshComponent = _meshActor->CreateComponent<AMeshComponent>();
		_meshComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin"));
		_meshActor->GetTransform()->SetPosition({ -15.0f, 5.0f, 0.0f });
		_meshActor->GetTransform()->SetScale({ 2.5f, 2.5f, 2.5f });

		_planeActor = world->CreateChild<AActor>();
		_planeComponent = _planeActor->CreateComponent<AMeshComponent>();
		_planeComponent->SetMesh(GResourceModule::Get().LoadResource<OMesh>("Meshes/cube.bin"));
		_planeActor->GetTransform()->SetScale({ 550.0f, 550.0f, 550.0f });

		FShadowRenderer::Get().init();

		

		// Vertex declarations.
		bgfx::VertexLayout PosNormalTexcoordLayout;
		PosNormalTexcoordLayout.begin()
			.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Normal,    4, bgfx::AttribType::Uint8, true, true)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();

		m_posLayout.begin();
		m_posLayout.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float);
		m_posLayout.end();

		PosColorTexCoord0Vertex::init();

		// Textures.
		m_texFigure     = loadTexture("textures/figure-rgba.dds");
		m_texFlare      = loadTexture("textures/flare.dds");
		m_texFieldstone = loadTexture("textures/fieldstone-rgba.dds");

		// Meshes.
		m_bunnyMesh.load("meshes/bunny.bin");
		m_treeMesh.load("meshes/tree.bin");
		m_cubeMesh.load("meshes/cube.bin");
		m_hollowcubeMesh.load("meshes/hollowcube.bin");
		m_hplaneMesh.load(s_hplaneVertices, BX_COUNTOF(s_hplaneVertices), PosNormalTexcoordLayout, s_planeIndices, BX_COUNTOF(s_planeIndices) );
		m_vplaneMesh.load(s_vplaneVertices, BX_COUNTOF(s_vplaneVertices), PosNormalTexcoordLayout, s_planeIndices, BX_COUNTOF(s_planeIndices) );

		
		
		//// Setup camera.
		//cameraCreate();
		//cameraSetPosition({ 0.0f, 60.0f, -105.0f });
		//cameraSetVerticalAngle(-0.45f);

		m_timeAccumulatorLight = 0.0f;
		m_timeAccumulatorScene = 0.0f;
	}

	virtual int shutdown() override
	{
		
		return 0;
	}

	bool update() override
	{

#pragma region gui
			//		FShadowRenderer::s_uniforms.submitConstUniforms();

			showExampleDialog(this);

			ImGui::SetNextWindowPos(
				  ImVec2(m_viewState.m_width - m_viewState.m_width / 5.0f - 10.0f, 10.0f)
				, ImGuiCond_FirstUseEver
				);
			ImGui::SetNextWindowSize(
				  ImVec2(m_viewState.m_width / 5.0f, m_viewState.m_height - 20.0f)
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

			ImGui::Checkbox("Update lights", &FShadowRenderer::s_settings.m_updateLights);
			ImGui::Checkbox("Update scene", &FShadowRenderer::s_settings.m_updateScene);

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
				  ImVec2(m_viewState.m_width / 5.0f, 350.0f)
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

		FShadowRenderer::Get().update();

		GProcessWindow& processWindow = GProcessWindow::Get();

		m_viewState.m_width = uint16_t(processWindow._width);
		m_viewState.m_height = uint16_t(processWindow._height);

		const bgfx::Caps* caps = bgfx::getCaps();

		// Set view and projection matrices.
		const float camFovy = 60.0f;
		const float camAspect = float(int32_t(m_viewState.m_width)) / float(int32_t(m_viewState.m_height));
		const float camNear = 0.1f;
		const float camFar = 2000.0f;
		const float projHeight = bx::tan(bx::toRad(camFovy)*0.5f);
		const float projWidth = projHeight * camAspect;
		bx::mtxProj(m_viewState.m_proj, camFovy, camAspect, camNear, camFar, caps->homogeneousDepth);

		TMatrix3x4F viewMatrix = _camera->GetViewMatrix();
		TMatrix4x4F transposeViewMatrix = viewMatrix.ToMatrix4().Transpose();
		m_viewState.m_view[0] = transposeViewMatrix._m00;
		m_viewState.m_view[1] = transposeViewMatrix._m01;
		m_viewState.m_view[2] = transposeViewMatrix._m02;
		m_viewState.m_view[3] = transposeViewMatrix._m03;
		m_viewState.m_view[4] = transposeViewMatrix._m10;
		m_viewState.m_view[5] = transposeViewMatrix._m11;
		m_viewState.m_view[6] = transposeViewMatrix._m12;
		m_viewState.m_view[7] = transposeViewMatrix._m13;
		m_viewState.m_view[8] = transposeViewMatrix._m20;
		m_viewState.m_view[9] = transposeViewMatrix._m21;
		m_viewState.m_view[10] = transposeViewMatrix._m22;
		m_viewState.m_view[11] = transposeViewMatrix._m23;
		m_viewState.m_view[12] = transposeViewMatrix._m30;
		m_viewState.m_view[13] = transposeViewMatrix._m31;
		m_viewState.m_view[14] = transposeViewMatrix._m32;
		m_viewState.m_view[15] = transposeViewMatrix._m33;
		//cameraGetViewMtx(m_viewState.m_view);

		float currentShadowMapSizef = float(int16_t(FShadowRenderer::s_currentShadowMapSize));
		FShadowRenderer::s_uniforms.m_shadowMapTexelSize = 1.0f / currentShadowMapSizef;

		FShadowRenderer::s_uniforms.submitConstUniforms();

		// Update uniforms.
		FShadowRenderer::s_uniforms.m_shadowMapBias   = currentSmSettings->m_bias;
		FShadowRenderer::s_uniforms.m_shadowMapOffset = currentSmSettings->m_normalOffset;
		FShadowRenderer::s_uniforms.m_shadowMapParam0 = currentSmSettings->m_customParam0;
		FShadowRenderer::s_uniforms.m_shadowMapParam1 = currentSmSettings->m_customParam1;
		FShadowRenderer::s_uniforms.m_depthValuePow   = currentSmSettings->m_depthValuePow;
		FShadowRenderer::s_uniforms.m_XNum            = currentSmSettings->m_xNum;
		FShadowRenderer::s_uniforms.m_YNum            = currentSmSettings->m_yNum;
		FShadowRenderer::s_uniforms.m_XOffset         = currentSmSettings->m_xOffset;
		FShadowRenderer::s_uniforms.m_YOffset         = currentSmSettings->m_yOffset;
		FShadowRenderer::s_uniforms.m_showSmCoverage  = float(FShadowRenderer::s_settings.m_showSmCoverage);
		FShadowRenderer::s_uniforms.m_lightPtr = (LightType::DirectionalLight == FShadowRenderer::s_settings.m_lightType) ? &FShadowRenderer::s_directionalLight : &FShadowRenderer::s_pointLight;

		if (LightType::SpotLight == FShadowRenderer::s_settings.m_lightType)
		{
			FShadowRenderer::s_pointLight.m_attenuationSpotOuter.m_outer = FShadowRenderer::s_settings.m_spotOuterAngle;
			FShadowRenderer::s_pointLight.m_spotDirectionInner.m_inner   = FShadowRenderer::s_settings.m_spotInnerAngle;
		}
		else
		{
			FShadowRenderer::s_pointLight.m_attenuationSpotOuter.m_outer = 91.0f; //above 90.0f means point light
		}

		FShadowRenderer::s_uniforms.submitPerFrameUniforms();

		// Time.
		int64_t now = bx::getHPCounter();
		static int64_t last = now;
		const int64_t frameTime = now - last;
		last = now;
		const double freq = double(bx::getHPFrequency() );
		const float deltaTime = float(frameTime/freq);

		// Update camera.
		//cameraUpdate(deltaTime, m_mouseState);

		TMatrix3x4F viewMatrix2 = _camera->GetViewMatrix();
		TMatrix4x4F transposeViewMatrix2 = viewMatrix.ToMatrix4().Transpose();
		m_viewState.m_view[0] = transposeViewMatrix2._m00;
		m_viewState.m_view[1] = transposeViewMatrix2._m01;
		m_viewState.m_view[2] = transposeViewMatrix2._m02;
		m_viewState.m_view[3] = transposeViewMatrix2._m03;
		m_viewState.m_view[4] = transposeViewMatrix2._m10;
		m_viewState.m_view[5] = transposeViewMatrix2._m11;
		m_viewState.m_view[6] = transposeViewMatrix2._m12;
		m_viewState.m_view[7] = transposeViewMatrix2._m13;
		m_viewState.m_view[8] = transposeViewMatrix2._m20;
		m_viewState.m_view[9] = transposeViewMatrix2._m21;
		m_viewState.m_view[10] = transposeViewMatrix2._m22;
		m_viewState.m_view[11] = transposeViewMatrix2._m23;
		m_viewState.m_view[12] = transposeViewMatrix2._m30;
		m_viewState.m_view[13] = transposeViewMatrix2._m31;
		m_viewState.m_view[14] = transposeViewMatrix2._m32;
		m_viewState.m_view[15] = transposeViewMatrix2._m33;
		// Update view mtx.
		//cameraGetViewMtx(m_viewState.m_view);

		// Update lights.
		FShadowRenderer::s_pointLight.computeViewSpaceComponents(m_viewState.m_view);
		FShadowRenderer::s_directionalLight.computeViewSpaceComponents(m_viewState.m_view);

		// Update time accumulators.
		if (FShadowRenderer::s_settings.m_updateLights) { m_timeAccumulatorLight += deltaTime; }
		if (FShadowRenderer::s_settings.m_updateScene)  { m_timeAccumulatorScene += deltaTime; }

		// Setup lights.
		FShadowRenderer::s_pointLight.m_position.m_x = bx::cos(m_timeAccumulatorLight) * 20.0f;
		FShadowRenderer::s_pointLight.m_position.m_y = 26.0f;
		FShadowRenderer::s_pointLight.m_position.m_z = bx::sin(m_timeAccumulatorLight) * 20.0f;
		FShadowRenderer::s_pointLight.m_spotDirectionInner.m_x = -FShadowRenderer::s_pointLight.m_position.m_x;
		FShadowRenderer::s_pointLight.m_spotDirectionInner.m_y = -FShadowRenderer::s_pointLight.m_position.m_y;
		FShadowRenderer::s_pointLight.m_spotDirectionInner.m_z = -FShadowRenderer::s_pointLight.m_position.m_z;

		FShadowRenderer::s_directionalLight.m_position.m_x = -bx::cos(m_timeAccumulatorLight);
		FShadowRenderer::s_directionalLight.m_position.m_y = -1.0f;
		FShadowRenderer::s_directionalLight.m_position.m_z = -bx::sin(m_timeAccumulatorLight);

		// Setup instance matrices.
		float mtxFloor[16];
		const float floorScale = 550.0f;
		TMatrix4x4F& planeMatrix = _planeActor->GetTransform()->GetWorldTransform().ToMatrix4().Transpose();
		mtxFloor[0] = planeMatrix._m00;
		mtxFloor[1] = planeMatrix._m01;
		mtxFloor[2] = planeMatrix._m02;
		mtxFloor[3] = planeMatrix._m03;
		mtxFloor[4] = planeMatrix._m10;
		mtxFloor[5] = planeMatrix._m11;
		mtxFloor[6] = planeMatrix._m12;
		mtxFloor[7] = planeMatrix._m13;
		mtxFloor[8] = planeMatrix._m20;
		mtxFloor[9] = planeMatrix._m21;
		mtxFloor[10] = planeMatrix._m22;
		mtxFloor[11] = planeMatrix._m23;
		mtxFloor[12] = planeMatrix._m30;
		mtxFloor[13] = planeMatrix._m31;
		mtxFloor[14] = planeMatrix._m32;
		mtxFloor[15] = planeMatrix._m33;
		//bx::mtxSRT(mtxFloor
		//		   , floorScale //scaleX
		//		   , floorScale //scaleY
		//		   , floorScale //scaleZ
		//		   , 0.0f //rotX
		//		   , 0.0f //rotY
		//		   , 0.0f //rotZ
		//		   , 0.0f //translateX
		//		   , 0.0f //translateY
		//		   , 0.0f //translateZ
		//		   );

		float mtxCube[16];
		TMatrix4x4F& meshMatrix = _meshActor->GetTransform()->GetWorldTransform().ToMatrix4().Transpose();
		mtxCube[0] = meshMatrix._m00;
		mtxCube[1] = meshMatrix._m01;
		mtxCube[2] = meshMatrix._m02;
		mtxCube[3] = meshMatrix._m03;
		mtxCube[4] = meshMatrix._m10; 
		mtxCube[5] = meshMatrix._m11; 
		mtxCube[6] = meshMatrix._m12;
		mtxCube[7] = meshMatrix._m13;
		mtxCube[8] = meshMatrix._m20;
		mtxCube[9] = meshMatrix._m21;
		mtxCube[10] = meshMatrix._m22;
		mtxCube[11] = meshMatrix._m23;
		mtxCube[12] = meshMatrix._m30;
		mtxCube[13] = meshMatrix._m31;
		mtxCube[14] = meshMatrix._m32;
		mtxCube[15] = meshMatrix._m33;

		// Compute transform matrices.
		const uint8_t shadowMapPasses = ShadowMapRenderTargets::Count;
		float lightView[shadowMapPasses][16];
		float lightProj[shadowMapPasses][16];
		float mtxYpr[TetrahedronFaces::Count][16];

		float screenProj[16];
		float screenView[16];
		bx::mtxIdentity(screenView);

		bx::mtxOrtho(
						screenProj
						, 0.0f
						, 1.0f
						, 1.0f
						, 0.0f
						, 0.0f
						, 100.0f
						, 0.0f
						, caps->homogeneousDepth
						);

		// Update render target size.
		uint16_t shadowMapSize = 1 << uint32_t(currentSmSettings->m_sizePwrTwo);
		if (bLtChanged || FShadowRenderer::s_currentShadowMapSize != shadowMapSize)
		{
			FShadowRenderer::s_currentShadowMapSize = shadowMapSize;
			FShadowRenderer::s_uniforms.m_shadowMapTexelSize = 1.0f / currentShadowMapSizef;

			{
				bgfx::destroy(FShadowRenderer::s_rtShadowMap[0]);

				bgfx::TextureHandle fbtextures[] =
				{
					bgfx::createTexture2D(FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT),
					bgfx::createTexture2D(FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize, false, 1, bgfx::TextureFormat::D24S8, BGFX_TEXTURE_RT),
				};
				FShadowRenderer::s_rtShadowMap[0] = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);
			}

			if (LightType::DirectionalLight == FShadowRenderer::s_settings.m_lightType)
			{
				for (uint8_t ii = 1; ii < ShadowMapRenderTargets::Count; ++ii)
				{
					{
						bgfx::destroy(FShadowRenderer::s_rtShadowMap[ii]);

						bgfx::TextureHandle fbtextures[] =
						{
							bgfx::createTexture2D(FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT),
							bgfx::createTexture2D(FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize, false, 1, bgfx::TextureFormat::D24S8, BGFX_TEXTURE_RT),
						};
						FShadowRenderer::s_rtShadowMap[ii] = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);
					}
				}
			}

			bgfx::destroy(FShadowRenderer::s_rtBlur);
			FShadowRenderer::s_rtBlur = bgfx::createFrameBuffer(FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize, bgfx::TextureFormat::BGRA8);
		}

		if (LightType::SpotLight == FShadowRenderer::s_settings.m_lightType)
		{
			const float fovy = FShadowRenderer::s_settings.m_coverageSpotL;
			const float aspect = 1.0f;
			bx::mtxProj(
						lightProj[ProjType::Horizontal]
						, fovy
						, aspect
						, currentSmSettings->m_near
						, currentSmSettings->m_far
						, false
						);

			//For linear depth, prevent depth division by variable w-component in shaders and divide here by far plane
			if (DepthImpl::Linear == FShadowRenderer::s_settings.m_depthImpl)
			{
				lightProj[ProjType::Horizontal][10] /= currentSmSettings->m_far;
				lightProj[ProjType::Horizontal][14] /= currentSmSettings->m_far;
			}

			const bx::Vec3 at = bx::add(bx::load<bx::Vec3>(FShadowRenderer::s_pointLight.m_position.m_v), bx::load<bx::Vec3>(FShadowRenderer::s_pointLight.m_spotDirectionInner.m_v) );
			bx::mtxLookAt(lightView[TetrahedronFaces::Green], bx::load<bx::Vec3>(FShadowRenderer::s_pointLight.m_position.m_v), at);
		}
		else if (LightType::PointLight == FShadowRenderer::s_settings.m_lightType)
		{
			float ypr[TetrahedronFaces::Count][3] =
			{
				{ bx::toRad(  0.0f), bx::toRad( 27.36780516f), bx::toRad(0.0f) },
				{ bx::toRad(180.0f), bx::toRad( 27.36780516f), bx::toRad(0.0f) },
				{ bx::toRad(-90.0f), bx::toRad(-27.36780516f), bx::toRad(0.0f) },
				{ bx::toRad( 90.0f), bx::toRad(-27.36780516f), bx::toRad(0.0f) },
			};


			if (FShadowRenderer::s_settings.m_stencilPack)
			{
				const float fovx = 143.98570868f + 3.51f + FShadowRenderer::s_settings.m_fovXAdjust;
				const float fovy = 125.26438968f + 9.85f + FShadowRenderer::s_settings.m_fovYAdjust;
				const float aspect = bx::tan(bx::toRad(fovx*0.5f) )/bx::tan(bx::toRad(fovy*0.5f) );

				bx::mtxProj(
						lightProj[ProjType::Vertical]
							, fovx
							, aspect
							, currentSmSettings->m_near
							, currentSmSettings->m_far
							, false
							);

				//For linear depth, prevent depth division by variable w-component in shaders and divide here by far plane
				if (DepthImpl::Linear == FShadowRenderer::s_settings.m_depthImpl)
				{
					lightProj[ProjType::Vertical][10] /= currentSmSettings->m_far;
					lightProj[ProjType::Vertical][14] /= currentSmSettings->m_far;
				}

				ypr[TetrahedronFaces::Green ][2] = bx::toRad(180.0f);
				ypr[TetrahedronFaces::Yellow][2] = bx::toRad(  0.0f);
				ypr[TetrahedronFaces::Blue  ][2] = bx::toRad( 90.0f);
				ypr[TetrahedronFaces::Red   ][2] = bx::toRad(-90.0f);
			}

			const float fovx = 143.98570868f + 7.8f + FShadowRenderer::s_settings.m_fovXAdjust;
			const float fovy = 125.26438968f + 3.0f + FShadowRenderer::s_settings.m_fovYAdjust;
			const float aspect = bx::tan(bx::toRad(fovx*0.5f) )/bx::tan(bx::toRad(fovy*0.5f) );

			bx::mtxProj(
						lightProj[ProjType::Horizontal]
						, fovy
						, aspect
						, currentSmSettings->m_near
						, currentSmSettings->m_far
						, caps->homogeneousDepth
						);

			//For linear depth, prevent depth division by variable w component in shaders and divide here by far plane
			if (DepthImpl::Linear == FShadowRenderer::s_settings.m_depthImpl)
			{
				lightProj[ProjType::Horizontal][10] /= currentSmSettings->m_far;
				lightProj[ProjType::Horizontal][14] /= currentSmSettings->m_far;
			}


			for (uint8_t ii = 0; ii < TetrahedronFaces::Count; ++ii)
			{
				float mtxTmp[16];
				mtxYawPitchRoll(mtxTmp, ypr[ii][0], ypr[ii][1], ypr[ii][2]);

				float tmp[3] =
				{
					-bx::dot(bx::load<bx::Vec3>(FShadowRenderer::s_pointLight.m_position.m_v), bx::load<bx::Vec3>(&mtxTmp[0]) ),
					-bx::dot(bx::load<bx::Vec3>(FShadowRenderer::s_pointLight.m_position.m_v), bx::load<bx::Vec3>(&mtxTmp[4]) ),
					-bx::dot(bx::load<bx::Vec3>(FShadowRenderer::s_pointLight.m_position.m_v), bx::load<bx::Vec3>(&mtxTmp[8]) ),
				};

				bx::mtxTranspose(mtxYpr[ii], mtxTmp);

				bx::memCopy(lightView[ii], mtxYpr[ii], 12*sizeof(float) );
				lightView[ii][12] = tmp[0];
				lightView[ii][13] = tmp[1];
				lightView[ii][14] = tmp[2];
				lightView[ii][15] = 1.0f;
			}
		}
		else // LightType::DirectionalLight == settings.m_lightType
		{
			// Setup light view mtx.
			const bx::Vec3 at = { 0.0f, 0.0f, 0.0f };
			const bx::Vec3 eye =
			{
				-FShadowRenderer::s_directionalLight.m_position.m_x,
				-FShadowRenderer::s_directionalLight.m_position.m_y,
				-FShadowRenderer::s_directionalLight.m_position.m_z,
			};
			bx::mtxLookAt(lightView[0], eye, at);

			// Compute camera inverse view mtx.
			float mtxViewInv[16];
			bx::mtxInverse(mtxViewInv, m_viewState.m_view);

			// Compute split distances.
			const uint8_t maxNumSplits = 4;
			BX_CHECK(maxNumSplits >= settings.m_numSplits, "Error! Max num splits.");

			float splitSlices[maxNumSplits*2];
			splitFrustum(splitSlices
				, uint8_t(FShadowRenderer::s_settings.m_numSplits)
				, currentSmSettings->m_near
				, currentSmSettings->m_far
				, FShadowRenderer::s_settings.m_splitDistribution
				);

			// Update uniforms.
			for (uint8_t ii = 0, ff = 1; ii < FShadowRenderer::s_settings.m_numSplits; ++ii, ff+=2)
			{
				// This lags for 1 frame, but it's not a problem.
				FShadowRenderer::s_uniforms.m_csmFarDistances[ii] = splitSlices[ff];
			}

			float mtxProj[16];
			bx::mtxOrtho(
					mtxProj
				, 1.0f
				, -1.0f
				, 1.0f
				, -1.0f
				, -currentSmSettings->m_far
				, currentSmSettings->m_far
				, 0.0f
				, caps->homogeneousDepth
				);

			const uint8_t numCorners = 8;
			float frustumCorners[maxNumSplits][numCorners][3];
			for (uint8_t ii = 0, nn = 0, ff = 1; ii < FShadowRenderer::s_settings.m_numSplits; ++ii, nn+=2, ff+=2)
			{
				// Compute frustum corners for one split in world space.
				worldSpaceFrustumCorners( (float*)frustumCorners[ii], splitSlices[nn], splitSlices[ff], projWidth, projHeight, mtxViewInv);

				bx::Vec3 min = {  9000.0f,  9000.0f,  9000.0f };
				bx::Vec3 max = { -9000.0f, -9000.0f, -9000.0f };

				for (uint8_t jj = 0; jj < numCorners; ++jj)
				{
					// Transform to light space.
					const bx::Vec3 xyz = bx::mul(bx::load<bx::Vec3>(frustumCorners[ii][jj]), lightView[0]);

					// Update bounding box.
					min = bx::min(min, xyz);
					max = bx::max(max, xyz);
				}

				const bx::Vec3 minproj = bx::mulH(min, mtxProj);
				const bx::Vec3 maxproj = bx::mulH(max, mtxProj);

				float scalex = 2.0f / (maxproj.x - minproj.x);
				float scaley = 2.0f / (maxproj.y - minproj.y);

				if (FShadowRenderer::s_settings.m_stabilize)
				{
					const float quantizer = 64.0f;
					scalex = quantizer / bx::ceil(quantizer / scalex);
					scaley = quantizer / bx::ceil(quantizer / scaley);
				}

				float offsetx = 0.5f * (maxproj.x + minproj.x) * scalex;
				float offsety = 0.5f * (maxproj.y + minproj.y) * scaley;

				if (FShadowRenderer::s_settings.m_stabilize)
				{
					const float halfSize = currentShadowMapSizef * 0.5f;
					offsetx = bx::ceil(offsetx * halfSize) / halfSize;
					offsety = bx::ceil(offsety * halfSize) / halfSize;
				}

				float mtxCrop[16];
				bx::mtxIdentity(mtxCrop);
				mtxCrop[ 0] = scalex;
				mtxCrop[ 5] = scaley;
				mtxCrop[12] = offsetx;
				mtxCrop[13] = offsety;

				bx::mtxMul(lightProj[ii], mtxCrop, mtxProj);
			}
		}

		// Reset render targets.
		const bgfx::FrameBufferHandle invalidRt = BGFX_INVALID_HANDLE;
		for (uint8_t ii = 0; ii < RENDERVIEW_DRAWDEPTH_3_ID+1; ++ii)
		{
			bgfx::setViewFrameBuffer(ii, invalidRt);
			bgfx::setViewRect(ii, 0, 0, m_viewState.m_width, m_viewState.m_height);
		}

		// Determine on-screen rectangle size where depth buffer will be drawn.
		uint16_t depthRectHeight = uint16_t(float(m_viewState.m_height) / 2.5f);
		uint16_t depthRectWidth  = depthRectHeight;
		uint16_t depthRectX = 0;
		uint16_t depthRectY = m_viewState.m_height - depthRectHeight;

		// Setup views and render targets.
		bgfx::setViewRect(0, 0, 0, m_viewState.m_width, m_viewState.m_height);
		bgfx::setViewTransform(0, m_viewState.m_view, m_viewState.m_proj);

		if (LightType::SpotLight == FShadowRenderer::s_settings.m_lightType)
		{
			/**
				* RENDERVIEW_SHADOWMAP_0_ID - Clear shadow map. (used as convenience, otherwise render_pass_1 could be cleared)
				* RENDERVIEW_SHADOWMAP_1_ID - Craft shadow map.
				* RENDERVIEW_VBLUR_0_ID - Vertical blur.
				* RENDERVIEW_HBLUR_0_ID - Horizontal blur.
				* RENDERVIEW_DRAWSCENE_0_ID - Draw scene.
				* RENDERVIEW_DRAWSCENE_1_ID - Draw floor bottom.
				* RENDERVIEW_DRAWDEPTH_0_ID - Draw depth buffer.
				*/

			bgfx::setViewRect(RENDERVIEW_SHADOWMAP_0_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_SHADOWMAP_1_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_VBLUR_0_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_HBLUR_0_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_DRAWSCENE_0_ID, 0, 0, m_viewState.m_width, m_viewState.m_height);
			bgfx::setViewRect(RENDERVIEW_DRAWSCENE_1_ID, 0, 0, m_viewState.m_width, m_viewState.m_height);
			bgfx::setViewRect(RENDERVIEW_DRAWDEPTH_0_ID, depthRectX, depthRectY, depthRectWidth, depthRectHeight);

			bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_0_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_1_ID, lightView[0], lightProj[ProjType::Horizontal]);
			bgfx::setViewTransform(RENDERVIEW_VBLUR_0_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_HBLUR_0_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_DRAWSCENE_0_ID, m_viewState.m_view, m_viewState.m_proj);
			bgfx::setViewTransform(RENDERVIEW_DRAWSCENE_1_ID, m_viewState.m_view, m_viewState.m_proj);
			bgfx::setViewTransform(RENDERVIEW_DRAWDEPTH_0_ID, screenView, screenProj);

			bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_0_ID, FShadowRenderer::s_rtShadowMap[0]);
			bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_1_ID, FShadowRenderer::s_rtShadowMap[0]);
			bgfx::setViewFrameBuffer(RENDERVIEW_VBLUR_0_ID, FShadowRenderer::s_rtBlur);
			bgfx::setViewFrameBuffer(RENDERVIEW_HBLUR_0_ID, FShadowRenderer::s_rtShadowMap[0]);
		}
		else if (LightType::PointLight == FShadowRenderer::s_settings.m_lightType)
		{
			/**
				* RENDERVIEW_SHADOWMAP_0_ID - Clear entire shadow map.
				* RENDERVIEW_SHADOWMAP_1_ID - Craft green tetrahedron shadow face.
				* RENDERVIEW_SHADOWMAP_2_ID - Craft yellow tetrahedron shadow face.
				* RENDERVIEW_SHADOWMAP_3_ID - Craft blue tetrahedron shadow face.
				* RENDERVIEW_SHADOWMAP_4_ID - Craft red tetrahedron shadow face.
				* RENDERVIEW_VBLUR_0_ID - Vertical blur.
				* RENDERVIEW_HBLUR_0_ID - Horizontal blur.
				* RENDERVIEW_DRAWSCENE_0_ID - Draw scene.
				* RENDERVIEW_DRAWSCENE_1_ID - Draw floor bottom.
				* RENDERVIEW_DRAWDEPTH_0_ID - Draw depth buffer.
				*/

			bgfx::setViewRect(RENDERVIEW_SHADOWMAP_0_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			if (FShadowRenderer::s_settings.m_stencilPack)
			{
				const uint16_t f = FShadowRenderer::s_currentShadowMapSize;   //full size
				const uint16_t h = FShadowRenderer::s_currentShadowMapSize/2; //half size
				bgfx::setViewRect(RENDERVIEW_SHADOWMAP_1_ID, 0, 0, f, h);
				bgfx::setViewRect(RENDERVIEW_SHADOWMAP_2_ID, 0, h, f, h);
				bgfx::setViewRect(RENDERVIEW_SHADOWMAP_3_ID, 0, 0, h, f);
				bgfx::setViewRect(RENDERVIEW_SHADOWMAP_4_ID, h, 0, h, f);
			}
			else
			{
				const uint16_t h = FShadowRenderer::s_currentShadowMapSize/2; //half size
				bgfx::setViewRect(RENDERVIEW_SHADOWMAP_1_ID, 0, 0, h, h);
				bgfx::setViewRect(RENDERVIEW_SHADOWMAP_2_ID, h, 0, h, h);
				bgfx::setViewRect(RENDERVIEW_SHADOWMAP_3_ID, 0, h, h, h);
				bgfx::setViewRect(RENDERVIEW_SHADOWMAP_4_ID, h, h, h, h);
			}
			bgfx::setViewRect(RENDERVIEW_VBLUR_0_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_HBLUR_0_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_DRAWSCENE_0_ID, 0, 0, m_viewState.m_width, m_viewState.m_height);
			bgfx::setViewRect(RENDERVIEW_DRAWSCENE_1_ID, 0, 0, m_viewState.m_width, m_viewState.m_height);
			bgfx::setViewRect(RENDERVIEW_DRAWDEPTH_0_ID, depthRectX, depthRectY, depthRectWidth, depthRectHeight);

			bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_0_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_1_ID, lightView[TetrahedronFaces::Green],  lightProj[ProjType::Horizontal]);
			bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_2_ID, lightView[TetrahedronFaces::Yellow], lightProj[ProjType::Horizontal]);
			if(FShadowRenderer::s_settings.m_stencilPack)
			{
				bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_3_ID, lightView[TetrahedronFaces::Blue], lightProj[ProjType::Vertical]);
				bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_4_ID, lightView[TetrahedronFaces::Red],  lightProj[ProjType::Vertical]);
			}
			else
			{
				bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_3_ID, lightView[TetrahedronFaces::Blue], lightProj[ProjType::Horizontal]);
				bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_4_ID, lightView[TetrahedronFaces::Red],  lightProj[ProjType::Horizontal]);
			}
			bgfx::setViewTransform(RENDERVIEW_VBLUR_0_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_HBLUR_0_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_DRAWSCENE_0_ID, m_viewState.m_view, m_viewState.m_proj);
			bgfx::setViewTransform(RENDERVIEW_DRAWSCENE_1_ID, m_viewState.m_view, m_viewState.m_proj);
			bgfx::setViewTransform(RENDERVIEW_DRAWDEPTH_0_ID, screenView, screenProj);

			bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_0_ID, FShadowRenderer::s_rtShadowMap[0]);
			bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_1_ID, FShadowRenderer::s_rtShadowMap[0]);
			bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_2_ID, FShadowRenderer::s_rtShadowMap[0]);
			bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_3_ID, FShadowRenderer::s_rtShadowMap[0]);
			bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_4_ID, FShadowRenderer::s_rtShadowMap[0]);
			bgfx::setViewFrameBuffer(RENDERVIEW_VBLUR_0_ID, FShadowRenderer::s_rtBlur);
			bgfx::setViewFrameBuffer(RENDERVIEW_HBLUR_0_ID, FShadowRenderer::s_rtShadowMap[0]);
		}
		else // LightType::DirectionalLight == settings.m_lightType
		{
			/**
				* RENDERVIEW_SHADOWMAP_1_ID - Craft shadow map for first  split.
				* RENDERVIEW_SHADOWMAP_2_ID - Craft shadow map for second split.
				* RENDERVIEW_SHADOWMAP_3_ID - Craft shadow map for third  split.
				* RENDERVIEW_SHADOWMAP_4_ID - Craft shadow map for fourth split.
				* RENDERVIEW_VBLUR_0_ID - Vertical   blur for first  split.
				* RENDERVIEW_HBLUR_0_ID - Horizontal blur for first  split.
				* RENDERVIEW_VBLUR_1_ID - Vertical   blur for second split.
				* RENDERVIEW_HBLUR_1_ID - Horizontal blur for second split.
				* RENDERVIEW_VBLUR_2_ID - Vertical   blur for third  split.
				* RENDERVIEW_HBLUR_2_ID - Horizontal blur for third  split.
				* RENDERVIEW_VBLUR_3_ID - Vertical   blur for fourth split.
				* RENDERVIEW_HBLUR_3_ID - Horizontal blur for fourth split.
				* RENDERVIEW_DRAWSCENE_0_ID - Draw scene.
				* RENDERVIEW_DRAWSCENE_1_ID - Draw floor bottom.
				* RENDERVIEW_DRAWDEPTH_0_ID - Draw depth buffer for first  split.
				* RENDERVIEW_DRAWDEPTH_1_ID - Draw depth buffer for second split.
				* RENDERVIEW_DRAWDEPTH_2_ID - Draw depth buffer for third  split.
				* RENDERVIEW_DRAWDEPTH_3_ID - Draw depth buffer for fourth split.
				*/

			depthRectHeight = m_viewState.m_height / 3;
			depthRectWidth  = depthRectHeight;
			depthRectX = 0;
			depthRectY = m_viewState.m_height - depthRectHeight;

			bgfx::setViewRect(RENDERVIEW_SHADOWMAP_1_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_SHADOWMAP_2_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_SHADOWMAP_3_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_SHADOWMAP_4_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_VBLUR_0_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_HBLUR_0_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_VBLUR_1_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_HBLUR_1_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_VBLUR_2_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_HBLUR_2_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_VBLUR_3_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_HBLUR_3_ID, 0, 0, FShadowRenderer::s_currentShadowMapSize, FShadowRenderer::s_currentShadowMapSize);
			bgfx::setViewRect(RENDERVIEW_DRAWSCENE_0_ID, 0, 0, m_viewState.m_width, m_viewState.m_height);
			bgfx::setViewRect(RENDERVIEW_DRAWSCENE_1_ID, 0, 0, m_viewState.m_width, m_viewState.m_height);
			bgfx::setViewRect(RENDERVIEW_DRAWDEPTH_0_ID, depthRectX+(0*depthRectWidth), depthRectY, depthRectWidth, depthRectHeight);
			bgfx::setViewRect(RENDERVIEW_DRAWDEPTH_1_ID, depthRectX+(1*depthRectWidth), depthRectY, depthRectWidth, depthRectHeight);
			bgfx::setViewRect(RENDERVIEW_DRAWDEPTH_2_ID, depthRectX+(2*depthRectWidth), depthRectY, depthRectWidth, depthRectHeight);
			bgfx::setViewRect(RENDERVIEW_DRAWDEPTH_3_ID, depthRectX+(3*depthRectWidth), depthRectY, depthRectWidth, depthRectHeight);

			bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_1_ID, lightView[0], lightProj[0]);
			bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_2_ID, lightView[0], lightProj[1]);
			bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_3_ID, lightView[0], lightProj[2]);
			bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_4_ID, lightView[0], lightProj[3]);
			bgfx::setViewTransform(RENDERVIEW_VBLUR_0_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_HBLUR_0_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_VBLUR_1_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_HBLUR_1_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_VBLUR_2_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_HBLUR_2_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_VBLUR_3_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_HBLUR_3_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_DRAWSCENE_0_ID, m_viewState.m_view, m_viewState.m_proj);
			bgfx::setViewTransform(RENDERVIEW_DRAWSCENE_1_ID, m_viewState.m_view, m_viewState.m_proj);
			bgfx::setViewTransform(RENDERVIEW_DRAWDEPTH_0_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_DRAWDEPTH_1_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_DRAWDEPTH_2_ID, screenView, screenProj);
			bgfx::setViewTransform(RENDERVIEW_DRAWDEPTH_3_ID, screenView, screenProj);

			bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_1_ID, FShadowRenderer::s_rtShadowMap[0]);
			bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_2_ID, FShadowRenderer::s_rtShadowMap[1]);
			bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_3_ID, FShadowRenderer::s_rtShadowMap[2]);
			bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_4_ID, FShadowRenderer::s_rtShadowMap[3]);
			bgfx::setViewFrameBuffer(RENDERVIEW_VBLUR_0_ID, FShadowRenderer::s_rtBlur);         //vblur
			bgfx::setViewFrameBuffer(RENDERVIEW_HBLUR_0_ID, FShadowRenderer::s_rtShadowMap[0]); //hblur
			bgfx::setViewFrameBuffer(RENDERVIEW_VBLUR_1_ID, FShadowRenderer::s_rtBlur);         //vblur
			bgfx::setViewFrameBuffer(RENDERVIEW_HBLUR_1_ID, FShadowRenderer::s_rtShadowMap[1]); //hblur
			bgfx::setViewFrameBuffer(RENDERVIEW_VBLUR_2_ID, FShadowRenderer::s_rtBlur);         //vblur
			bgfx::setViewFrameBuffer(RENDERVIEW_HBLUR_2_ID, FShadowRenderer::s_rtShadowMap[2]); //hblur
			bgfx::setViewFrameBuffer(RENDERVIEW_VBLUR_3_ID, FShadowRenderer::s_rtBlur);         //vblur
			bgfx::setViewFrameBuffer(RENDERVIEW_HBLUR_3_ID, FShadowRenderer::s_rtShadowMap[3]); //hblur
		}

		// Clear backbuffer at beginning.
		bgfx::setViewClear(0
							, BGFX_CLEAR_COLOR
							| BGFX_CLEAR_DEPTH
							, m_clearValues.m_clearRgba
							, m_clearValues.m_clearDepth
							, m_clearValues.m_clearStencil
							);
		bgfx::touch(0);

		// Clear shadowmap rendertarget at beginning.
		const uint8_t flags0 = (LightType::DirectionalLight == FShadowRenderer::s_settings.m_lightType)
		? 0
		: BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH | BGFX_CLEAR_STENCIL
		;

		bgfx::setViewClear(RENDERVIEW_SHADOWMAP_0_ID
							, flags0
							, 0xfefefefe //blur fails on completely white regions
							, m_clearValues.m_clearDepth
							, m_clearValues.m_clearStencil
							);
		bgfx::touch(RENDERVIEW_SHADOWMAP_0_ID);

		const uint8_t flags1 = (LightType::DirectionalLight == FShadowRenderer::s_settings.m_lightType)
		? BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		: 0
		;

		for (uint8_t ii = 0; ii < 4; ++ii)
		{
			bgfx::setViewClear(RENDERVIEW_SHADOWMAP_1_ID+ii
								, flags1
								, 0xfefefefe //blur fails on completely white regions
								, m_clearValues.m_clearDepth
								, m_clearValues.m_clearStencil
								);
			bgfx::touch(RENDERVIEW_SHADOWMAP_1_ID+ii);
		}

		// Render.

		// Craft shadow map.
		{
			// Craft stencil mask for point light shadow map packing.
			if(LightType::PointLight == FShadowRenderer::s_settings.m_lightType && FShadowRenderer::s_settings.m_stencilPack)
			{
				if (6 == bgfx::getAvailTransientVertexBuffer(6, m_posLayout) )
				{
					struct Pos
					{
						float m_x, m_y, m_z;
					};

					bgfx::TransientVertexBuffer vb;
					bgfx::allocTransientVertexBuffer(&vb, 6, m_posLayout);
					Pos* vertex = (Pos*)vb.data;

					const float min = 0.0f;
					const float max = 1.0f;
					const float center = 0.5f;
					const float zz = 0.0f;

					vertex[0].m_x = min;
					vertex[0].m_y = min;
					vertex[0].m_z = zz;

					vertex[1].m_x = max;
					vertex[1].m_y = min;
					vertex[1].m_z = zz;

					vertex[2].m_x = center;
					vertex[2].m_y = center;
					vertex[2].m_z = zz;

					vertex[3].m_x = center;
					vertex[3].m_y = center;
					vertex[3].m_z = zz;

					vertex[4].m_x = max;
					vertex[4].m_y = max;
					vertex[4].m_z = zz;

					vertex[5].m_x = min;
					vertex[5].m_y = max;
					vertex[5].m_z = zz;

					bgfx::setState(0);
					bgfx::setStencil(BGFX_STENCIL_TEST_ALWAYS
										| BGFX_STENCIL_FUNC_REF(1)
										| BGFX_STENCIL_FUNC_RMASK(0xff)
										| BGFX_STENCIL_OP_FAIL_S_REPLACE
										| BGFX_STENCIL_OP_FAIL_Z_REPLACE
										| BGFX_STENCIL_OP_PASS_Z_REPLACE
										);
					bgfx::setVertexBuffer(0, &vb);
					bgfx::submit(RENDERVIEW_SHADOWMAP_0_ID, FShadowRenderer::s_programs.m_black);
				}
			}

			// Draw scene into shadowmap.
			uint8_t drawNum;
			if (LightType::SpotLight == FShadowRenderer::s_settings.m_lightType)
			{
				drawNum = 1;
			}
			else if (LightType::PointLight == FShadowRenderer::s_settings.m_lightType)
			{
				drawNum = 4;
			}
			else //LightType::DirectionalLight == settings.m_lightType)
			{
				drawNum = uint8_t(FShadowRenderer::s_settings.m_numSplits);
			}

			for (uint8_t ii = 0; ii < drawNum; ++ii)
			{
				const uint8_t viewId = RENDERVIEW_SHADOWMAP_1_ID + ii;

				uint8_t renderStateIndex = RenderState::ShadowMap_PackDepth;
				if(LightType::PointLight == FShadowRenderer::s_settings.m_lightType && FShadowRenderer::s_settings.m_stencilPack)
				{
					renderStateIndex = uint8_t( (ii < 2) ? RenderState::ShadowMap_PackDepthHoriz : RenderState::ShadowMap_PackDepthVert);
				}

				// Floor.
				m_hplaneMesh.submit(viewId
									, mtxFloor
									, *currentSmSettings->m_progPack
									, s_renderStates[renderStateIndex]
									);

				//// Bunny.
				//m_bunnyMesh.submit(viewId
				//				   , mtxBunny
				//				   , *currentSmSettings->m_progPack
				//				   , s_renderStates[renderStateIndex]
				//				   );

				//// Hollow cube.
				//m_hollowcubeMesh.submit(viewId
				//						, mtxHollowcube
				//						, *currentSmSettings->m_progPack
				//						, s_renderStates[renderStateIndex]
				//						);

				// Cube.
				m_cubeMesh.submit(viewId
									, mtxCube
									, *currentSmSettings->m_progPack
									, s_renderStates[renderStateIndex]
									);

				//// Trees.
				//for (uint8_t jj = 0; jj < numTrees; ++jj)
				//{
				//	m_treeMesh.submit(viewId
				//					  , mtxTrees[jj]
				//					  , *currentSmSettings->m_progPack
				//					  , s_renderStates[renderStateIndex]
				//					  );
				//}
			}
		}

		PackDepth::Enum depthType = (SmImpl::VSM == FShadowRenderer::s_settings.m_smImpl) ? PackDepth::VSM : PackDepth::RGBA;
		bool bVsmOrEsm = (SmImpl::VSM == FShadowRenderer::s_settings.m_smImpl) || (SmImpl::ESM == FShadowRenderer::s_settings.m_smImpl);

		// Blur shadow map.
		if (bVsmOrEsm
			&&  currentSmSettings->m_doBlur)
		{
			bgfx::setTexture(4, FShadowRenderer::s_shadowMap[0], bgfx::getTexture(FShadowRenderer::s_rtShadowMap[0]) );
			bgfx::setState(BGFX_STATE_WRITE_RGB|BGFX_STATE_WRITE_A);
			screenSpaceQuad(currentShadowMapSizef, currentShadowMapSizef, FShadowRenderer::s_flipV);
			bgfx::submit(RENDERVIEW_VBLUR_0_ID, FShadowRenderer::s_programs.m_vBlur[depthType]);

			bgfx::setTexture(4, FShadowRenderer::s_shadowMap[0], bgfx::getTexture(FShadowRenderer::s_rtBlur) );
			bgfx::setState(BGFX_STATE_WRITE_RGB|BGFX_STATE_WRITE_A);
			screenSpaceQuad(currentShadowMapSizef, currentShadowMapSizef, FShadowRenderer::s_flipV);
			bgfx::submit(RENDERVIEW_HBLUR_0_ID, FShadowRenderer::s_programs.m_hBlur[depthType]);

			if (LightType::DirectionalLight == FShadowRenderer::s_settings.m_lightType)
			{
				for (uint8_t ii = 1, jj = 2; ii < FShadowRenderer::s_settings.m_numSplits; ++ii, jj+=2)
				{
					const uint8_t viewId = RENDERVIEW_VBLUR_0_ID + jj;

					bgfx::setTexture(4, FShadowRenderer::s_shadowMap[0], bgfx::getTexture(FShadowRenderer::s_rtShadowMap[ii]) );
					bgfx::setState(BGFX_STATE_WRITE_RGB|BGFX_STATE_WRITE_A);
					screenSpaceQuad(currentShadowMapSizef, currentShadowMapSizef, FShadowRenderer::s_flipV);
					bgfx::submit(viewId, FShadowRenderer::s_programs.m_vBlur[depthType]);

					bgfx::setTexture(4, FShadowRenderer::s_shadowMap[0], bgfx::getTexture(FShadowRenderer::s_rtBlur) );
					bgfx::setState(BGFX_STATE_WRITE_RGB|BGFX_STATE_WRITE_A);
					screenSpaceQuad(currentShadowMapSizef, currentShadowMapSizef, FShadowRenderer::s_flipV);
					bgfx::submit(viewId+1, FShadowRenderer::s_programs.m_hBlur[depthType]);
				}
			}
		}

		// Draw scene.
		{
			// Setup shadow mtx.
			float mtxShadow[16];

			const float ymul = (FShadowRenderer::s_flipV) ? 0.5f : -0.5f;
			float zadd = (DepthImpl::Linear == FShadowRenderer::s_settings.m_depthImpl) ? 0.0f : 0.5f;

			const float mtxBias[16] =
			{
				0.5f, 0.0f, 0.0f, 0.0f,
				0.0f, ymul, 0.0f, 0.0f,
				0.0f, 0.0f, 0.5f, 0.0f,
				0.5f, 0.5f, zadd, 1.0f,
			};

			if (LightType::SpotLight == FShadowRenderer::s_settings.m_lightType)
			{
				float mtxTmp[16];
				bx::mtxMul(mtxTmp, lightProj[ProjType::Horizontal], mtxBias);
				bx::mtxMul(mtxShadow, lightView[0], mtxTmp); //lightViewProjBias
			}
			else if (LightType::PointLight == FShadowRenderer::s_settings.m_lightType)
			{
				const float s = (FShadowRenderer::s_flipV) ? 1.0f : -1.0f; //sign
				zadd = (DepthImpl::Linear == FShadowRenderer::s_settings.m_depthImpl) ? 0.0f : 0.5f;

				const float mtxCropBias[2][TetrahedronFaces::Count][16] =
				{
					{ // settings.m_stencilPack == false

						{ // D3D: Green, OGL: Blue
							0.25f,    0.0f, 0.0f, 0.0f,
							0.0f, s*0.25f, 0.0f, 0.0f,
							0.0f,    0.0f, 0.5f, 0.0f,
							0.25f,   0.25f, zadd, 1.0f,
						},
						{ // D3D: Yellow, OGL: Red
							0.25f,    0.0f, 0.0f, 0.0f,
							0.0f, s*0.25f, 0.0f, 0.0f,
							0.0f,    0.0f, 0.5f, 0.0f,
							0.75f,   0.25f, zadd, 1.0f,
						},
						{ // D3D: Blue, OGL: Green
							0.25f,    0.0f, 0.0f, 0.0f,
							0.0f, s*0.25f, 0.0f, 0.0f,
							0.0f,    0.0f, 0.5f, 0.0f,
							0.25f,   0.75f, zadd, 1.0f,
						},
						{ // D3D: Red, OGL: Yellow
							0.25f,    0.0f, 0.0f, 0.0f,
							0.0f, s*0.25f, 0.0f, 0.0f,
							0.0f,    0.0f, 0.5f, 0.0f,
							0.75f,   0.75f, zadd, 1.0f,
						},
					},
					{ // settings.m_stencilPack == true

						{ // D3D: Red, OGL: Blue
							0.25f,   0.0f, 0.0f, 0.0f,
							0.0f, s*0.5f, 0.0f, 0.0f,
							0.0f,   0.0f, 0.5f, 0.0f,
							0.25f,   0.5f, zadd, 1.0f,
						},
						{ // D3D: Blue, OGL: Red
							0.25f,   0.0f, 0.0f, 0.0f,
							0.0f, s*0.5f, 0.0f, 0.0f,
							0.0f,   0.0f, 0.5f, 0.0f,
							0.75f,   0.5f, zadd, 1.0f,
						},
						{ // D3D: Green, OGL: Green
							0.5f,    0.0f, 0.0f, 0.0f,
							0.0f, s*0.25f, 0.0f, 0.0f,
							0.0f,    0.0f, 0.5f, 0.0f,
							0.5f,   0.75f, zadd, 1.0f,
						},
						{ // D3D: Yellow, OGL: Yellow
							0.5f,    0.0f, 0.0f, 0.0f,
							0.0f, s*0.25f, 0.0f, 0.0f,
							0.0f,    0.0f, 0.5f, 0.0f,
							0.5f,   0.25f, zadd, 1.0f,
						},
					}
				};

				//Use as: [stencilPack][flipV][tetrahedronFace]
				static const uint8_t cropBiasIndices[2][2][4] =
				{
					{ // settings.m_stencilPack == false
						{ 0, 1, 2, 3 }, //flipV == false
						{ 2, 3, 0, 1 }, //flipV == true
					},
					{ // settings.m_stencilPack == true
						{ 3, 2, 0, 1 }, //flipV == false
						{ 2, 3, 0, 1 }, //flipV == true
					},
				};

				for (uint8_t ii = 0; ii < TetrahedronFaces::Count; ++ii)
				{
					ProjType::Enum projType = (FShadowRenderer::s_settings.m_stencilPack) ? ProjType::Enum(ii>1) : ProjType::Horizontal;
					uint8_t biasIndex = cropBiasIndices[FShadowRenderer::s_settings.m_stencilPack][uint8_t(FShadowRenderer::s_flipV)][ii];

					float mtxTmp[16];
					bx::mtxMul(mtxTmp, mtxYpr[ii], lightProj[projType]);
					bx::mtxMul(FShadowRenderer::s_shadowMapMtx[ii], mtxTmp, mtxCropBias[FShadowRenderer::s_settings.m_stencilPack][biasIndex]); //mtxYprProjBias
				}

				bx::mtxTranslate(mtxShadow //lightInvTranslate
									, -FShadowRenderer::s_pointLight.m_position.m_v[0]
									, -FShadowRenderer::s_pointLight.m_position.m_v[1]
									, -FShadowRenderer::s_pointLight.m_position.m_v[2]
									);
			}
			else //LightType::DirectionalLight == settings.m_lightType
			{
				for (uint8_t ii = 0; ii < FShadowRenderer::s_settings.m_numSplits; ++ii)
				{
					float mtxTmp[16];

					bx::mtxMul(mtxTmp, lightProj[ii], mtxBias);
					bx::mtxMul(FShadowRenderer::s_shadowMapMtx[ii], lightView[0], mtxTmp); //lViewProjCropBias
				}
			}

			// Floor.
			if (LightType::DirectionalLight != FShadowRenderer::s_settings.m_lightType)
			{
				bx::mtxMul(FShadowRenderer::s_lightMtx, mtxFloor, mtxShadow); //not needed for directional light
			}
			m_hplaneMesh.submit(RENDERVIEW_DRAWSCENE_0_ID
								, mtxFloor
								, *currentSmSettings->m_progDraw
								, s_renderStates[RenderState::Default]
								, true
								);

			//// Bunny.
			//if (LightType::DirectionalLight != FShadowRenderer::s_settings.m_lightType)
			//{
			//	bx::mtxMul(FShadowRenderer::s_lightMtx, mtxBunny, mtxShadow);
			//}
			//m_bunnyMesh.submit(RENDERVIEW_DRAWSCENE_0_ID
			//				   , mtxBunny
			//				   , *currentSmSettings->m_progDraw
			//				   , s_renderStates[RenderState::Default]
			//				   , true
			//				   );

			//// Hollow cube.
			//if (LightType::DirectionalLight != FShadowRenderer::s_settings.m_lightType)
			//{
			//	bx::mtxMul(FShadowRenderer::s_lightMtx, mtxHollowcube, mtxShadow);
			//}
			//m_hollowcubeMesh.submit(RENDERVIEW_DRAWSCENE_0_ID
			//						, mtxHollowcube
			//						, *currentSmSettings->m_progDraw
			//						, s_renderStates[RenderState::Default]
			//						, true
			//						);

			// Cube.
			if (LightType::DirectionalLight != FShadowRenderer::s_settings.m_lightType)
			{
				bx::mtxMul(FShadowRenderer::s_lightMtx, mtxCube, mtxShadow);
			}
			m_cubeMesh.submit(RENDERVIEW_DRAWSCENE_0_ID
								, mtxCube
								, *currentSmSettings->m_progDraw
								, s_renderStates[RenderState::Default]
								, true
								);

			//// Trees.
			//for (uint8_t ii = 0; ii < numTrees; ++ii)
			//{
			//	if (LightType::DirectionalLight != FShadowRenderer::s_settings.m_lightType)
			//	{
			//		bx::mtxMul(FShadowRenderer::s_lightMtx, mtxTrees[ii], mtxShadow);
			//	}
			//	m_treeMesh.submit(RENDERVIEW_DRAWSCENE_0_ID
			//					  , mtxTrees[ii]
			//					  , *currentSmSettings->m_progDraw
			//					  , s_renderStates[RenderState::Default]
			//					  , true
			//					  );
			//}

			// Lights.
			if (LightType::SpotLight == FShadowRenderer::s_settings.m_lightType || LightType::PointLight == FShadowRenderer::s_settings.m_lightType)
			{
				const float lightScale[3] = { 1.5f, 1.5f, 1.5f };
				float mtx[16];
				mtxBillboard(mtx, m_viewState.m_view, FShadowRenderer::s_pointLight.m_position.m_v, lightScale);
				m_vplaneMesh.submit(RENDERVIEW_DRAWSCENE_0_ID
									, mtx
									, FShadowRenderer::s_programs.m_colorTexture
									, s_renderStates[RenderState::Custom_BlendLightTexture]
									, m_texFlare
									);
			}

			// Draw floor bottom.
			float floorBottomMtx[16];
			bx::mtxSRT(floorBottomMtx
						, floorScale //scaleX
						, floorScale //scaleY
						, floorScale //scaleZ
						, 0.0f  //rotX
						, 0.0f  //rotY
						, 0.0f  //rotZ
						, 0.0f  //translateX
						, -0.1f //translateY
						, 0.0f  //translateZ
						);

			m_hplaneMesh.submit(RENDERVIEW_DRAWSCENE_1_ID
								, floorBottomMtx
								, FShadowRenderer::s_programs.m_texture
								, s_renderStates[RenderState::Custom_DrawPlaneBottom]
								, m_texFigure
								);
		}

		// Draw depth rect.
		if (FShadowRenderer::s_settings.m_drawDepthBuffer)
		{
			bgfx::setTexture(4, FShadowRenderer::s_shadowMap[0], bgfx::getTexture(FShadowRenderer::s_rtShadowMap[0]) );
			bgfx::setState(BGFX_STATE_WRITE_RGB|BGFX_STATE_WRITE_A);
			screenSpaceQuad(currentShadowMapSizef, currentShadowMapSizef, FShadowRenderer::s_flipV);
			bgfx::submit(RENDERVIEW_DRAWDEPTH_0_ID, FShadowRenderer::s_programs.m_drawDepth[depthType]);

			if (LightType::DirectionalLight == FShadowRenderer::s_settings.m_lightType)
			{
				for (uint8_t ii = 1; ii < FShadowRenderer::s_settings.m_numSplits; ++ii)
				{
					bgfx::setTexture(4, FShadowRenderer::s_shadowMap[0], bgfx::getTexture(FShadowRenderer::s_rtShadowMap[ii]) );
					bgfx::setState(BGFX_STATE_WRITE_RGB|BGFX_STATE_WRITE_A);
					screenSpaceQuad(currentShadowMapSizef, currentShadowMapSizef, FShadowRenderer::s_flipV);
					bgfx::submit(RENDERVIEW_DRAWDEPTH_0_ID+ii, FShadowRenderer::s_programs.m_drawDepth[depthType]);
				}
			}
		}


		// Advance to next frame. Rendering thread will be kicked to
		// process submitted rendering primitives.
		bgfx::frame();

		return true;
	}

	ViewState m_viewState;
	ClearValues m_clearValues;

	bgfx::VertexLayout m_posLayout;

	bgfx::TextureHandle m_texFigure;
	bgfx::TextureHandle m_texFlare;
	bgfx::TextureHandle m_texFieldstone;

	Mesh m_bunnyMesh;
	Mesh m_treeMesh;
	Mesh m_cubeMesh;
	Mesh m_hollowcubeMesh;
	Mesh m_hplaneMesh;
	Mesh m_vplaneMesh;

	//float FShadowRenderer::s_color[4];

	//float FShadowRenderer::s_lightMtx[16];
	//float FShadowRenderer::s_shadowMapMtx[ShadowMapRenderTargets::Count][16];

	float m_timeAccumulatorLight;
	float m_timeAccumulatorScene;

	ACameraComponent* _camera;

	AActor* _meshActor;
	AMeshComponent* _meshComponent;

	AActor* _planeActor;
	AMeshComponent* _planeComponent;
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
