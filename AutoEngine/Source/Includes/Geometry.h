#pragma once
#include "Auto.h"
#include "Object.h"
#include "GraphicsDef.h"

namespace Auto3D {

class VertexBuffer;
class IndexBuffer;

class Geometry : public Object
{
	REGISTER_OBJECT_CLASS(Geometry, Object)
public:
	explicit Geometry(SharedPtr<Ambient> ambient);

};

}