#ifndef MANAGER_CONTEXT_H_
#define MANAGER_CONTEXT_H_
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
		kScriptMapper,
		kMotionManager,
		kGlobalManagerCount,
		//Level Managers
		kSceneManager,
		kRenderManager,
		kCameraManager,
		kGameObjectManager,
		kManagerCount
	};

	ManagerContext();
	void InitializeClasses();
	Object*	m_Managers[kManagerCount];
	int		m_ManagerClassIDs[kManagerCount];
};
AUTO_END
#endif //!MANAGER_CONTEXT_H_
