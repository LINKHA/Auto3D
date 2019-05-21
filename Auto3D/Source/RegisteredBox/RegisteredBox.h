#pragma once

#include "../Object/GameManager.h"
#include "../AutoConfig.h"

namespace Auto3D
{
class Scene;
class Camera;
class Scene2D;
class Camera2D;

/// Manages messaging between the user and the engine
class AUTO_API RegisteredBox : public BaseSubsystem
{
	REGISTER_OBJECT_CLASS(RegisteredBox, BaseSubsystem)
public:
	/// Construct.
	RegisteredBox();
	/// Destructor
	~RegisteredBox();
	/// Register scene
	void RegisterScene(Scene* scene, Camera* camera) { _scene.Push(Pair<Scene*, Camera*>(scene, camera)); }
	/// Register canvas
	void RegisterCanvas(Scene2D* canvas, Camera2D* camera) { _canvases.Push(Pair<Scene2D*, Camera2D*>(canvas, camera)); }
	/// Return scenes
	const Vector<Pair<Scene*, Camera*> >& GetScenes() { return _scene; }
	/// Return canvases
	const Vector<Pair<Scene2D*, Camera2D*> >& GetCanvases() { return _canvases; }
private:
	/// Scene array
	Vector<Pair<Scene*, Camera*> > _scene;
	/// Scene2D array 
	Vector<Pair<Scene2D*, Camera2D*> > _canvases;
};

}