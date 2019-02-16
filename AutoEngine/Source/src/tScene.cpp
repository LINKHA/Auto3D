#include "tScene.h"

#include "Camera.h"
#include "SpriteRenderer.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "tLight.h"

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
#include "SkyBox.h"
#include "MeshPBR.h"

#include "NewDef.h"


namespace Auto3D {

tScene::tScene(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
	RegisterSceneLib(_ambient);
}

tScene::~tScene()
{
	_levelScenes.clear();
}

void tScene::RegisterScene(SharedPtr<LevelScene> level)
{
	level->Enable(true);
	AddScene(level);
}

void tScene::AddScene(SharedPtr<LevelScene> level)
{

	Assert(level != NULL);
	if (_isInsideRun)
	{
		VectorFindEarse(_levelScenesToRemove, level);
		_levelScenesToAdd.push_back(level);
		return;
	}
	VectorFindEarse(_levelScenesToAdd, level);
	VectorFindEarse(_levelScenesToRemove, level);
	_levelScenes.push_back(level);
}

void tScene::RemoveScene(SharedPtr<LevelScene> level)
{
	Assert(level != NULL);
	VectorFindEarse(_levelScenesToAdd, level);
	VectorFindEarse(_levelScenesToRemove, level);
	if (_isInsideRun)
	{
		_levelScenesToRemove.push_back(level);
	}
	else
	{
		VectorFindEarse(_levelScenes, level);
	}
}

void tScene::RemoveScene(int sceneId)
{
	for (auto i = _levelScenes.begin(); i != _levelScenes.end(); i++)
	{
		if ((*i)->GetLevelID() == sceneId)
		{
			(*i)->Enable(false);
			RemoveScene(*i);
		}
	}
}

SharedPtr<LevelScene> tScene::GetLevelScene(int index)
{
	for (auto i = _levelScenes.begin(); i != _levelScenes.end(); i++)
	{
		if ((*i)->GetLevelID() == index)
			return *i;
	}
	ErrorString("Fail load level scene");
	return SharedPtr<LevelScene>();
}

void tScene::ModeRunLevel(RunMode runMode)
{
	_isInsideRun = true;

	if (runMode == RunMode::Default)
	{
		ErrorString("Space fail to Run.");
		return;
	}
	for (auto i = _levelScenes.begin(); i != _levelScenes.end(); i++)
	{
		if ((*i) && (*i)->IsEnable())
		{
			if (runMode == RunMode::Awake)
			{
				(*i)->Awake();
				(*i)->ModeRunNode(RunMode::Awake);
			}
			else if (runMode == RunMode::Start)
			{
				(*i)->Start();
				(*i)->ModeRunNode(RunMode::Start);
			}
			else if (runMode == RunMode::Update)
			{
				(*i)->Update();
				(*i)->ModeRunNode(RunMode::Update);
			}
			else if (runMode == RunMode::FixUpdate)
			{
				(*i)->FixUpdate();
				(*i)->ModeRunNode(RunMode::FixUpdate);
			}
			else if (runMode == RunMode::Finish)
			{
				(*i)->Finish();
				(*i)->ModeRunNode(RunMode::Finish);
			}
			else if (runMode == RunMode::Draw)
			{
				(*i)->Draw();
				(*i)->ModeRunNode(RunMode::Draw);
			}
			else
				ErrorString("Level fail to Run.");
		}
	}

	_isInsideRun = false;
	delayAddRemoveScene();
}

void tScene::RegisterSceneLib(SharedPtr<Ambient> ambient)
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
	tLight::RegisterObject(ambient);

	SpriteRenderer::RegisterObject(ambient);
	MeshRenderer::RegisterObject(ambient);
	SkyBox::RegisterObject(ambient);
	MeshPBR::RegisterObject(ambient);
}

void tScene::delayAddRemoveScene()
{
	Assert(!_isInsideRun);
	for (VECTOR<SharedPtr<LevelScene> >::iterator i = _levelScenesToRemove.begin(); i != _levelScenesToRemove.end(); /**/)
	{
		SharedPtr<LevelScene> level = *i;
		++i;
		RemoveScene(level);
	}
	_levelScenesToRemove.clear();
	for (VECTOR<SharedPtr<LevelScene> >::iterator i = _levelScenesToAdd.begin(); i != _levelScenesToAdd.end(); /**/)
	{
		SharedPtr<LevelScene> level = *i;
		++i;
		AddScene(level);
	}
	_levelScenesToAdd.clear();
}

}