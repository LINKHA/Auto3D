#ifndef MONITORS_H_
#define MONITORS_H_
#include "Auto.h"
#include "GLStateCache.h"
#include "stl_use.h"
#include "LogAssert.h"
#include <iostream>
class Monitors
{
private:
	
	GLFWmonitor** ppMonitor;
	Int32 monitorCount;
	static Monitors* ptr_Monitors;

	Monitors();
	Monitors(const Monitors&) {}
	Monitors& operator=(const Monitors&) {}
public:
	
	AUTO_VECTOR(int,int) mnitors;
	static Monitors* getInstance();
	Int32 getMonitorsCount();
	Int32 getMonitorsHeightWithIndex(int index);
	Int32 getMonitorsWidthIndex(int index);
};

#endif // !MONITORS_H_