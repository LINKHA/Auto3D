#pragma once
#include "Object.h"
#include "GraphicsDef.h"
namespace Auto3D {
class Geometry : public Object
{
	REGISTER_OBJECT_CLASS(Geometry, Object)
public:
	explicit Geometry(SharedPtr<Ambient> ambient);
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

