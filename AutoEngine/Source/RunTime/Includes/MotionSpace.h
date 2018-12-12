#pragma once
#include "BehaviorObject.h"
#include "Singleton.h"
#include "Launch.h"

namespace Auto3D {

class LevelScene;
class MotionSpace : public BehaviorObject
{
	REGISTER_OBJECT_ABSTRACT_CLASS(MotionSpace, BehaviorObject)
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
	/**
	* @brief : Launch application
	*/
	int Launch() const;
};
class SpaceHandle : public Singleton<SpaceHandle>
{
public:
	SpaceHandle() = default;
	~SpaceHandle() = default;
	SharedPtr<MotionSpace> space;
	void ModeRunSpace(RunMode runMode);
};

/// @brief : Regisiter application in main function
#define AUTO_APPLICATION_MAIN(className) \
int runApplication() \
{ \
    SharedPtr<Auto3D::Ambient> ambient(new Auto3D::Ambient()); \
    className work(ambient.get());\
    return work.Launch(); \
} \
AUTO_MAIN(runApplication());

}

