#pragma once
namespace Auto3D
{

namespace EGraphicsVersion
{
	enum Type
	{
		OPENGL_4_3,
		OPENGL_3_3,
		OPENGL_2_0,
		OPENGL_ES_3_0,
		OPENGL_ES_2_0,
		VULKAN_1_0
	};
}

namespace EGraphicsSLVersion
{
	enum Type
	{
		GLSL_430,
		GLSL_330,
		GLSL_150,
		GLSL_450,
		GLSL_ES_300,
		GLSL_ES_100
	};
}

}