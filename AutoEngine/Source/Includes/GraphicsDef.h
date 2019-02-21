#pragma once
#include "Auto.h"
#include "FlagSet.h"

#define _OPENGL_4_6_ 0		//OpenGL 4.6
#define _OPENGL_4_PLUS_ 1	//OpenGL 4.3
#define _OPENGL_3_PLUS_ 0	//OpenGL 3.3

#define _DIRECT_3D_12 0		//Direct3D 12

#define AUTO_OPENGL _OPENGL_4_6_ || _OPENGL_4_PLUS_ || _OPENGL_3_PLUS_

#define AUTO_DIRECT_X _DIRECT_3D_12

/// Graphics capability support level. Web platform (Emscripten) also uses OpenGL ES, but is considered a desktop platform capability-wise
#if defined(IOS) || defined(TVOS) || defined(__ANDROID__) || defined(__arm__) || defined(__aarch64__)
#define MOBILE_GRAPHICS
#else
#define DESKTOP_GRAPHICS
#endif
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
/// Maximum simultaneous constant buffers.
static const size_t MAX_CONSTANT_BUFFERS = 15;

namespace Auto3D {



static const int MAX_TEXTURE_UNITS = 16;

// Legacy vertex element bitmasks.
enum VertexMask : unsigned
{
	VERTEX_MASK_NONE = 0x0,
	VERTEX_MASK_POSITION = 0x1,
	VERTEX_MASK_NORMAL = 0x2,
	VERTEX_MASK_COLOR = 0x4,
	VERTEX_MASK_TEXCOORD1 = 0x8,
	VERTEX_MASK_TEXCOORD2 = 0x10,
	VERTEX_MASK_CUBETEXCOORD1 = 0x20,
	VERTEX_MASK_CUBETEXCOORD2 = 0x40,
	VERTEX_MASK_TANGENT = 0x80,
	VERTEX_MASK_BLENDWEIGHTS = 0x100,
	VERTEX_MASK_BLENDINDICES = 0x200,
	VERTEX_MASK_INSTANCEMATRIX1 = 0x400,
	VERTEX_MASK_INSTANCEMATRIX2 = 0x800,
	VERTEX_MASK_INSTANCEMATRIX3 = 0x1000,
	VERTEX_MASK_OBJECTINDEX = 0x2000,
};

AUTO_FLAGSET(VertexMask, VertexMaskFlags);

/// Resource usage modes. Rendertarget usage can only be used with textures.
enum class ResourceUsage
{
	Default = 0,
	Immutable,
	Dynamic,
	RenderTarget
};

/// Texture types.
enum class TextureType
{
	OneD = 0,
	TwoD,
	ThreeD,
	Cube,
};

/// %Geometry type for vertex shader geometry variations.
enum class GeometryType
{
	Static = 0,
	Skinned = 1,
	Instanced = 2,
	Billboard = 3,
	Dirbillboard = 4,
	TrailFaceCamera = 5,
	TrailBone = 6,
	Count = 7,
	// This is not a real geometry type for VS, but used to mark objects that do not desire to be instanced
	StaticNoinstancing = 7,
};

/// Blend factors.
enum class BlendFactor
{
	Zero = 1,
	One,
	SrcColor,
	InvSrcColor,
	SrcAlpha,
	InvSrcAlpha,
	DestAlpha,
	InvDestAlpha,
	DestColor,
	InvDestColor,
	SrcAlphaSat,
	Count
};
/// Shader stages.
enum class ShaderStage
{
	VS = 0,
	PS,
	Count
};

/// Element semantics for vertex elements.
enum class ElementSemantic
{
	Position = 0,
	Normal,
	Binormal,
	Tangent,
	Texcoord,
	Color,
	BlendWeight,
	BlendIndices,
	Count
};


enum class MaterialQuality : unsigned
{
	Low = 0,
	Medium = 1,
	High = 2,
	Max = 15,
};

/// Texture usage types
enum class TextureUsage
{
	Static = 0,
	Dynamic,
	RenderTarget,
	DepthStencil
};

/// Culling mode
enum class CullMode
{
	None = 0,
	CCW,
	CW,
	Count
};

/// Texture filtering mode.
enum class TextureFilterMode
{
	Nearest = 0,
	Bilinear,
	Trilinear,
	Anisotropic,
	NearestAnisotropic,
	Default,
	Count
};

/// Texture addressing mode
enum class TextureAddressMode
{
	Wrap = 0,
	Mirror,
	Clamp,
	Border,
	Count
};

/// Texture coordinates.
enum class TextureCoordinate
{
	U = 0,
	V,
	W,
	Count
};

enum ClearTarget : unsigned
{
	CLEAR_TARGET_COLOR = 0x1,
	CLEAR_TARGET_DEPTH = 0x2,
	CLEAR_TARGET_STENCIL = 0x4
};
AUTO_FLAGSET(ClearTarget, ClearTargetFlags);


/// Texture units
enum class TextureUnit
{
	Diffuse = 0,
	Albedobuffer = 0,
	Normal = 1,
	Normalbuffer = 1,
	Specular = 2,
	Emissive = 3,
	Environment = 4,
#ifdef DESKTOP_GRAPHICS
	Volumemap = 5,
	Custom1 = 6,
	Custom2 = 7,
	Lightramp = 8,
	Lightshape = 9,
	Shadowmap = 10,
	Faceselect = 11,
	Indirection = 12,
	Depthbuffer = 13,
	Lightbuffer = 14,
	Zone = 15,
	MaxMaterialTextureUnits = 8,
	MaxTextureUnits = 16
#else
	Lightramp = 5,
	Lightshape = 6,
	Shadowmap = 7,
	MaxMaterialTextureUnits = 5,
	MaxTextureUnits = 8
#endif
};

/// Depth compare mode.
enum class DepthMode
{
	Always = 0,
	Equal,
	NotEqual,
	Less,
	LessEqual,
	Greater,
	GreaterEqual,
	Count
};

/// Stencil compare mode.
enum class StencilMode
{
	Always = 0,
	Equal,
	NotEqual,
	Less,
	LessEqual,
	Greater,
	GreaterEqual,
	Count
};
/// Fill modes.
enum class FillMode
{
	Wireframe = 2,
	Solid = 3,
	Count
};


/// Depth or stencil compare modes.
enum class CompareFunc
{
	Never = 1,
	Less,
	Equal,
	LessEqual,
	Greater,
	NotEqual,
	GreaterEqual,
	Always,
	Count
};
/// Blend operations.
enum class BlendOp
{
	Add = 1,
	Subtract,
	RevSubtract,
	Min,
	Max,
	Count
};

/// Stencil operation.
enum class StencilOp
{
	Keep = 0,
	Zero,
	Ref,
	Incr,
	Decr
};

enum class ChangeMode
{
	Static = 0,
	Dynamic = 1,
	Stream = 2,
	Count
};

enum class BufferMode
{
	Array = 0,
	ElementArray = 1,
	Count
};

enum class ElementType
{
	Int = 0,
	Float,
	UnsignedByte,
	Count
};
enum class PrimitiveType
{
	TringleList = 0,
	LineList,
	PointList,
	TriangleStrip,
	LineStrip,
	TiangleFan,
	Count
};

/// Cube map faces.
enum class CubeMapFace
{
	PositiveX = 0,
	NegativeX,
	PositiveY,
	NegativeY,
	PositiveZ,
	NegativeZ,
	Count
};

/// Blending mode
enum class BlendMode
{
	Replace = 0,
	Add,
	Multiply,
	Alpha,
	AddAlpha,
	PremulAlpha,
	InvdestAlpha,
	Subtract,
	SubtractAlpha,
	Count
};

/// Hardcoded legacy vertex elements.
enum class LegacyVertexElement
{
	Position = 0,
	Normal,
	Color,
	Texcoord1,
	Texcoord2,
	Cubetexcoord1,
	Cubetexcoord2,
	Tangent,
	Blendweights,
	Blendindices,
	Instancematrix1,
	Instancematrix2,
	Instancematrix3,
	// custom 32-bit integer object index. due to api limitations, not supported on d3d9
	Objectindex,
	Count
};

/// Arbitrary vertex declaration element datatypes.
enum class VertexElementType
{
	Int = 0,
	Float,
	Vector2,
	Vector3,
	Vector4,
	Ubyte4,
	Ubyte4Norm,
	Count
};

/// Arbitrary vertex declaration element semantics.
enum class VertexElementSemantic
{
	Position = 0,
	Normal,
	Binormal,
	Tangent,
	Texcoord,
	Color,
	Blendweights,
	Blendindices,
	Objectindex,
	Count
};

/// Description of a stencil test.
struct StencilTestDesc
{
	/// Default-construct.
	StencilTestDesc()
	{
		Reset();
	}

