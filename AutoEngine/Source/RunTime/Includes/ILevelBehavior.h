#pragma once

#include "Auto.h"

namespace Auto3D {
class Node;

class ILevelBehavior
{
public:
	/**
	* @brief : Create node to level
	*/
	virtual Node* CreateNode(STRING name) = 0;
};

}