#include "Renderer/HDRPipline.h"
namespace Auto3D
{
	void sSubmitTemp(FGeometry* geometry, bgfx::ViewId id, bgfx::ProgramHandle program, const float* mtx, uint64_t state)
	{
		if (BGFX_STATE_MASK == state)
		{
			state = 0
				| BGFX_STATE_WRITE_RGB
				| BGFX_STATE_WRITE_A
				| BGFX_STATE_WRITE_Z
				| BGFX_STATE_DEPTH_TEST_LESS
				| BGFX_STATE_CULL_CCW
				| BGFX_STATE_MSAA
				;
		}

		bgfx::setTransform(mtx);
		bgfx::setState(state);

		TVector<TPair<FMeshGroup*, bgfx::OcclusionQueryHandle>>& geometryValue = geometry->_geometryValue;

		for (auto it = geometryValue.Begin(), itEnd = geometryValue.End(); it != itEnd; ++it)
		{
			FMeshGroup* group = it->_first;
			//bgfx::OcclusionQueryHandle& occlusionQuery = it->_second;

			bgfx::setIndexBuffer(group->_ibh);
			bgfx::setVertexBuffer(0, group->_vbh);
			bgfx::submit(id, program, 0, it != itEnd - 1);
		}
	}

FHDRPipline::FHDRPipline()
{

}
FHDRPipline::~FHDRPipline()
{

}

}