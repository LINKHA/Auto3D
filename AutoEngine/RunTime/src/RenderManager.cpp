#include "RenderManager.h"

AUTO_BEGIN

SINGLETON_INSTANCE(RenderManager);

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
}
void RenderManager::RenderCameras()
{
	m_InsideRenderOrCull = true;
	for (CameraContainer::iterator i = m_Cameras.begin(); i != m_Cameras.end(); i++)
	{
		Camera* cam = *i;
		if (cam && cam->GetEnable())
		{
			m_CurrentCamera = cam;
			cam->Render();

		}
	}
	m_InsideRenderOrCull = false;
	DelayedAddRemoveCameras();
}
void RenderManager::AddCamera(Camera * c)
{
	Assert(c != NULL);
	if (m_InsideRenderOrCull)
	{
		m_CamerasToRemove.remove(c);
		m_CamerasToAdd.push_back(c);
		return;
	}
	m_CamerasToAdd.remove(c);
	m_CamerasToRemove.remove(c);

	m_Cameras.remove(c);

	CameraContainer &t_queue = m_Cameras;

	for (CameraContainer::iterator i = t_queue.begin(); i != t_queue.end(); i++)
	{
		Camera* curCamera = *i;
		if (curCamera && curCamera->GetDepth() > c->GetDepth())
		{
			t_queue.insert(i, c);
			return;
		}
	}
	t_queue.push_back(c);
}

void RenderManager::RemoveCamera(Camera * c)
{
	Assert(c != NULL);
	m_CamerasToAdd.remove(c);
	m_CamerasToRemove.remove(c);

	if (m_InsideRenderOrCull)
	{
		m_CamerasToRemove.push_back(c);
	}
	else
	{
		m_Cameras.remove(c);
	}

	Camera* currentCamera = m_CurrentCamera;
	if (currentCamera == c)
	{
		if (m_Cameras.empty())
			m_CurrentCamera = NULL;
		else
			m_CurrentCamera = m_Cameras.front(); 
	}
}


///Privete
void RenderManager::DelayedAddRemoveCameras()
{
	DebugAssertIf(m_InsideRenderOrCull);
	for (CameraContainer::iterator i = m_CamerasToRemove.begin(); i != m_CamerasToRemove.end(); /**/)
	{
		Camera* cam = *i;
		++i; // increment iterator before removing camera; as it changes the list
		RemoveCamera(cam);
	}
	m_CamerasToRemove.clear();
	for (CameraContainer::iterator i = m_CamerasToAdd.begin(); i != m_CamerasToAdd.end(); /**/)
	{
		Camera* cam = *i;
		++i; // increment iterator before adding camera; as it changes the list
		AddCamera(cam);
	}
	m_CamerasToAdd.clear();
}
AUTO_END