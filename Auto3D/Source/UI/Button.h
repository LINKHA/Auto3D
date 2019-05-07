#pragma once
#include "../Renderer/GeometryNode.h"

namespace Auto3D
{

class AUTO_API Button : public UIGeometryNode
{
	REGISTER_OBJECT_CLASS(Button, UIGeometryNode)

public:
	Button();
	~Button();
	/// Register factory and attributes.
	static void RegisterObject();

};

}