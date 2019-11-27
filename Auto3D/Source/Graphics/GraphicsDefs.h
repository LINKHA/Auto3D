#pragma once

#include "AutoConfig.h"
#include "Container/String.h"
#include "Math/Rect.h"


namespace Auto3D
{

/// Clear rendertarget color.
static const unsigned CLEAR_COLOR = 1;
/// Clear rendertarget depth.
static const unsigned CLEAR_DEPTH = 2;
/// Clear rendertarget stencil.
static const unsigned CLEAR_STENCIL = 4;
/// Clear color+depth+stencil.
static const unsigned CLEAR_ALL = 7;

/// Maximum simultaneous vertex buffers.
static const size_t MAX_VERTEX_STREAMS = 4;
/// Maximum simultaneous constant buffers.
static const size_t MAX_CONSTANT_BUFFERS = 15;
/// Maximum number of textures in use at once.
static const size_t MAX_TEXTURE_UNITS = 16;
/// Maximum number of textures reserved for materials, starting from 0.
static const size_t MAX_MATERIAL_TEXTURE_UNITS = 8;
/// Maximum number of color rendertargets in use at once.
static const size_t MAX_RENDERTARGETS = 4;
/// Number of cube map faces.
static const size_t MAX_CUBE_FACES = 6;

/// Disable color write.
static const unsigned char COLORMASK_NONE = 0x0;
/// Write to red channel.
static const unsigned char COLORMASK_R = 0x1;
/// Write to green channel.
static const unsigned char COLORMASK_G = 0x2;
/// Write to blue channel.
static const unsigned char COLORMASK_B = 0x4;
/// Write to alpha channel.
static const unsigned char COLORMASK_A = 0x8;
/// Write to all color channels (default.)
static const unsigned char COLORMASK_ALL = 0xf;

/// Shader stages.
namespace ShaderStage
{
	enum Type
	{
		VS = 0,
		PS,
		Count
	};
};


/// Element types for constant buffers and vertex elements.
namespace ElementType
{
	enum Type
	{
		INT = 0,
		FLOAT,
		VECTOR2,
		VECTOR3,
		VECTOR4,
		UBYTE4,
		MATRIX3X4,
		MATRIX4,
		Count
	};
};

/// Element semantics for vertex elements.
namespace ElementSemantic
{
	enum Type
	{
		POSITION = 0,
		NORMAL,
		BINORMAL,
		TANGENT,
		TEXCOORD,
		COLOR,
		BLENDWEIGHT,
		BLENDINDICES,
		Count
	};
};


/// Primitive types.
namespace PrimitiveType
{
	enum Type
	{
		POINT_LIST = 1,
		LINE_LIST,
		LINE_STRIP,
		TRIANGLE_LIST,
		TRIANGLE_STRIP,
		Count
	};
};

/// Blend factors.
namespace BlendFactor
{
	enum Type
	{
		ZERO = 1,
		ONE,
		SRC_COLOR,
		INV_SRC_COLOR,
		SRC_ALPHA,
		INV_SRC_ALPHA,
		DEST_ALPHA,
		INV_DEST_ALPHA,
		DEST_COLOR,
		INV_DEST_COLOR,
		SRC_ALPHA_SAT,
		Count
	};
};


/// Blend operations.
namespace BlendOp
{
	enum Type
	{
		ADD = 1,
		SUBTRACT,
		REV_SUBTRACT,
		MIN,
		MAX,
		Count
	};
};

/// Predefined blend modes.
namespace BlendMode
{
	enum Type
	{
		REPLACE = 0,
		ADD,
		MULTIPLY,
		ALPHA,
		ADDALPHA,
		PREMULALPHA,
		INVDESTALPHA,
		SUBTRACT,
		SUBTRACTALPHA,
		Count
	};
};

/// Fill modes.
namespace FillMode
{
	enum Type
	{
		WIREFRAME = 2,
		SOLID = 3,
		Count
	};
};

/// Triangle culling modes.
namespace CullMode
{
	enum Type
	{
		NONE = 1,
		FRONT,
		BACK,
		Count
	};
}; 


/// Depth or stencil compare modes.
namespace CompareFunc
{
	enum Type
	{
		NEVER = 1,
		LESS,
		EQUAL,
		LESS_EQUAL,
		GREATER,
		NOT_EQUAL,
		GREATER_EQUAL,
		ALWAYS,
		Count
	};
};

/// Stencil operations.
namespace StencilOp
{
	enum Type
	{
		KEEP = 1,
		ZERO,
		REPLACE,
		INCR_SAT,
		DECR_SAT,
		INVERT,
		INCR,
		DECR,
		Count
	};
};

/// Texture types.
namespace TextureType
{
	enum Type
	{
#ifndef AUTO_OPENGL_ES
		TEX_1D = 0,
#endif
		TEX_2D,
		TEX_3D,
		TEX_CUBE,
	};
};

/// Resource usage modes. Rendertarget usage can only be used with textures.
namespace ResourceUsage
{
	enum Type
	{
		DEFAULT = 0,
		IMMUTABLE,
		DYNAMIC,
		RENDERTARGET
	};
};

/// Texture filtering modes.
namespace TextureFilterMode
{
	enum Type
	{
		FILTER_POINT = 0,
		FILTER_BILINEAR,
		FILTER_TRILINEAR,
		FILTER_ANISOTROPIC,
		COMPARE_POINT,
		COMPARE_BILINEAR,
		COMPARE_TRILINEAR,
		COMPARE_ANISOTROPIC
	};
};

/// Texture addressing modes.
namespace TextureAddressMode
{
	enum Type
	{
		WRAP = 1,
		MIRROR,
		CLAMP,
#ifndef AUTO_OPENGL_ES
		BORDER,
		MIRROR_ONCE
#endif
	};
};


/// Description of an element in a vertex declaration.
struct AUTO_API VertexElement
{
    /// Default-construct.
    VertexElement() :
        _type(ElementType::VECTOR3),
        _semantic(ElementSemantic::POSITION),
        _index(0),
        _perInstance(false),
        _offset(0)
    {
    }

