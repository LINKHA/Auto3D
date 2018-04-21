#include "Monitors.h"

Monitors* Monitors::ptr_Monitors = nullptr;

Monitors::Monitors()
{
	ppMonitor = glfwGetMonitors(&monitorCount);

	for (int i = 0; i < monitorCount; i++)
	{
		int screen_x, screen_y;

		const GLFWvidmode * mode = glfwGetVideoMode(ppMonitor[i]);
		mnitors.push_back(M_PAIR(mode->width, mode->height));

		std::cout << "Screen size is X = " << mode->width << ", Y = " << mode->height << std::endl;
	}
}
Monitors* Monitors::getInstance()
{
	if (ptr_Monitors == nullptr)
	{
		ptr_Monitors = new Monitors();
	}
	return ptr_Monitors;
}

Int32 Monitors::getMonitorsCount()
{
	return monitorCount;
}

Int32 Monitors::getMonitorsHeightWithIndex(int index)
{
	return mnitors.at(index).second;
}

Int32 Monitors::getMonitorsWidthIndex(int index)
{
	if (index > monitorCount-1)
	{
		WarningString("Fail to get monitors with index(Maybe index beyond the limit)");
		return 0;
	}
	return mnitors.at(index).first;
}
