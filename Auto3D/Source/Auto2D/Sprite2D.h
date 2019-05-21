#pragma once
#include "GeometryNode2D.h"

namespace Auto3D 
{

class AUTO_API Sprite2D : public GeometryNode2D
{
	REGISTER_OBJECT_CLASS(Sprite2D, GeometryNode2D)

public:
	/// Constructor
	Sprite2D();
	/// Destructor
	~Sprite2D();
	/// Register factory and attributes.
	static void RegisterObject();
};

}