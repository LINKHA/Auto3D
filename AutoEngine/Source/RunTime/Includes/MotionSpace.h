#pragma once
#include "Auto.h"
#include "Singleton.h"
#include "Camera.h"
#include "RunDefines.h"
#include "BehaviorObject.h"

namespace Auto3D {

class LevelScene;
class MotionSpace : public BehaviorObject
{
	REGISTER_DERIVED_ABSTRACT_CLASS(MotionSpace, BehaviorObject);
	DECLARE_OBJECT_SERIALIZE(MotionSpace);
public:
	explicit MotionSpace(Ambient* ambient);
	/**
	* @brief : Registration level to scene sub system
	*/
	void RegisterScene(LevelScene* scene);
	/**
	* @brief : REmove level for index
	*/
	void RemoveScene(int id);

};
class SpaceHandle : public Singleton<SpaceHandle>
{
public:
	SpaceHandle() = default;
	~SpaceHandle() = default;
	sharedPtr<MotionSpace> space;
	void ModeRunSpace(RunMode runMode);
};
}

