#ifndef MANAGER_CONTEXT_H_
#define MANAGER_CONTEXT_H_


class Object;
struct ManagerContext
{
	enum Mangagers
	{
		//Global Managers
		kTimeManager,
		kScriptMapper,
		kGlobalManagerCount,
		//Level Managers
		kSceneManager,
		kRenderManager,
		kManagerCount
	};

	ManagerContext();
	void InitializeClasses();
	Object*	m_Managers[kManagerCount];
	int		m_ManagerClassIDs[kManagerCount];
};
#endif //!MANAGER_CONTEXT_H_
