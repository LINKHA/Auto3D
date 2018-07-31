#include "ManagerContext.h"

#include "Time.h"
namespace Auto3D {
#define INIT_MANAGER(x) _managerClassIDs[k##x] = Object::StringToClassID (#x); 
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
	INIT_MANAGER(TimeManager)
}
#undef INIT_MANAGER
}