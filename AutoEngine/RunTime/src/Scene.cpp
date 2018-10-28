#include "Scene.h"
#include "GameObject.h"
#include "LevelScene.h"
#include "Camera.h"
#include "Sprite.h"
#include "Transform.h"
#include "Mesh.h"

namespace Auto3D {

Scene::Scene(Ambient* ambient)
	:Super(ambient)
{
	RegisterSceneLib(_ambient);
}

Scene::~Scene()
{}

void Scene::RegisterLevel(int index, LevelScene* level)
{
	level->Enable(true);
	//_dynamicLevelScenes.Emplace(MAKE_PAIR(index, level));
	_dynamicLevelScenes.Insert(MAKE_PAIR(index, level));
}

void Scene::RemoveLevel(int index)
{
	_dynamicLevelScenes[index]->Enable(false);
	_dynamicLevelScenes.Erase(index);
}

void Scene::ModeRunLevel(RunMode runMode)
{
	_isInsideRun = true;

	_actionLevelScenes = _dynamicLevelScenes;
	if (runMode == RunMode::kDefault)
	{
		ErrorString("Space fail to Run.");
		return;
	}
	for (auto i = _actionLevelScenes.Begin(); i != _actionLevelScenes.End(); i++)
	{
		LevelScene* level = i->second;
		if (level && level->IsEnable())
		{
			if (runMode == RunMode::kAwake)
			{
				level->Awake();
				level->ModeRunNode(RunMode::kAwake);
			}
			else if (runMode == RunMode::kStart)
			{
				level->Start();
				level->ModeRunNode(RunMode::kStart);
			}
			else if (runMode == RunMode::kUpdate)
			{
				level->Update();
				level->ModeRunNode(RunMode::kUpdate);
			}
			else if (runMode == RunMode::kFixUpdate)
			{
				level->FixUpdate();
				level->ModeRunNode(RunMode::kFixUpdate);
			}
			else if (runMode == RunMode::kFinish)
			{
				level->Finish();
				level->ModeRunNode(RunMode::kFinish);
			}
			else if (runMode == RunMode::kDraw)
			{
				level->Draw();
				level->ModeRunNode(RunMode::kDraw);
			}
			else
				ErrorString("Level fail to Run.");
		}
	}

	_isInsideRun = false;

}

void Scene::RegisterSceneLib(Ambient* ambient)
{
	Camera::RegisterObject(ambient);
	Sprite::RegisterObject(ambient);
	Transform::RegisterObject(ambient);
	Mesh::RegisterObject(ambient);
}

}