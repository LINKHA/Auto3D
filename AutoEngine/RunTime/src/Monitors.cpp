#include "Monitors.h"
#include "SDL2/SDL.h"
namespace Auto3D {
SINGLETON_INSTANCE(Monitors);
Monitors::~Monitors()
{
}

Monitors::Monitors()
{
	SDL_Rect rect;
	monitorCount = SDL_GetNumVideoDisplays();
	Print(SDL_GetNumVideoDisplays());
	for (int i = 0; i < monitorCount; i++)
	{
		SDL_GetDisplayBounds(i, &rect);
		mnitors.push_back(M_PAIR(rect.w, rect.h));
	}
	
	Print(monitorCount);
}

Int32 Monitors::GetMonitorsCount()
{
	return monitorCount;
}

Int32 Monitors::GetMonitorsHeight(int index)
{
	if (index > monitorCount - 1)
	{
		WarningString("Fail to get monitors with index(Maybe index beyond the limit)");
		return 0;
	}
	return mnitors.at(index).second;
}

Int32 Monitors::GetMonitorsWidth(int index)
{
	if (index > monitorCount-1)
	{
		WarningString("Fail to get monitors with index(Maybe index beyond the limit)");
		return 0;
	}
	return mnitors.at(index).first;
}

Vector2 Monitors::GetMonitorsSize(int index)
{
	if (index > monitorCount - 1)
	{
		WarningString("Fail to get monitors size index(Maybe index beyond the limit)");
	}
	Vector2 t(mnitors.at(index).first, mnitors.at(index).second);
	return t;
}

}