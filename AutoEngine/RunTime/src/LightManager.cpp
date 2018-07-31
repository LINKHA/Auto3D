#include "LightManager.h"

namespace Auto3D {
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
	assert(source);
	//Maybe Delay add?
	_lights.push_back(source);
}
void LightManager::RemoveLight(Light* source)
{
	assert(source);
	for (Lights::iterator it = _lights.begin(); it != _lights.end(); ++it)
	{
		if (*it = source)
		{
			//Maybe Delay delete?
			_lights.erase(it);
		}
	}
	if (_lastMainLight == source)
		_lastMainLight = nullptr;
}
int LightManager::Size()
{
	return _lights.size();
}
}