#pragma once

#include "../Object/GameManager.h"
#include "../AutoConfig.h"

namespace Auto3D
{
class Scene;
class Camera;
class Canvas;
class UICamera;

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
	void RegisterCanvas(Canvas* canvas, UICamera* camera) { _canvases.Push(Pair<Canvas*, UICamera*>(canvas, camera)); }
	/// Return scenes
	const Vector<Pair<Scene*, Camera*> >& GetScenes() { return _scene; }
	/// Return canvases
	const Vector<Pair<Canvas*, UICamera*> >& GetCanvases() { return _canvases; }
private:
	/// Scene array
	Vector<Pair<Scene*, Camera*> > _scene;
	/// Canvas array 
	Vector<Pair<Canvas*, UICamera*> > _canvases;
};

}