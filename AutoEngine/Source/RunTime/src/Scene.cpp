#include "Scene.h"

#include "Camera.h"
#include "SpriteRenderer.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Light.h"

#include "AudioListener.h"
#include "AudioSource.h"

#include "Collider2D.h"
#include "ColliderBox2D.h"
#include "PhysicsWorld2D.h"
#include "RigidBody2D.h"

#include "Collider.h"
#include "ColliderBox.h"
#include "ColliderSphere.h"
#include "PhysicsWorld.h"
#include "RigidBody.h"

#include "NewDef.h"


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
	_dynamicLevelScenes.emplace(MakePair(sceneId, scene));
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
	if (runMode == RunMode::Default)
	{
		ErrorString("Space fail to Run.");
		return;
	}
	for (auto i = _actionLevelScenes.begin(); i != _actionLevelScenes.end(); i++)
	{
		LevelScene* level = i->second;
		if (level && level->IsEnable())
		{
			if (runMode == RunMode::Awake)
			{
				level->Awake();
				level->ModeRunNode(RunMode::Awake);
			}
			else if (runMode == RunMode::Start)
			{
				level->Start();
				level->ModeRunNode(RunMode::Start);
			}
			else if (runMode == RunMode::Update)
			{
				level->Update();
				level->ModeRunNode(RunMode::Update);
			}
			else if (runMode == RunMode::FixUpdate)
			{
				level->FixUpdate();
				level->ModeRunNode(RunMode::FixUpdate);
			}
			else if (runMode == RunMode::Finish)
			{
				level->Finish();
				level->ModeRunNode(RunMode::Finish);
			}
			else if (runMode == RunMode::Draw)
			{
				level->Draw();
				level->ModeRunNode(RunMode::Draw);
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
	
	Transform::RegisterObject(ambient);

	Collider2D::RegisterObject(ambient);
	ColliderBox2D::RegisterObject(ambient);
	PhysicsWorld2D::RegisterObject(ambient);
	RigidBody2D::RegisterObject(ambient);

	Collider::RegisterObject(ambient);
	ColliderBox::RegisterObject(ambient);
	ColliderSphere::RegisterObject(ambient);
	PhysicsWorld::RegisterObject(ambient);
	RigidBody::RegisterObject(ambient);

	AudioListener::RegisterObject(ambient);
	AudioSource::RegisterObject(ambient);
	Light::RegisterObject(ambient);

	SpriteRenderer::RegisterObject(ambient);
	MeshRenderer::RegisterObject(ambient);

}

}