	/// Reset to defaults.
	void Reset()
	{
		stencilReadMask = 0xff;
		stencilWriteMask = 0xff;
		frontFunc = CompareFunc::Always;
		frontFail = StencilOp::Keep;
		frontDepthFail = StencilOp::Keep;
		frontPass = StencilOp::Keep;
		backFunc = CompareFunc::Always;
		backFail = StencilOp::Keep;
		backDepthFail = StencilOp::Keep;
		backPass = StencilOp::Keep;
	}

	/// Stencil read bit mask.
	unsigned char stencilReadMask;
	/// Stencil write bit mask.
	unsigned char stencilWriteMask;
	/// Stencil front face compare function.
	CompareFunc frontFunc;
	/// Operation for front face stencil test fail.
	StencilOp frontFail;
	/// Operation for front face depth test fail.
	StencilOp frontDepthFail;
	/// Operation for front face pass.
	StencilOp frontPass;
	/// Stencil back face compare function.
	CompareFunc backFunc;
	/// Operation for back face stencil test fail.
	StencilOp backFail;
	/// Operation for back face depth test fail.
	StencilOp backDepthFail;
	/// Operation for back face pass.
	StencilOp backPass;
};

/// Description of a blend mode.
struct BlendModeDesc
{
	/// Default-construct.
	BlendModeDesc()
	{
		Reset();
	}

