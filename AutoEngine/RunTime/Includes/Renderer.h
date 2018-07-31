#pragma once
#include "GameManager.h"
#include "Camera.h"
namespace Auto3D {
class Ambient;
class Renderer : public LevelGameManager
{
	REGISTER_DERIVED_CLASS(Renderer, LevelGameManager);
	DECLARE_OBJECT_SERIALIZE(Renderer);
	using CameraContainer = _LIST(Camera*);

public:
	explicit Renderer(Ambient* ambient);

	void Render();
	Camera& GetCurrentCamera() { return *_currentCamera; }
	Camera* GetCurrentCameraPtr() { return _currentCamera; }
	void SetCurrentCamera(Camera* c) { _currentCamera = c; }
	CameraContainer& GetAllCamera() { return _cameras; }

	void AddCamera(Camera* c);
	void RemoveCamera(Camera* c);
private:
	void delayedAddRemoveCameras();
private:
	Camera * _currentCamera;
	CameraContainer _cameras;
	CameraContainer _camerasToAdd;
	CameraContainer _camerasToRemove;
	//Run render now
	bool			_insideRenderOrCull;
};

}