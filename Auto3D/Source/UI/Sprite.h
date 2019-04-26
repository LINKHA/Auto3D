#pragma once
#include "../Renderer/GeometryNode.h"

namespace Auto3D 
{

class AUTO_API Sprite : public UIGeometryNode
{
	REGISTER_OBJECT_CLASS(Sprite, UIGeometryNode)

public:
	/// Constructor
	Sprite();
	/// Destructor
	~Sprite();
	/// Register factory and attributes.
	static void RegisterObject();
};

}