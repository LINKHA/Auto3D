#pragma once
#include "BehaviorObject.h"

namespace Auto3D {
/**
* Level Behavior object save level ID
*/
class LevelBehaviorObject : public BehaviorObject
{
	REGISTER_OBJECT_ABSTRACT_CLASS(LevelBehaviorObject, BehaviorObject)
public:
	/**
	* @brief : Construct
	*/
	explicit LevelBehaviorObject(SharedPtr<Ambient> ambient);
	/**
	* @brief : Set level ID
	*/
	void SetLevelID(int levelID) { _levelID = levelID; }
	/**
	* @brief : Return int with scene id
	*/
	const int GetLevelID() const;
protected:
	/// The behavior object must specify a scenario
	int _levelID;
};

}