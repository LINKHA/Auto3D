#pragma once

#include "../AutoConfig.h"
#include "../Base/String.h"
#include "../Math/Rect.h"

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
enum ShaderStage
{
    SHADER_VS = 0,
    SHADER_PS,
    MAX_SHADER_STAGES
};

/// Element types for constant buffers and vertex elements.
enum ElementType
{
    ELEM_INT = 0,
    ELEM_FLOAT,
    ELEM_VECTOR2,
    ELEM_VECTOR3,
    ELEM_VECTOR4,
    ELEM_UBYTE4,
    ELEM_MATRIX3X4,
    ELEM_MATRIX4,
    MAX_ELEMENT_TYPES
};

/// Element semantics for vertex elements.
enum ElementSemantic
{
    SEM_POSITION = 0,
    SEM_NORMAL,
    SEM_BINORMAL,
    SEM_TANGENT,
    SEM_TEXCOORD,
    SEM_COLOR,
    SEM_BLENDWEIGHT,
    SEM_BLENDINDICES,
    MAX_ELEMENT_SEMANTICS
};

/// Primitive types.
enum PrimitiveType
{
    POINT_LIST = 1,
    LINE_LIST,
    LINE_STRIP,
    TRIANGLE_LIST,
    TRIANGLE_STRIP,
    MAX_PRIMITIVE_TYPES
};

/// Blend factors.
enum BlendFactor
{
    BLEND_ZERO = 1,
    BLEND_ONE,
    BLEND_SRC_COLOR,
    BLEND_INV_SRC_COLOR,
    BLEND_SRC_ALPHA,
    BLEND_INV_SRC_ALPHA,
    BLEND_DEST_ALPHA,
    BLEND_INV_DEST_ALPHA,
    BLEND_DEST_COLOR,
    BLEND_INV_DEST_COLOR,
    BLEND_SRC_ALPHA_SAT,
    MAX_BLEND_FACTORS
};

/// Blend operations.
enum BlendOp
{
    BLEND_OP_ADD = 1,
    BLEND_OP_SUBTRACT,
    BLEND_OP_REV_SUBTRACT,
    BLEND_OP_MIN,
    BLEND_OP_MAX,
    MAX_BLEND_OPS
};

/// Predefined blend modes.
enum BlendMode
{
    BLEND_MODE_REPLACE = 0,
    BLEND_MODE_ADD,
    BLEND_MODE_MULTIPLY,
    BLEND_MODE_ALPHA,
    BLEND_MODE_ADDALPHA,
    BLEND_MODE_PREMULALPHA,
    BLEND_MODE_INVDESTALPHA,
    BLEND_MODE_SUBTRACT,
    BLEND_MODE_SUBTRACTALPHA,
    MAX_BLEND_MODES
};

/// Fill modes.
enum FillMode
{
    FILL_WIREFRAME = 2,
    FILL_SOLID = 3,
    MAX_FILL_MODES
};

/// Triangle culling modes.
enum CullMode
{
    CULL_NONE = 1,
    CULL_FRONT,
    CULL_BACK,
    MAX_CULL_MODES
};

/// Depth or stencil compare modes.
enum CompareFunc
{
    CMP_NEVER = 1,
    CMP_LESS,
    CMP_EQUAL,
    CMP_LESS_EQUAL,
    CMP_GREATER,
    CMP_NOT_EQUAL,
    CMP_GREATER_EQUAL,
    CMP_ALWAYS,
    MAX_COMPARE_MODES
};

/// Stencil operations.
enum StencilOp
{
    STENCIL_OP_KEEP = 1,
    STENCIL_OP_ZERO,
    STENCIL_OP_REPLACE,
    STENCIL_OP_INCR_SAT,
    STENCIL_OP_DECR_SAT,
    STENCIL_OP_INVERT,
    STENCIL_OP_INCR,
    STENCIL_OP_DECR,
    MAX_STENCIL_OPS
};

/// Texture types.
enum TextureType
{
    TEX_1D = 0,
    TEX_2D,
    TEX_3D,
    TEX_CUBE,
};

/// Resource usage modes. Rendertarget usage can only be used with textures.
enum ResourceUsage
{
    USAGE_DEFAULT = 0,
    USAGE_IMMUTABLE,
    USAGE_DYNAMIC,
    USAGE_RENDERTARGET
};

/// Texture filtering modes.
enum TextureFilterMode
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

/// Texture addressing modes.
enum TextureAddressMode
{
    ADDRESS_WRAP = 1,
    ADDRESS_MIRROR,
    ADDRESS_CLAMP,
    ADDRESS_BORDER,
    ADDRESS_MIRROR_ONCE
};

