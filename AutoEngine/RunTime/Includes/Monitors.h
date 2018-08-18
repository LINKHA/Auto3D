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
	Vector2 GetMonitorsSize(int index);
private:
	Int32 monitorCount;
	AUTO_VECTOR(int, int) mnitors;
};
}
