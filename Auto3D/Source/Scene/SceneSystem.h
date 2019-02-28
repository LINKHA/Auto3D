#pragma once

#include "../Object/GameManager.h"
#include "../AutoConfig.h"

namespace Auto3D
{
class Scene;
class Camera;

class AUTO_API SceneSystem : public BaseSubsystem
{
	REGISTER_OBJECT_CLASS(SceneSystem, BaseSubsystem)
public:
	SceneSystem();
	~SceneSystem();
	void RegisterScene(Scene* scene, Camera* camera) { _scene.Push(Pair<Scene*, Camera*>(scene, camera)); }
	
	const Vector<Pair<Scene*, Camera*> >& GetScenes() { return _scene; }
private:
	Vector<Pair<Scene*, Camera*> > _scene;
};

}