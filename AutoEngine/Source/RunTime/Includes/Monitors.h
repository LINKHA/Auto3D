#pragma once
#include "Auto.h"
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
	int GetMonitorsCount();
	/**
	* @brief : Get monitor index height
	* @return : return Int32
	*/
	int GetMonitorsHeight(int index);
	/**
	* @brief : Get monitor index width
	* @return : return Int32
	*/
	int GetMonitorsWidth(int index);
	/**
	* @brief : Get mouitor index Size
	* @return : return Vector2
	*/
	RectInt GetMonitorsSize(int index);
private:
	int _monitorCount;
	PAIR_VECTOR<int, int> _mnitors;
};

}