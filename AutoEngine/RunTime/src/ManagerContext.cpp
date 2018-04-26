#include "ManagerContext.h"
#include "BaseObject.h"


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
	#define INIT_MANAGER_CLASS(x) m_ManagerClassIDs[k##x] = Object::StringToClassID (#x); 

}
