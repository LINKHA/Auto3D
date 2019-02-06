#pragma once
#include "Auto.h"

#define _OPENGL_4_6_ 0		//OpenGL 4.6
#define _OPENGL_4_PLUS_ 1	//OpenGL 4.3
#define _OPENGL_3_PLUS_ 0	//OpenGL 3.3

#define _DIRECT_3D_12 0		//Direct3D 12

#define AUTO_OPENGL _OPENGL_4_6_ || _OPENGL_4_PLUS_ || _OPENGL_3_PLUS_

#define AUTO_DIRECT_X _DIRECT_3D_12

namespace Auto3D {


static const int CLEAR_TARGET_COLOR = 0x1;
static const int CLEAR_TARGET_DEPTH = 0x2;
static const int CLEAR_TARGET_STENCIL = 0x4;


static const int MAX_TEXTURE_UNITS = 16;

enum class FillTypes
{
	Fill = 0,
	Line = 1,
	Point = 2
};

enum class ChangeMode
{
	Static = 0,
	Dynamic = 1,
	Stream = 2
};

enum class BufferMode
{
	Aarray = 0,
	ElementArray = 1
};

enum class DepthMode
{
	Always = 0,
	Never = 1,
	Less = 2,
	Equal = 3,
	LEqual = 4,
	Greater = 5,
	NotEqual = 6,
	GEqual = 7
};

enum class StencilOps
{
	Keep = 0,
	Zero,
	Replace,
	Incr,
	IncrWrap,
	Decr,
	DecrWrap,
	Invert
};

enum class BlendSrcFu
{
	Zero = 0,
	One,
	Colr,
	OneMinusSrcColor,
	Alpha,
	OneMinusSrcAlpha,
	ConstantColor,
	OneMinusConstantColor,
	ConstantAlpha,
	OneMinusConstantAlpha
};

enum class BlendDestFu
{
	DstColor_COLOR = 0,
	OneMinusDstColor,
	DstAlpha,
	OneMinusDstAlpha
};

enum class BlendOp
{
	Add = 0,
	ReversrSubtract
};

enum ElementTypes
{
	Int = 0,
	Float,
	UnsignedByte
};
enum PrimitiveTypes
{
	TringleList = 0,
	LineList,
	PointList,
	TriangleStrip,
	LineStrip,
	TiangleFan
};


}