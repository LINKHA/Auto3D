#pragma once
#include "BaseObject.h"

AUTO_BEGIN
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
AUTO_END

