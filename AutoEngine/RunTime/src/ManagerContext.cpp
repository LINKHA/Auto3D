#include "ManagerContext.h"

#include "TimeManager.h"
AUTO_BEGIN
ManagerContext::ManagerContext()
{
	for (int i = 0; i < kManagerCount; i++)
		m_Managers[i] = NULL;
}

void ManagerContext::InitializeClasses()
{
	for (int i = 0; i < kManagerCount; i++)
	{
		m_ManagerClassIDs[i] = -1;
	}
	#define INIT_MANAGER(x) m_ManagerClassIDs[k##x] = Object::StringToClassID (#x); 
	INIT_MANAGER(TimeManager)
}
AUTO_END