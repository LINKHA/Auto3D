#pragma once

#include "Core/GameManager.h"
#include "AutoConfig.h"

namespace Auto3D
{

class AScene;
class ACamera;
class Scene2D;
class Camera2D;
class Canvas;

/// Manages messaging between the user and the engine
class AUTO_API ARegisteredBox : public ABaseModule
{
	REGISTER_OBJECT_CLASS(ARegisteredBox, ABaseModule)
public:
	/// Construct.
	ARegisteredBox();
	/// Destructor
	~ARegisteredBox();
	/// Register scene
	void RegisterScene(AScene* scene) { SetActiveScene(scene);  _scenes.Push(scene); }
	/// Register scene2d
	void RegisterScene2D(Scene2D* scene2d) { SetActiveScene2D(scene2d); _scene2ds.Push(scene2d); }
	/// Register uiScene
	void RegisterCanvas(Canvas* canvas) { SetActiveCanvas(canvas); _canvases.Push(canvas); }
	/// Set active scene.
	void SetActiveScene(AScene* scene);
	/// Set active scene2d.
	void SetActiveScene2D(Scene2D* scene2d);
	/// Set active canvas.
	void SetActiveCanvas(Canvas* canvas);

	/// Return scenes.
	const TVector<AScene*>& GetScenes() { return _scenes; }
	/// Return scene2d.
	const TVector<Scene2D*>& GetScene2Ds() { return _scene2ds; }
	/// Return canvases.
	const TVector<Canvas*>& GetCanvas() { return _canvases; }
	/// Get active scenen.
	AScene* GetActiveScene();
	/// Get active scene2d.
	Scene2D* GetActiveScene2D();
	/// Get active canvas.
	Canvas* GetActiveCanvas();
private:
	/// AScene array.
	TVector<AScene*> _scenes;
	/// Scene2D array. 
	TVector<Scene2D*> _scene2ds;
	/// AUI scene.
	TVector<Canvas*> _canvases;

	/// Active scene.
	AScene* _activeScene;
	/// Active scene2d.
	Scene2D* _activeScene2d;
	/// Active scene.
	Canvas* _activeCanvas;

};

}