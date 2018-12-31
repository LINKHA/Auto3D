#include "LightContainer.h"
#include "NewDef.h"

namespace Auto3D {

LightContainer::LightContainer(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, _isRenderOrCull(false)
{}
LightContainer::~LightContainer()
{}
void LightContainer::AddLight(SharedPtr<Light> source)
{
	Assert(source);
	//Maybe Delay add?
	_lights.push_back(source);
}
void LightContainer::RemoveLight(SharedPtr<Light> source)
{
	Assert(source);
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
int LightContainer::Size()
{
	return _lights.size();
}

}