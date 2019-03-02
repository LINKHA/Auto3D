#pragma once
#include "../AutoConfig.h"

#include "UISpatialNode.h"

namespace Auto3D
{

class AUTO_API UICamera : public UISpatialNode
{
	REGISTER_OBJECT_CLASS(UICamera, UISpatialNode)
public:
	UICamera();
	~UICamera();
	/// Register factory and attributes.
	static void RegisterObject();
};

}