	/// Construct with parameters.
	BlendModeDesc(bool blendEnable_, BlendFactor srcBlend_, BlendFactor destBlend_, BlendOp blendOp_, BlendFactor srcBlendAlpha_, BlendFactor destBlendAlpha_, BlendOp blendOpAlpha_) :
		blendEnable(blendEnable_),
		srcBlend(srcBlend_),
		destBlend(destBlend_),
		blendOp(blendOp_),
		srcBlendAlpha(srcBlendAlpha_),
		destBlendAlpha(destBlendAlpha_),
		blendOpAlpha(blendOpAlpha_)
	{
	}

	/// Reset to defaults.
	void Reset()
	{
		blendEnable = false;
		srcBlend = BlendFactor::One;
		destBlend = BlendFactor::One;
		blendOp = BlendOp::Add;
		srcBlendAlpha = BlendFactor::One;
		destBlendAlpha = BlendFactor::One;
		blendOpAlpha = BlendOp::Add;
	}

	/// Test for equality with another blend mode description.
	bool operator == (const BlendModeDesc& rhs) const { return blendEnable == rhs.blendEnable && srcBlend == rhs.srcBlend && destBlend == rhs.destBlend && blendOp == rhs.blendOp && srcBlendAlpha == rhs.srcBlendAlpha && destBlendAlpha == rhs.destBlendAlpha && blendOpAlpha == rhs.blendOpAlpha; }
	/// Test for inequality with another blend mode description.
	bool operator != (const BlendModeDesc& rhs) const { return !(*this == rhs); }

