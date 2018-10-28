#pragma once
#include "Auto.h"
#include "stl_use.h"
#include "Math/Rect.h"
namespace Auto3D {

class Monitors
{
public:
	Monitors();
	~Monitors() = default;
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
	RectInt GetMonitorsSize(int index);
private:
	Int32 _monitorCount;
	PAIR_VECTOR(int, int) _mnitors;
};

}