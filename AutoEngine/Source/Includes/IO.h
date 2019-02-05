#pragma once
#include "GameManager.h"
#include "Monitors.h"
#include "EngineInfo.h"


namespace Auto3D {

class Monitors;
class IO : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(IO, GlobalGameManager)
public:
	explicit IO(SharedPtr<Ambient> ambient);
	SharedPtr<Monitors> GetMonitors() { return _monitors; }
	/**
	* @brief : Get engine info
	*/
	SharedPtr<EngineInfo> GetEngineInfo() { return _engineInfo; }
private:
	/// Monitors info
	SharedPtr<Monitors> _monitors;
	/// Engine information
	SharedPtr<EngineInfo> _engineInfo;
};

}

