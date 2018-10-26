#include "LightContainer.h"


namespace Auto3D {

LightContainer::LightContainer(Ambient* ambient)
	: Super(ambient)
	, _isRenderOrCull(false)
{}
LightContainer::~LightContainer()
{}
void LightContainer::AddLight(Light* source)
{
	Assert(source);
	//Maybe Delay add?
	_lights.PushBack(source);
}
void LightContainer::RemoveLight(Light* source)
{
	Assert(source);
	for (Lights::Iterator it = _lights.Begin(); it != _lights.End(); ++it)
	{
		if (*it = source)
		{
			//Maybe Delay delete?
			_lights.Erase(it);
		}
	}
	if (_lastMainLight == source)
		_lastMainLight = nullptr;
}
int LightContainer::Size()
{
	return _lights.Size();
}

}