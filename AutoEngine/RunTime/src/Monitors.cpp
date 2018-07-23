#include "Monitors.h"

AUTO_BEGIN
SINGLETON_INSTANCE(Monitors);
Monitors::~Monitors()
{
	//ppMonitor = nullptr;
}

Monitors::Monitors()
{
	/*ppMonitor = glfwGetMonitors(&monitorCount);
	for (int i = 0; i < monitorCount; i++)
	{
		const GLFWvidmode * mode = glfwGetVideoMode(ppMonitor[i]);
		mnitors.push_back(M_PAIR(mode->width, mode->height));
	}*/
}

Int32 Monitors::GetMonitorsCount()
{
	return monitorCount;
}

Int32 Monitors::GetMonitorsHeightWithIndex(int index)
{
	if (index > monitorCount - 1)
	{
		WarningString("Fail to get monitors with index(Maybe index beyond the limit)");
		return 0;
	}
	return mnitors.at(index).second;
}

Int32 Monitors::GetMonitorsWidthIndex(int index)
{
	if (index > monitorCount-1)
	{
		WarningString("Fail to get monitors with index(Maybe index beyond the limit)");
		return 0;
	}
	return mnitors.at(index).first;
}

Vector2 & Monitors::GetMonitorsSizeIndex(int index)
{
	if (index > monitorCount - 1)
	{
		WarningString("Fail to get monitors size index(Maybe index beyond the limit)");
	}
	Vector2 t(mnitors.at(index).first, mnitors.at(index).second);
	return t;
}

AUTO_END