#pragma once
#include "GameManager.h"
#include "SDL2/SDL.h"
#include "Math/Vector2.h"
namespace Auto3D {
struct Monitors
{
	Monitors::Monitors();
	/**
	* @brief : Get moitors count
	*/
	Int32 GetMonitorsCount();
	/**
	* @brief : Get monitor index height
	* @return : return Int32
	*/
	Int32 GetMonitorsHeight(int index);
	/**
	* @brief : Get monitor index width
	* @return : return Int32
	*/
	Int32 GetMonitorsWidth(int index);
	/**
	* @brief : Get mouitor index Size
	* @return : return Vector2
	*/
	Vector2 GetMonitorsSize(int index);
private:
	Int32 monitorCount;
	AUTO_VECTOR(int, int) mnitors;
};


class Hardware : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(Hardware, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Hardware);
public:
	explicit Hardware(Ambient* ambient);
	Monitors& GetMonitors() { return _monitors; }
private:
	Monitors _monitors;
};



}

