#pragma once
#include "Auto.h"
#include "AutoSTL.h"
#include "Singleton.h"
#include "Camera.h"
#include "RunDefines.h"
#include "StateVessel.h"

namespace Auto3D {

class LevelScene;
class MotionSpace : public StateVessel
{
	REGISTER_DERIVED_ABSTRACT_CLASS(MotionSpace, StateVessel);
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

};
class SpaceHandle : public Singleton<SpaceHandle>
{
public:
	SpaceHandle() = default;
	~SpaceHandle() = default;
	SharedPtr<MotionSpace> space;
	void ModeRunSpace(RunMode runMode);
};
}

