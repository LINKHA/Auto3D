#pragma once
#include "Object.h"
#include "Node.h"
namespace Auto3D {
class Prefab : public Object
{
	REGISTER_OBJECT_CLASS(Prefab, Object)
public:
	explicit Prefab(Ambient* ambient);
private:
	Node* _gameObject;
};

}
