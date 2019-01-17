#pragma once
#include "GameManager.h"
#include "LevelScene.h"
#include "AutoSTL.h"

namespace Auto3D {

class Scene : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(Scene, GlobalGameManager)
public:
	explicit Scene(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register level
	*/
	void RegisterScene(SharedPtr<LevelScene> level);
	/**
	* @brief : Add level for level
	*/
	void AddScene(SharedPtr<LevelScene> level);
	/**
	* @brief : Remove level for index
	*/
	void RemoveScene(int index);
	/**
	* @brief : Remove level for level
	*/
	void RemoveScene(SharedPtr<LevelScene> level);
	/**
	* @brief : Get all level scenes
	* @return : HASH_MAP(int,LevelScene*)
	*/
	VECTOR<SharedPtr<LevelScene> >& GetLevelScenes() { return _levelScenes; }
	/**
	* @brief : Get level scene for index
	*/
	SharedPtr<LevelScene> GetLevelScene(int index);
	/**
	* @brief : Run level for mode
	*/
	void ModeRunLevel(RunMode runMode);
	/**
	* @brief : Register scene library objects.
	*/
	void RegisterSceneLib(SharedPtr<Ambient> ambient);
private:
	/**
	* @brief : if not run this function will run once in one frame
	*/
	void delayAddRemoveScene();
private:
	/// action level container
	VECTOR<SharedPtr<LevelScene> > _levelScenes;
	/// dynamic level container
	VECTOR<SharedPtr<LevelScene> > _levelScenesToAdd;
	/// dynamic level container
	VECTOR<SharedPtr<LevelScene> > _levelScenesToRemove;
	/// is inside run
	bool _isInsideRun{};
};

}

