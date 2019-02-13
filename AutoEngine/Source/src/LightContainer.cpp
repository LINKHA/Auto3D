#include "LightContainer.h"
#include "NewDef.h"

namespace Auto3D {

LightContainer::LightContainer(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, _isRenderOrCull(false)
{}
LightContainer::~LightContainer()
{}
void LightContainer::AddLight(SharedPtr<tLight> source)
{
	Assert(source);
	//Maybe Delay add?
	_lights.push_back(source);
}
void LightContainer::RemoveLight(SharedPtr<tLight> source)
{
	Assert(source);
	for (VECTOR<SharedPtr<tLight> >::iterator it = _lights.begin(); it != _lights.end(); ++it)
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