#include "Hardware.h"

namespace Auto3D {


/////////////////////////////////////////////////////////////////////////////////////////////
//Monitors
/////////////////////////////////////////////////////////////////////////////////////////////
Monitors::Monitors()
{
	SDL_Rect rect;
	_monitorCount = SDL_GetNumVideoDisplays();
	for (int i = 0; i < _monitorCount; i++)
	{
		SDL_GetDisplayBounds(i, &rect);
		_mnitors.push_back(M_PAIR(rect.w, rect.h));
	}
}

Int32 Monitors::GetMonitorsCount()
{
	return _monitorCount;
}

Int32 Monitors::GetMonitorsHeight(int index)
{
	if (index > _monitorCount - 1)
	{
		WarningString("Fail to get monitors with index(Maybe index beyond the limit)");
		return 0;
	}
	return _mnitors.at(index).second;
}

Int32 Monitors::GetMonitorsWidth(int index)
{
	if (index > _monitorCount - 1)
	{
		WarningString("Fail to get monitors with index(Maybe index beyond the limit)");
		return 0;
	}
	return _mnitors.at(index).first;
}

RectInt Monitors::GetMonitorsSize(int index)
{
	if (index > _monitorCount - 1)
	{
		WarningString("Fail to get monitors size index(Maybe index beyond the limit)");
	}
	RectInt rectSize;
	rectSize.width = _mnitors.at(index).first;
	rectSize.height = _mnitors.at(index).second;
	return rectSize;
}


/////////////////////////////////////////////////////////////////////////////////////////////
//Hardware
/////////////////////////////////////////////////////////////////////////////////////////////
Hardware::Hardware(Ambient* ambient)
	:Super(ambient)
{
	_monitors = new Monitors();
}


Hardware::~Hardware()
{
}

}