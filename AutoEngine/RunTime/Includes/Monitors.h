#ifndef MONITORS_H_
#define MONITORS_H_
#include "Auto.h"
#include "GLStateCache.h"
#include "stl_use.h"
#include "LogAssert.h"
#include "Singleton.h"
AUTO_BEGIN
/**
* @brief : Singleton class
*/
class Monitors : public Singleton<Monitors>
{
private:
	GLFWmonitor** ppMonitor;
	Int32 monitorCount;

public:
	Monitors();
	~Monitors();
	AUTO_VECTOR(int,int) mnitors;
	Int32 GetMonitorsCount();
	Int32 GetMonitorsHeightWithIndex(int index);
	Int32 GetMonitorsWidthIndex(int index);


};
AUTO_END
#endif // !MONITORS_H_