    /// Construct with type, semantic, index and whether is per-instance data.
    VertexElement(ElementType::Type type, ElementSemantic::Type semantic, unsigned char index = 0, bool perInstance = false) :
        _type(type),
        _semantic(semantic),
        _index(index),
        _perInstance(perInstance),
        _offset(0)
    {
    }

    /// Data type of element.
    ElementType::Type _type;
    /// Semantic of element.
    ElementSemantic::Type _semantic;
    /// Semantic index of element, for example multi-texcoords.
    unsigned char _index;
    /// Per-instance flag.
    bool _perInstance;
    /// Offset of element from vertex start. Filled by VertexBuffer.
    size_t _offset;
};

/// Description of a shader constant.
struct AUTO_API Constant
{
    /// Construct empty.
    Constant() :
        _numElements(1)
    {
    }

    /// Construct with type, name and optional number of elements.
    Constant(ElementType::Type type, const String& name, size_t numElements = 1) :
        _type(type),
        _name(name),
        _numElements(numElements)
    {
    }

    /// Construct with type, name and optional number of elements.
    Constant(ElementType::Type type, const char* name, size_t numElements = 1) :
        _type(type),
        _name(name),
        _numElements(numElements)
    {
    }

    /// Data type of constant.
    ElementType::Type _type;
    /// Name of constant.
    String _name;
    /// Number of elements. Default 1.
    size_t _numElements;
    /// Element _size. Filled by ConstantBuffer.
    size_t _elementSize;
    /// Offset from the beginning of the buffer. Filled by ConstantBuffer.
    size_t _offset;
};

/// Description of a blend mode.
struct AUTO_API BlendModeDesc
{
    /// Default-construct.
    BlendModeDesc()
    {
        Reset();
    }

    /// Construct with parameters.
    BlendModeDesc(bool blendEnable, BlendFactor::Type srcBlend, BlendFactor::Type destBlend, BlendOp::Type blendOp, BlendFactor::Type srcBlendAlpha, BlendFactor::Type destBlendAlpha, BlendOp::Type blendOpAlpha) :
        _blendEnable(blendEnable),
        _srcBlend(srcBlend),
        _destBlend(destBlend),
        _blendOp(blendOp),
        _srcBlendAlpha(srcBlendAlpha),
        _destBlendAlpha(destBlendAlpha),
        _blendOpAlpha(blendOpAlpha)
    {
    }

    /// Reset to defaults.
    void Reset()
    {
        _blendEnable = false;
        _srcBlend = BlendFactor::ONE;
        _destBlend = BlendFactor::ONE;
        _blendOp = BlendOp::ADD;
        _srcBlendAlpha = BlendFactor::ONE;
        _destBlendAlpha = BlendFactor::ONE;
        _blendOpAlpha = BlendOp::ADD;
    }

    /// Test for equality with another blend mode description.
    bool operator == (const BlendModeDesc& rhs) const { return _blendEnable == rhs._blendEnable && _srcBlend == rhs._srcBlend && _destBlend == rhs._destBlend && _blendOp == rhs._blendOp && _srcBlendAlpha == rhs._srcBlendAlpha && _destBlendAlpha == rhs._destBlendAlpha && _blendOpAlpha == rhs._blendOpAlpha; }
    /// Test for inequality with another blend mode description.
    bool operator != (const BlendModeDesc& rhs) const { return !(*this == rhs); }

