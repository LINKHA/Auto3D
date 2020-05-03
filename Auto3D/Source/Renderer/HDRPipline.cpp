#include "Renderer/HDRPipline.h"
namespace Auto3D
{
	void setOffsets2x2Lum(bgfx::UniformHandle _handle, uint32_t _width, uint32_t _height)
	{
		float offsets[16][4];

		float du = 1.0f / _width;
		float dv = 1.0f / _height;

		uint16_t num = 0;
		for (uint32_t yy = 0; yy < 3; ++yy)
		{
			for (uint32_t xx = 0; xx < 3; ++xx)
			{
				offsets[num][0] = (xx - FRendererDef::s_texelHalf) * du;
				offsets[num][1] = (yy - FRendererDef::s_texelHalf) * dv;
				++num;
			}
		}

		bgfx::setUniform(_handle, offsets, num);
	}

	void setOffsets4x4Lum(bgfx::UniformHandle _handle, uint32_t _width, uint32_t _height)
	{
		float offsets[16][4];

		float du = 1.0f / _width;
		float dv = 1.0f / _height;

		uint16_t num = 0;
		for (uint32_t yy = 0; yy < 4; ++yy)
		{
			for (uint32_t xx = 0; xx < 4; ++xx)
			{
				offsets[num][0] = (xx - 1.0f - FRendererDef::s_texelHalf) * du;
				offsets[num][1] = (yy - 1.0f - FRendererDef::s_texelHalf) * dv;
				++num;
			}
		}

		bgfx::setUniform(_handle, offsets, num);
	}

FHDRPipline::FHDRPipline()
{

}
FHDRPipline::~FHDRPipline()
{

}

}