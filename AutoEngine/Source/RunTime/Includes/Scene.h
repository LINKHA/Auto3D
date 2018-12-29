#pragma once
#include "GameManager.h"
#include "LevelScene.h"
#include "AutoSTL.h"

namespace Auto3D {

class Scene : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(Scene, GlobalGameManager)

	using LevelScenes = HASH_MAP<int,LevelScene*>;
public:
	explicit Scene(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register level for index
	*/
	void RegisterScene(int index,LevelScene* level);
	/**
	* @brief : Remove level for index
	*/
	void RemoveScene(int index);
	/**
	* @brief : Get all level scenes
	* @return : HASH_MAP(int,LevelScene*)
	*/
	LevelScenes& GetLevelScenes() { return _actionLevelScenes; }
	/**
	* @brief : Get level scene for index
	*/
	LevelScene* GetLevelScene(int index) { return _actionLevelScenes[index]; }
	/**
	* @brief : Run level for mode
	*/
	void ModeRunLevel(RunMode runMode);
	/**
	* @brief : Register scene library objects.
	*/
	void RegisterSceneLib(SharedPtr<Ambient> ambient);
private:
	/// dynamic level container
	LevelScenes _dynamicLevelScenes;
	/// action level container
	LevelScenes _actionLevelScenes;
	/// is inside run
	bool _isInsideRun{};
};

}

