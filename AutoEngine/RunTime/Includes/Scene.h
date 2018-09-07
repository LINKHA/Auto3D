#pragma once
#include "GameManager.h"
#include "stl_use.h"
#include "RunDefines.h"

namespace Auto3D {
class LevelScene;

const static char* SCENE_ATTACH = "Scene_attach";

class Scene : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(Scene, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Scene);
	using LevelScenes = AUTO_HASH_MAP(int,LevelScene*);
public:
	explicit Scene(Ambient* ambient);
	/**
	* @brief : Register level for index
	*/
	void RegisterLevel(int index,LevelScene* level);
	/**
	* @brief : Remove level for index
	*/
	void RemoveLevel(int index);
	/**
	* @brief : Get all level scenes
	* @return : AUTO_HASH_MAP(int,LevelScene*)
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
private:
	/// dynamic level container
	LevelScenes _dynamicLevelScenes;
	/// action level container
	LevelScenes _actionLevelScenes;
	/// is inside run
	bool _isInsideRun{};
};

}

