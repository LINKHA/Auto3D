#pragma once
#include "Object.h"
#include "GraphicsDefines.h"
namespace Auto3D {
class Graphics;
class Geometry : public Object
{
	REGISTER_DERIVED_CLASS(Geometry, Object);
	DECLARE_OBJECT_SERIALIZE(Geometry);
public:
	explicit Geometry(Ambient* ambient);
	FillTypes GetFillType()const { return _fillType; }


	unsigned GetIndexStart() const { return _indexStart; }
	unsigned GetIndexCount() const { return _indexCount; }
	unsigned GetVertexStart() const { return _vertexStart; }
	unsigned GetVertexCount() const { return _vertexCount; }
private:
	FillTypes _fillType{};
	unsigned _indexStart;
	unsigned _indexCount;
	unsigned _vertexStart;
	unsigned _vertexCount;

};

}

