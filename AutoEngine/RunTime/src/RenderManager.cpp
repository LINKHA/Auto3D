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
	_insideRenderOrCull = true;
	for (CameraContainer::iterator i = _cameras.begin(); i != _cameras.end(); i++)
	{
		Camera* cam = *i;
		if (cam && cam->GetEnable())
		{
			_currentCamera = cam;
			cam->Render();
		}
	}
	_insideRenderOrCull = false;
	DelayedAddRemoveCameras();
}
void RenderManager::AddCamera(Camera * c)
{
	assert(c != NULL);
	if (_insideRenderOrCull)
	{
		_camerasToRemove.remove(c);
		_camerasToAdd.push_back(c);
		return;
	}
	_camerasToAdd.remove(c);
	_camerasToRemove.remove(c);

	_cameras.remove(c);

	CameraContainer &queue = _cameras;

	for (CameraContainer::iterator i = queue.begin(); i != queue.end(); i++)
	{
		Camera* curCamera = *i;
		if (curCamera && curCamera->GetDepth() > c->GetDepth())
		{
			queue.insert(i, c);
			return;
		}
	}
	queue.push_back(c);
}

void RenderManager::RemoveCamera(Camera * c)
{
	assert(c != NULL);
	_camerasToAdd.remove(c);
	_camerasToRemove.remove(c);

	if (_insideRenderOrCull)
	{
		_camerasToRemove.push_back(c);
	}
	else
	{
		_cameras.remove(c);
	}

	Camera* currentCamera = _currentCamera;
	if (currentCamera == c)
	{
		if (_cameras.empty())
			_currentCamera = NULL;
		else
			_currentCamera = _cameras.front(); 
	}
}


///Privete
void RenderManager::DelayedAddRemoveCameras()
{
	DebugAssertIf(_insideRenderOrCull);
	for (CameraContainer::iterator i = _camerasToRemove.begin(); i != _camerasToRemove.end(); /**/)
	{
		Camera* cam = *i;
		++i; // increment iterator before removing camera; as it changes the list
		RemoveCamera(cam);
	}
	_camerasToRemove.clear();
	for (CameraContainer::iterator i = _camerasToAdd.begin(); i != _camerasToAdd.end(); /**/)
	{
		Camera* cam = *i;
		++i; // increment iterator before adding camera; as it changes the list
		AddCamera(cam);
	}
	_camerasToAdd.clear();
}
AUTO_END