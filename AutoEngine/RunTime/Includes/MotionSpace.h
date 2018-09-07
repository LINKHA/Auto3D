#pragma once
#include "Auto.h"
#include "stl_use.h"
#include "Singleton.h"
#include "Camera.h"
#include "RunDefines.h"
namespace Auto3D {
class LevelScene;

class MotionSpace : public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(MotionSpace, Object);
	DECLARE_OBJECT_SERIALIZE(MotionSpace);
public:
	explicit MotionSpace(Ambient* ambient);
	/**
	* @brief : Registration level to scene sub system
	*/
	void RegisterLevel(LevelScene* level);
	/**
	* @brief : REmove level for index
	*/
	void RemoveLevel(int index);

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
	SpaceManager() = default;
	~SpaceManager() = default;
	SharedPtr<MotionSpace> space;
	void ModeRunSpace(RunMode runMode);
};
}

