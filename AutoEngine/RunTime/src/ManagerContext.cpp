#include "ManagerContext.h"

#include "TimeManager.h"
AUTO_BEGIN
ManagerContext::ManagerContext()
{
	for (int i = 0; i < kManagerCount; i++)
		_managers[i] = NULL;
}

void ManagerContext::InitializeClasses()
{
	for (int i = 0; i < kManagerCount; i++)
	{
		_managerClassIDs[i] = -1;
	}
	#define INIT_MANAGER(x) _managerClassIDs[k##x] = Object::StringToClassID (#x); 
	INIT_MANAGER(TimeManager)
}
AUTO_END