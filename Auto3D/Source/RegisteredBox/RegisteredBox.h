#pragma once

#include "../Object/GameManager.h"
#include "../AutoConfig.h"

namespace Auto3D
{

class Scene;
class Camera;
class Scene2D;
class Camera2D;
class Canvas;

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
	void RegisterScene(Scene* scene) { _scenes.Push(scene); }
	/// Register scene2d
	void RegisterScene2D(Scene2D* scene2d) { _scene2ds.Push(scene2d); }
	/// Register uiScene
	void RegisterCanvas(Canvas* canvas) { _canvases.Push(canvas); }
	/// Return scenes
	const Vector<Scene*>& GetScenes() { return _scenes; }
	/// Return scene2d
	const Vector<Scene2D*>& GetScene2D() { return _scene2ds; }
	/// Return canvases
	const Vector<Canvas*>& GetCanvas() { return _canvases; }
private:
	/// Scene array
	Vector<Scene*> _scenes;
	/// Scene2D array 
	Vector<Scene2D*> _scene2ds;
	/// UI scene
	Vector<Canvas*> _canvases;
};

}