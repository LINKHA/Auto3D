#pragma once

#include "../Object/GameManager.h"
#include "../AutoConfig.h"

namespace Auto3D
{
class Scene;
class Camera;
class Canvas;
class UICamera;

class AUTO_API RegisteredBox : public BaseSubsystem
{
	REGISTER_OBJECT_CLASS(RegisteredBox, BaseSubsystem)
public:
	RegisteredBox();
	~RegisteredBox();
	void RegisterScene(Scene* scene, Camera* camera) { _scene.Push(Pair<Scene*, Camera*>(scene, camera)); }

	void RegisterCanvas(Canvas* canvas, UICamera* camera) { _canvases.Push(Pair<Canvas*, UICamera*>(canvas, camera)); }

	const Vector<Pair<Scene*, Camera*> >& GetScenes() { return _scene; }

	const Vector<Pair<Canvas*, UICamera*> >& GetCanvases() { return _canvases; }

private:
	Vector<Pair<Scene*, Camera*> > _scene;

	Vector<Pair<Canvas*, UICamera*> > _canvases;
};

}