/// Description of an element in a vertex declaration.
struct AUTO_API VertexElement
{
    /// Default-construct.
    VertexElement() :
        _type(ELEM_VECTOR3),
        _semantic(SEM_POSITION),
        _index(0),
        _perInstance(false),
        _offset(0)
    {
    }

    /// Construct with type, semantic, index and whether is per-instance data.
    VertexElement(ElementType type, ElementSemantic semantic, unsigned char index = 0, bool perInstance = false) :
        _type(type),
        _semantic(semantic),
        _index(index),
        _perInstance(perInstance),
        _offset(0)
    {
    }

    /// Data type of element.
    ElementType _type;
    /// Semantic of element.
    ElementSemantic _semantic;
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
    Constant(ElementType type, const String& name, size_t numElements = 1) :
        _type(type),
        _name(name),
        _numElements(numElements)
    {
    }

    /// Construct with type, name and optional number of elements.
    Constant(ElementType type, const char* name, size_t numElements = 1) :
        _type(type),
        _name(name),
        _numElements(numElements)
    {
    }

    /// Data type of constant.
    ElementType _type;
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
    BlendModeDesc(bool blendEnable, BlendFactor srcBlend, BlendFactor destBlend, BlendOp blendOp, BlendFactor srcBlendAlpha, BlendFactor destBlendAlpha, BlendOp blendOpAlpha) :
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
        _srcBlend = BLEND_ONE;
        _destBlend = BLEND_ONE;
        _blendOp = BLEND_OP_ADD;
        _srcBlendAlpha = BLEND_ONE;
        _destBlendAlpha = BLEND_ONE;
        _blendOpAlpha = BLEND_OP_ADD;
    }

    /// Test for equality with another blend mode description.
    bool operator == (const BlendModeDesc& rhs) const { return _blendEnable == rhs._blendEnable && _srcBlend == rhs._srcBlend && _destBlend == rhs._destBlend && _blendOp == rhs._blendOp && _srcBlendAlpha == rhs._srcBlendAlpha && _destBlendAlpha == rhs._destBlendAlpha && _blendOpAlpha == rhs._blendOpAlpha; }
    /// Test for inequality with another blend mode description.
    bool operator != (const BlendModeDesc& rhs) const { return !(*this == rhs); }

    /// Blend enable flag.
    bool _blendEnable;
    /// Source color blend factor.
    BlendFactor _srcBlend;
    /// Destination color blend factor.
    BlendFactor _destBlend;
    /// Color blend operation.
    BlendOp _blendOp;
    /// Source alpha blend factor.
    BlendFactor _srcBlendAlpha;
    /// Destination alpha blend factor.
    BlendFactor _destBlendAlpha;
    /// Alpha blend operation.
    BlendOp _blendOpAlpha;
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
        _frontFunc = CMP_ALWAYS;
        _frontFail = STENCIL_OP_KEEP;
        _frontDepthFail = STENCIL_OP_KEEP;
        _frontPass = STENCIL_OP_KEEP;
        _backFunc = CMP_ALWAYS;
        _backFail = STENCIL_OP_KEEP;
        _backDepthFail = STENCIL_OP_KEEP;
        _backPass = STENCIL_OP_KEEP;
    }

    /// Stencil read bit mask.
    unsigned char _stencilReadMask;
    /// Stencil write bit mask.
    unsigned char _stencilWriteMask;
    /// Stencil front face compare function.
    CompareFunc _frontFunc;
    /// Operation for front face stencil test fail.
    StencilOp _frontFail;
    /// Operation for front face depth test fail.
    StencilOp _frontDepthFail;
    /// Operation for front face pass.
    StencilOp _frontPass;
    /// Stencil back face compare function.
    CompareFunc _backFunc;
    /// Operation for back face stencil test fail.
    StencilOp _backFail;
    /// Operation for back face depth test fail.
    StencilOp _backDepthFail;
    /// Operation for back face pass.
    StencilOp _backPass;
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
        _depthFunc = CMP_LESS_EQUAL;
        _depthWrite = true;
        _depthClip = true;
        _depthBias = 0;
        _slopeScaledDepthBias = 0.0f;
        _colorWriteMask = COLORMASK_ALL;
        _alphaToCoverage = false;
        _blendMode.Reset();
        _cullMode = CULL_BACK;
        _fillMode = FILL_SOLID;
        _scissorEnable = false;
        _scissorRect = BaseRect::ZERO;
        _stencilEnable = false;
        _stencilRef = 0;
        _stencilTest.Reset();
    }

    /// Depth test function.
    CompareFunc _depthFunc;
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
    CullMode _cullMode;
    /// Polygon fill mode.
    FillMode _fillMode;
    /// Scissor test enable.
    bool _scissorEnable;
    /// Scissor rectangle as pixels from rendertarget top left corner.
    BaseRect _scissorRect;
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
