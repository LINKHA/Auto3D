#pragma once
#include "GameManager.h"
#include "Monitors.h"
namespace Auto3D {

class Monitors;
class IO : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(IO, GlobalGameManager)
public:
	explicit IO(Ambient* ambient);
	Monitors* GetMonitors() { return _monitors; }
private:
	Monitors* _monitors;
};



}

