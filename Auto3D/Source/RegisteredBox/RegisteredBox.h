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
class AUTO_API RegisteredBox : public BaseModule
{
	REGISTER_OBJECT_CLASS(RegisteredBox, BaseModule)
public:
	/// Construct.
	RegisteredBox();
	/// Destructor
	~RegisteredBox();
	/// Register scene
	void RegisterScene(Scene* scene) { SetActiveScene(scene);  _scenes.Push(scene); }
	/// Register scene2d
	void RegisterScene2D(Scene2D* scene2d) { SetActiveScene2D(scene2d); _scene2ds.Push(scene2d); }
	/// Register uiScene
	void RegisterCanvas(Canvas* canvas) { SetActiveCanvas(canvas); _canvases.Push(canvas); }
	/// Set active scene.
	void SetActiveScene(Scene* scene);
	/// Set active scene2d.
	void SetActiveScene2D(Scene2D* scene2d);
	/// Set active canvas.
	void SetActiveCanvas(Canvas* canvas);

	/// Return scenes.
	const Vector<Scene*>& GetScenes() { return _scenes; }
	/// Return scene2d.
	const Vector<Scene2D*>& GetScene2Ds() { return _scene2ds; }
	/// Return canvases.
	const Vector<Canvas*>& GetCanvas() { return _canvases; }
	/// Get active scenen.
	Scene* GetActiveScene();
	/// Get active scene2d.
	Scene2D* GetActiveScene2D();
	/// Get active canvas.
	Canvas* GetActiveCanvas();
private:
	/// Scene array.
	Vector<Scene*> _scenes;
	/// Scene2D array. 
	Vector<Scene2D*> _scene2ds;
	/// UI scene.
	Vector<Canvas*> _canvases;

	/// Active scene.
	Scene* _activeScene;
	/// Active scene2d.
	Scene2D* _activeScene2d;
	/// Active scene.
	Canvas* _activeCanvas;

};

}