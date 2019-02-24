#include "GraphicsDef.h"

namespace Auto3D {

extern const unsigned ELEMENT_TYPESIZES[] =
{
	sizeof(int),
	sizeof(float),
	2 * sizeof(float),
	3 * sizeof(float),
	4 * sizeof(float),
	sizeof(unsigned),
	sizeof(unsigned)
};

extern const VertexElement LEGACY_VERTEXELEMENTS[] =
{
	VertexElement(VertexElementType::Vector3, VertexElementSemantic::Position, 0, false),     // Position
	VertexElement(VertexElementType::Vector3, VertexElementSemantic::Normal, 0, false),       // Normal
	VertexElement(VertexElementType::Ubyte4Norm, VertexElementSemantic::Color, 0, false),	  // Color
	VertexElement(VertexElementType::Vector2, VertexElementSemantic::Texcoord, 0, false),     // Texcoord1
	VertexElement(VertexElementType::Vector2, VertexElementSemantic::Texcoord, 1, false),     // Texcoord2
	VertexElement(VertexElementType::Vector3, VertexElementSemantic::Texcoord, 0, false),     // cubetexcoord1
	VertexElement(VertexElementType::Vector3, VertexElementSemantic::Texcoord, 1, false),     // cubetexcoord2
	VertexElement(VertexElementType::Vector4, VertexElementSemantic::Tangent, 0, false),      // tangent
	VertexElement(VertexElementType::Vector4, VertexElementSemantic::Blendweights, 0, false), // blendweights
	VertexElement(VertexElementType::Ubyte4, VertexElementSemantic::Blendindices, 0, false),  // blendindices
	VertexElement(VertexElementType::Vector4, VertexElementSemantic::Texcoord, 4, true),      // instancematrix1
	VertexElement(VertexElementType::Vector4, VertexElementSemantic::Texcoord, 5, true),      // instancematrix2
	VertexElement(VertexElementType::Vector4, VertexElementSemantic::Texcoord, 6, true),      // instancematrix3
	VertexElement(VertexElementType::Int, VertexElementSemantic::Objectindex, 0, false)       // objectindex
};

extern const BlendModeDesc blendModes[] =
{
	BlendModeDesc(false, BlendFactor::One, BlendFactor::One, BlendOp::Add, BlendFactor::One, BlendFactor::One, BlendOp::Add),
	BlendModeDesc(true, BlendFactor::One, BlendFactor::One, BlendOp::Add, BlendFactor::One, BlendFactor::One, BlendOp::Add),
	BlendModeDesc(true, BlendFactor::DestAlpha, BlendFactor::Zero, BlendOp::Add, BlendFactor::DestAlpha, BlendFactor::Zero, BlendOp::Add),
	BlendModeDesc(true, BlendFactor::SrcAlpha, BlendFactor::InvSrcAlpha, BlendOp::Add, BlendFactor::SrcAlpha, BlendFactor::InvSrcAlpha, BlendOp::Add),
	BlendModeDesc(true, BlendFactor::SrcAlpha, BlendFactor::One, BlendOp::Add, BlendFactor::SrcAlpha, BlendFactor::One, BlendOp::Add),
	BlendModeDesc(true, BlendFactor::One, BlendFactor::InvSrcAlpha, BlendOp::Add, BlendFactor::One, BlendFactor::InvSrcAlpha, BlendOp::Add),
	BlendModeDesc(true, BlendFactor::InvDestAlpha, BlendFactor::DestAlpha, BlendOp::Add, BlendFactor::InvDestAlpha, BlendFactor::DestAlpha, BlendOp::Add),
	BlendModeDesc(true, BlendFactor::One, BlendFactor::One, BlendOp::RevSubtract , BlendFactor::One, BlendFactor::One, BlendOp::RevSubtract),
	BlendModeDesc(true, BlendFactor::SrcAlpha, BlendFactor::One, BlendOp::RevSubtract, BlendFactor::SrcAlpha, BlendFactor::One, BlendOp::RevSubtract)
};

}