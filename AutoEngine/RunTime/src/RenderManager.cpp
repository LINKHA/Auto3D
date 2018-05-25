#include "RenderManager.h"

AUTO_BEGIN

SINGLETON_INSTANCE(RenderManager);

RenderManager::RenderManager()
{
}
RenderManager::~RenderManager()
{
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
			m_CurrentCamera = m_Cameras.front(); // ??? maybe better choose next
	}
}


AUTO_END