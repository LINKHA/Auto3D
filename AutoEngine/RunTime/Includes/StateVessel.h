#pragma once
#include "Object.h"

namespace Auto3D {

class StateVessel : public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(StateVessel, Object);
	DECLARE_OBJECT_SERIALIZE(StateVessel);
public:
	explicit StateVessel(Ambient* ambient);

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
	virtual void Draw() {}
};

}