#pragma once
#include "Singleton.h"
#include "GameManager.h"
#include "Camera.h"
AUTO_BEGIN
class RenderManager :public LevelGameManager, public Singleton<RenderManager>
{
	REGISTER_DERIVED_ABSTRACT_CLASS(RenderManager, LevelGameManager);
	DECLARE_OBJECT_SERIALIZE(RenderManager);
	using CameraContainer = _LIST(Camera*);
	
public:
	RenderManager();

	void RenderCameras();
	Camera &GetCurrentCamera() {  return *_currentCamera; }
	Camera* GetCurrentCameraPtr() { return _currentCamera; }
	void SetCurrentCamera(Camera *c) { _currentCamera = c; }
	CameraContainer & GetAllCamera() { return _cameras; }

	void AddCamera(Camera *c);
	void RemoveCamera(Camera *c);
private:
	void DelayedAddRemoveCameras();
private:
	Camera*			_currentCamera;
	CameraContainer _cameras;
	CameraContainer _camerasToAdd;
	CameraContainer _camerasToRemove;
	//Run render now
	bool			_insideRenderOrCull;
};
AUTO_END
