#pragma once
#include "GameManager.h"
#include "Monitors.h"
namespace Auto3D {

class Monitors;
class IO : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(IO, GlobalGameManager)
public:
	explicit IO(SharedPtr<Ambient> ambient);
	SharedPtr<Monitors> GetMonitors() { return _monitors; }
private:
	SharedPtr<Monitors> _monitors;
};

}

