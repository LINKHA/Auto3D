#pragma once
#include "GameManager.h"
#include "Monitors.h"
namespace Auto3D {

class Monitors;
class IO : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(IO, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(IO);
public:
	explicit IO(Ambient* ambient);
	Monitors* GetMonitors() { return _monitors; }
private:
	Monitors* _monitors;
};



}

