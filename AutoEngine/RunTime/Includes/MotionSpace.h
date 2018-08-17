#pragma once
#include "Auto.h"
#include "stl_use.h"
#include "Singleton.h"
#include "Camera.h"
namespace Auto3D {

enum class MotionRunMode
{
	kDefault = -1,
	kAwake,
	kStart,
	kUpdate,
	kFixUpdate,
	kFinish,
	kDraw,
};

class MotionSpace : public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(MotionSpace, Object);
	DECLARE_OBJECT_SERIALIZE(MotionSpace);
public:
	explicit MotionSpace(Ambient* ambient);
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
	virtual void Draw(){}
};
class SpaceManager : public Singleton<SpaceManager>
{
public:
	_VECTOR(MotionSpace*) spaces;
	void RegisterSpace(MotionSpace* space);
	void ModeRunSpace(MotionRunMode runMode);
};
}

