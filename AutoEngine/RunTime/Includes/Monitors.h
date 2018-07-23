#pragma once
#include "Auto.h"
#include "stl_use.h"
#include "LogAssert.h"
#include "Singleton.h"
#include "Math/Vector2.h"
#include "OpenGLGather.h"
USING_MATH
AUTO_BEGIN
/**
* @brief : Singleton class
*/
class Monitors : public Singleton<Monitors>
{
private:
	//GLFWmonitor** ppMonitor;
	Int32 monitorCount;

public:
	Monitors();
	~Monitors();
	AUTO_VECTOR(int,int) mnitors;
	Int32 GetMonitorsCount();
	Int32 GetMonitorsHeightWithIndex(int index);
	Int32 GetMonitorsWidthIndex(int index);
	Vector2& GetMonitorsSizeIndex(int index);

};
AUTO_END
