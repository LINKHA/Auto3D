#include "Renderer//RendererDef.h"
#include <bgfx/bgfx.h>

namespace Auto3D
{

FRenderState FRenderState::_renderState[FRenderState::Count] =
{
	{ // Default
	0
	| BGFX_STATE_WRITE_RGB
	| BGFX_STATE_WRITE_A
	| BGFX_STATE_DEPTH_TEST_LEQUAL
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
	| BGFX_STATE_DEPTH_TEST_LEQUAL
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
	| BGFX_STATE_DEPTH_TEST_LEQUAL
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
	| BGFX_STATE_DEPTH_TEST_LEQUAL
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
	| BGFX_STATE_DEPTH_TEST_LEQUAL
	| BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_COLOR, BGFX_STATE_BLEND_INV_SRC_COLOR)
	| BGFX_STATE_CULL_CCW
	| BGFX_STATE_MSAA
	, UINT32_MAX
	, BGFX_STENCIL_NONE
	, BGFX_STENCIL_NONE
},
{// Occlusion
	BGFX_STATE_DEPTH_TEST_LEQUAL
	| BGFX_STATE_CULL_CCW
	, UINT32_MAX
	, BGFX_STENCIL_NONE
	, BGFX_STENCIL_NONE
},
};

bgfx::VertexLayout PosColorTexCoord0Vertex::ms_layout;
float FRendererDef::s_texelHalf = 0.0f;
bool FRendererDef::s_flipV = false;

void screenSpaceQuad(float _textureWidth, float _textureHeight, bool _originBottomLeft, float _width, float _height)
{
	if (3 == bgfx::getAvailTransientVertexBuffer(3, PosColorTexCoord0Vertex::ms_layout))
	{
		bgfx::TransientVertexBuffer vb;
		bgfx::allocTransientVertexBuffer(&vb, 3, PosColorTexCoord0Vertex::ms_layout);
		PosColorTexCoord0Vertex* vertex = (PosColorTexCoord0Vertex*)vb.data;

		const float zz = 0.0f;

		const float minx = -_width;
		const float maxx = _width;
		const float miny = 0.0f;
		const float maxy = _height * 2.0f;

		const float texelHalfW = FRendererDef::s_texelHalf / _textureWidth;
		const float texelHalfH = FRendererDef::s_texelHalf / _textureHeight;
		const float minu = -1.0f + texelHalfW;
		const float maxu = 1.0f + texelHalfW;

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
}