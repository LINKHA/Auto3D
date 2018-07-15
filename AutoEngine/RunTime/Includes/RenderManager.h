#pragma once
#include "Singleton.h"
#include "GameManager.h"
#include "Camera.h"
AUTO_BEGIN
class RenderManager :public LevelGameManager, public Singleton<RenderManager>
{
	REGISTER_DERIVED_ABSTRACT_CLASS(RenderManager, LevelGameManager);
	DECLARE_OBJECT_SERIALIZE(RenderManager);
	typedef _LIST(Camera*) CameraContainer;
	
public:
	RenderManager();

	void RenderCameras();
	Camera &GetCurrentCamera() {  return *m_CurrentCamera; }
	Camera* GetCurrentCameraPtr() { return m_CurrentCamera; }
	void SetCurrentCamera(Camera *c) { m_CurrentCamera = c; }

	void AddCamera(Camera *c);
	void RemoveCamera(Camera *c);
private:
	void DelayedAddRemoveCameras();
private:
	Camera*			m_CurrentCamera;
	CameraContainer m_Cameras;
	CameraContainer m_CamerasToAdd;
	CameraContainer m_CamerasToRemove;
	//Run render now
	bool			m_InsideRenderOrCull;
};
AUTO_END
