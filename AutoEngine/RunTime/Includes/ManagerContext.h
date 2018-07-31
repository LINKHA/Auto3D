#pragma once
#include "Auto.h"
namespace Auto3D {
class Object;
class ManagerContext
{
public:
	enum Mangagers
	{
		//Global Managers
		kTimeManager,
		kScriptManager,
		kPreLoadManager,
		kGlobalManagerCount,
		//Level Managers
		kSceneManager,
		kRenderManager,
		kGameObjectManager,
		kLightManager,
		kManagerCount
	};

	ManagerContext();
	void InitializeClasses();
	Object*	_managers[kManagerCount];
	int		_managerClassIDs[kManagerCount];
};
}

