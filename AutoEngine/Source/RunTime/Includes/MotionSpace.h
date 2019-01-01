#pragma once
#include "BehaviorObject.h"
#include "Singleton.h"
#include "Launch.h"

namespace Auto3D {
class LevelScene;

class MotionSpace : public Object
{
	REGISTER_OBJECT_ABSTRACT_CLASS(MotionSpace, Object)
public:
	explicit MotionSpace(SharedPtr<Ambient> ambient);
	/**
	* @brief : Registration level to scene sub system
	*/
	void RegisterScene(SharedPtr<LevelScene> scene);
	/**
	* @brief : REmove level for index
	*/
	void RemoveScene(int id);
	/**
	* @brief : Init space
	*/
	virtual void Init() {}
	/**
	* @brief : Destruct space
	*/
	virtual void Destruct() {}
	/**
	* @brief : Launch application
	*/
	int Run() const;
};
class SpaceHandle : public Singleton<SpaceHandle>
{
public:
	SpaceHandle() = default;
	~SpaceHandle() = default;

	void Init()
	{
		AssertString(space, "No global space!");
		space->Init();
	}
	void Destruct()
	{
		AssertString(space, "No global space!");
		space->Destruct();
	}
	SharedPtr<MotionSpace> space;
};

/// @brief : Regisiter application in main function
#define AUTO_APPLICATION_MAIN(_Class) \
int runApplication() \
{ \
    SharedPtr<Auto3D::Ambient> ambient(new Auto3D::Ambient()); \
    _Class work(ambient);\
    return work.Run(); \
} \
AUTO_MAIN(runApplication())

}

