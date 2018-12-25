#pragma once
#include "Auto.h"
namespace Auto3D {
static const unsigned CLEAR_COLOR = 0x1;
static const unsigned CLEAR_DEPTH = 0x2;
static const unsigned CLEAR_STENCIL = 0x4;


static const int MAX_TEXTURE_UNITS = 16;

enum class FillTypes
{
	kFill = 0,
	kLine = 1,
	kPoint = 2
};

enum class ChangeMode
{
	kStatic = 0,
	kDynamic = 1,
	kStream = 2
};

enum class BufferMode
{
	kAarray = 0,
	kElementArray = 1
};

enum class DepthMode
{
	kAlways = 0,
	kNever = 1,
	kLess = 2,
	kEqual = 3,
	kLEqual = 4,
	kGreater = 5,
	kNotEqual = 6,
	kGEqual = 7
};

enum class StencilOps
{
	kKeep = 0,
	kZero,
	kReplace,
	kIncr,
	kIncrWrap,
	kDecr,
	kDecrWrap,
	kInvert
};

enum class BlendSrcFu
{
	kZero = 0,
	kOne,
	kColr,
	kOneMinusSrcColor,
	kAlpha,
	kOneMinusSrcAlpha,
	kConstantColor,
	kOneMinusConstantColor,
	kConstantAlpha,
	kOneMinusConstantAlpha
};

enum class BlendDestFu
{
	kDstColor_COLOR = 0,
	kOneMinusDstColor,
	kDstAlpha,
	kOneMinusDstAlpha
};

enum class BlendOp
{
	kAdd = 0,
	kReversrSubtract
};

enum ElementTypes
{
	kInt = 0,
	kFloat,
	kUnsignedByte
};
enum PrimitiveTypes
{
	kTringleList = 0,
	kLineList,
	kPointList,
	kTriangleStrip,
	kLineStrip,
	kTiangleFan
};

 
}