	/// Blend enable flag.
	bool blendEnable;
	/// Source color blend factor.
	BlendFactor srcBlend;
	/// Destination color blend factor.
	BlendFactor destBlend;
	/// Color blend operation.
	BlendOp blendOp;
	/// Source alpha blend factor.
	BlendFactor srcBlendAlpha;
	/// Destination alpha blend factor.
	BlendFactor destBlendAlpha;
	/// Alpha blend operation.
	BlendOp blendOpAlpha;
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
		depthFunc = CompareFunc::LessEqual;
		depthWrite = true;
		depthClip = true;
		depthBias = 0;
		slopeScaledDepthBias = 0.0f;
		colorWriteMask = COLORMASK_ALL;
		alphaToCoverage = false;
		blendMode.Reset();
		cullMode = CullMode::CCW;
		fillMode = FillMode::Solid;
		scissorEnable = false;
		scissorRect = RectInt(0, 0, 0, 0);
		stencilEnable = false;
		stencilRef = 0;
		stencilTest.Reset();
	}

	/// Depth test function.
	CompareFunc depthFunc;
	/// Depth write enable.
	bool depthWrite;
	/// Depth clipping enable.
	bool depthClip;
	/// Constant depth bias.
	int depthBias;
	/// Slope-scaled depth bias.
	float slopeScaledDepthBias;
	/// Rendertarget color channel write mask.
	unsigned char colorWriteMask;
	/// Alpha-to-coverage enable.
	bool alphaToCoverage;
	/// Blend mode parameters.
	BlendModeDesc blendMode;
	/// Polygon culling mode.
	CullMode cullMode;
	/// Polygon fill mode.
	FillMode fillMode;
	/// Scissor test enable.
	bool scissorEnable;
	/// Scissor rectangle as pixels from rendertarget top left corner.
	RectInt scissorRect;
	/// Stencil test enable.
	bool stencilEnable;
	/// Stencil reference value.
	unsigned char stencilRef;
	/// Stencil test parameters.
	StencilTestDesc stencilTest;
};

/// Vertex element description for arbitrary vertex declarations
struct VertexElement
{
	/// Default-construct.
	VertexElement() noexcept :
		_type(VertexElementType::Vector3),
		_semantic(VertexElementSemantic::Position),
		_index(0),
		_perInstance(false),
		_offset(0)
	{
	}

	/// Construct with type, semantic, index and whether is per-instance data.
	VertexElement(VertexElementType type, VertexElementSemantic semantic, unsigned char index = 0, bool perInstance = false) noexcept :
		_type(type),
		_semantic(semantic),
		_index(index),
		_perInstance(perInstance),
		_offset(0)
	{
	}

	/// Test for equality with another vertex element. Offset is intentionally not compared, as it's relevant only when an element exists within a vertex buffer.
	bool operator ==(const VertexElement& rhs) const { return _type == rhs._type && _semantic == rhs._semantic && _index == rhs._index && _perInstance == rhs._perInstance; }

	/// Test for inequality with another vertex element.
	bool operator !=(const VertexElement& rhs) const { return !(*this == rhs); }

	/// Data type of element.
	VertexElementType _type;
	/// Semantic of element.
	VertexElementSemantic _semantic;
	/// Semantic index of element, for example multi-texcoords.
	unsigned char _index;
	/// Per-instance flag.
	bool _perInstance;
	/// Offset of element from vertex start. Filled by VertexBuffer once the vertex declaration is built.
	unsigned _offset;
};

static const int MAX_RENDERTARGETS = 4;
static const int MAX_VERTEX_STREAMS = 4;
static const int MAX_CONSTANT_REGISTERS = 256;
static const int BITS_PER_COMPONENT = 8;

extern const unsigned ELEMENT_TYPESIZES[];

extern const VertexElement LEGACY_VERTEXELEMENTS[];

/// Shadow type
enum class ShadowQuality
{
	Simple16bit = 0,
	Simple24bit,
	Pcf16bit,
	Pcf24bit,
	Vsm,
	BlurVsm
};

}