#pragma once
#include "Object.h"
#include "Node.h"
namespace Auto3D {
class Prefab : public Object
{
	REGISTER_DERIVED_CLASS(Prefab, Object);
	DECLARE_OBJECT_SERIALIZE(Prefab);
public:
	explicit Prefab(Ambient* ambient);
private:
	Node* _gameObject;
};

}
