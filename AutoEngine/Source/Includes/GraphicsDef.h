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