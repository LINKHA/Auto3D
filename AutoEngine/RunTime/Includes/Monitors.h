#pragma once
#include "Auto.h"
#include "stl_use.h"
#include "LogAssert.h"
#include "Singleton.h"
#include "Math/Vector2.h"
#include "GLGather.h"

namespace Auto3D {
/**
* @brief : Singleton class
*/
class Monitors : public Singleton<Monitors>
{
private:
	Int32 monitorCount;
public:
	Monitors();
	~Monitors();
	AUTO_VECTOR(int,int) mnitors;
	Int32 GetMonitorsCount();
	Int32 GetMonitorsHeight(int index);
	Int32 GetMonitorsWidth(int index);
	Vector2 GetMonitorsSize(int index);

};
}
