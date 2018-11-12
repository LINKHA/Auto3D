#include "Scene.h"
#include "GameObject.h"
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

void Scene::RegisterScene(int sceneId, LevelScene* scene)
{
	scene->Enable(true);
	_dynamicLevelScenes.emplace(MAKE_PAIR(sceneId, scene));
}

void Scene::RemoveScene(int sceneId)
{
	_dynamicLevelScenes[sceneId]->Enable(false);
	_dynamicLevelScenes.erase(sceneId);
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
	for (auto i = _actionLevelScenes.begin(); i != _actionLevelScenes.end(); i++)
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