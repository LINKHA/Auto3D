#include "LightManager.h"

AUTO_BEGIN
SINGLETON_INSTANCE(LightManager);
LightManager::LightManager()
	:m_RenderOrCull(false)
{
}
LightManager::~LightManager()
{
}
void LightManager::AddLight(Light* source)
{
	DebugAssert(source);
	//Maybe Delay add?
	m_Lights.push_back(source);
}
void LightManager::RemoveLight(Light* source)
{
	DebugAssert(source);
	for (Lights::iterator it = m_Lights.begin(); it != m_Lights.end(); ++it)
	{
		if (*it = source)
		{
			//Maybe Delay delete?
			m_Lights.erase(it);
		}
	}
	if (m_LastMainLight == source)
		m_LastMainLight = NULL;
}
int LightManager::Size()
{
	return m_Lights.size();
}
AUTO_END