    /// Blend enable flag.
    bool _blendEnable;
    /// Source color blend factor.
    BlendFactor::Type _srcBlend;
    /// Destination color blend factor.
    BlendFactor::Type _destBlend;
    /// Color blend operation.
    BlendOp::Type _blendOp;
    /// Source alpha blend factor.
    BlendFactor::Type _srcBlendAlpha;
    /// Destination alpha blend factor.
    BlendFactor::Type _destBlendAlpha;
    /// Alpha blend operation.
    BlendOp::Type _blendOpAlpha;
};

/// Description of a stencil test.
struct AUTO_API StencilTestDesc
{
    /// Default-construct.
    StencilTestDesc()
    {
        Reset();
    }

    /// Reset to defaults.
    void Reset()
    {
        _stencilReadMask = 0xff;
        _stencilWriteMask = 0xff;
        _frontFunc = CompareFunc::ALWAYS;
        _frontFail = StencilOp::KEEP;
        _frontDepthFail = StencilOp::KEEP;
        _frontPass = StencilOp::KEEP;
        _backFunc = CompareFunc::ALWAYS;
        _backFail = StencilOp::KEEP;
        _backDepthFail = StencilOp::KEEP;
        _backPass = StencilOp::KEEP;
    }

    /// Stencil read bit mask.
    unsigned char _stencilReadMask;
    /// Stencil write bit mask.
    unsigned char _stencilWriteMask;
    /// Stencil front face compare function.
    CompareFunc::Type _frontFunc;
    /// Operation for front face stencil test fail.
    StencilOp::Type _frontFail;
    /// Operation for front face depth test fail.
    StencilOp::Type _frontDepthFail;
    /// Operation for front face pass.
    StencilOp::Type _frontPass;
    /// Stencil back face compare function.
    CompareFunc::Type _backFunc;
    /// Operation for back face stencil test fail.
    StencilOp::Type _backFail;
    /// Operation for back face depth test fail.
    StencilOp::Type _backDepthFail;
    /// Operation for back face pass.
    StencilOp::Type _backPass;
};

/// Collection of render state.
struct RenderState
{
    /// Default-construct.
    RenderState()
    {
        Reset();
    }

    /// Reset to defaults.
    void Reset()
    {
        _depthFunc = CompareFunc::LESS_EQUAL;
        _depthWrite = true;
        _depthClip = true;
        _depthBias = 0;
        _slopeScaledDepthBias = 0.0f;
        _colorWriteMask = COLORMASK_ALL;
        _alphaToCoverage = false;
        _blendMode.Reset();
        _cullMode = CullMode::BACK;
        _fillMode = FillMode::SOLID;
        _scissorEnable = false;
        _scissorRect = RectI::ZERO;
        _stencilEnable = false;
        _stencilRef = 0;
        _stencilTest.Reset();
    }

    /// Depth test function.
    CompareFunc::Type _depthFunc;
    /// Depth write enable.
    bool _depthWrite;
    /// Depth clipping enable.
    bool _depthClip;
    /// Constant depth bias.
    int _depthBias;
    /// Slope-scaled depth bias.
    float _slopeScaledDepthBias;
    /// Rendertarget color channel write mask.
    unsigned char _colorWriteMask;
    /// Alpha-to-coverage enable.
    bool _alphaToCoverage;
    /// Blend mode parameters.
    BlendModeDesc _blendMode;
    /// Polygon culling mode.
    CullMode::Type _cullMode;
    /// Polygon fill mode.
    FillMode::Type _fillMode;
    /// Scissor test enable.
    bool _scissorEnable;
    /// Scissor rectangle as pixels from rendertarget top left corner.
    RectI _scissorRect;
    /// Stencil test enable.
    bool _stencilEnable;
    /// Stencil reference value.
    unsigned char _stencilRef;
    /// Stencil test parameters.
    StencilTestDesc _stencilTest;
};

/// Vertex element sizes by element type.
extern AUTO_API const size_t elementSizes[];
/// Resource usage names.
extern AUTO_API const char* resourceUsageNames[];
/// Element type names.
extern AUTO_API const char* elementTypeNames[];
/// Vertex element semantic names.
extern AUTO_API const char* elementSemanticNames[];
/// Blend factor names.
extern AUTO_API const char* blendFactorNames[];
/// Blend operation names.
extern AUTO_API const char* blendOpNames[];
/// Predefined blend mode names.
extern AUTO_API const char* blendModeNames[];
/// Fill mode names.
extern AUTO_API const char* fillModeNames[];
/// Culling mode names.
extern AUTO_API const char* cullModeNames[];
/// Compare function names.
extern AUTO_API const char* compareFuncNames[];
/// Stencil operation names.
extern AUTO_API const char* stencilOpNames[];
/// Predefined blend modes.
extern AUTO_API const BlendModeDesc blendModes[];

}
