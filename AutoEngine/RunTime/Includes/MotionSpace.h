#pragma once
#include "Auto.h"
#include "stl_use.h"
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
class SpaceManager : public Singleton<SpaceManager>
{
public:
	SpaceManager() = default;
	~SpaceManager() = default;
	SharedPtr<MotionSpace> space;
	void ModeRunSpace(RunMode runMode);
};
}

