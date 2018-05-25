#ifndef RENDER_MANAGER_H_
#define RENDER_MANAGER_H_
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
	AUTO_HASH_MAP(int, Camera*) CameraArray;

	RenderManager();
	void RenderOffscreenCameras();
	void RenderCameras();
	Camera &GetCurrentCamera() {  return *m_CurrentCamera; }
	Camera* GetCurrentCameraPtr() { return m_CurrentCamera; }
	void SetCurrentCamera(Camera *c) { m_CurrentCamera = c; }

	void AddCamera(Camera *c);
	void RemoveCamera(Camera *c);

private:
	Camera*			m_CurrentCamera;
	CameraContainer m_Cameras;
	CameraContainer m_CamerasToAdd;
	CameraContainer m_CamerasToRemove;
	bool			m_InsideRenderOrCull;
};
AUTO_END
#endif //!RENDER_MANAGER_H_