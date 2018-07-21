#include "LightManager.h"

AUTO_BEGIN
SINGLETON_INSTANCE(LightManager);
LightManager::LightManager()
	:_isRenderOrCull(false)
{
}
LightManager::~LightManager()
{
}
void LightManager::AddLight(Light* source)
{
	DebugAssert(source);
	//Maybe Delay add?
	_lights.push_back(source);
}
void LightManager::RemoveLight(Light* source)
{
	DebugAssert(source);
	for (Lights::iterator it = _lights.begin(); it != _lights.end(); ++it)
	{
		if (*it = source)
		{
			//Maybe Delay delete?
			_lights.erase(it);
		}
	}
	if (_lastMainLight == source)
		_lastMainLight = NULL;
}
int LightManager::Size()
{
	return _lights.size();
}
AUTO_END