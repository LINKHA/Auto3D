#include "Renderer.h"
#include "Camera.h"
#include "Graphics.h"
#include "MSAA.h"
#include "BaseSpace.h"
#include "Transform.h"
#include "GameObject.h"
#include "stl_use.h"
namespace Auto3D {


Renderer::Renderer(Ambient* ambient)
	:Super(ambient)
{
	
}
Renderer::~Renderer()
{
}

void Renderer::Render()
{
	auto* graphics = GetSubSystem<Graphics>();
	Assert(graphics && graphics->IsInitialized() && !graphics->IsDeviceLost());
	_insideRenderOrCull = true;
	for (CameraContainer::iterator i = _cameras.begin(); i != _cameras.end(); i++)
	{
		Camera* cam = *i;
		if (cam && cam->GetEnable())
		{
			_currentCamera = cam;
			//Use MSAA
			if (cam->GetAllowMSAA())
				cam->GetMSAA()->RenderStart();
			//Use Post precess
			if (cam->GetAllowPostProcess())
				cam->GetBuffersScreen()->RenderStart();

			RectInt rect = GetSubSystem<Graphics>()->GetWindowRectInt();
			glViewport(
				cam->GetViewRect().x * rect.width,
				cam->GetViewRect().y * rect.height,
				cam->GetViewRect().width * rect.width,
				cam->GetViewRect().height * rect.height
			);
			GetSubSystem<BaseSpace>()->Draw();

			renderTranslucent(cam);

			if(cam->GetAllowMSAA())
				cam->GetBuffersScreen()->RenderEnd();
			if (cam->GetAllowPostProcess())
				cam->GetMSAA()->RenderEnd();
		}
	}
	_insideRenderOrCull = false;
	delayedAddRemoveCameras();
	delayedAddRemoveTranslucents();
}
void Renderer::AddCamera(Camera* c)
{
	Assert(c != NULL);
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

void Renderer::RemoveCamera(Camera* c)
{
	Assert(c != NULL);
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

void Renderer::AddOpaqueGeometry(RenderComponent* component)
{
	Assert(component != NULL);
	if (_insideRenderOrCull)
	{
		_opaquesToRemove.remove(component);
		_opaquesToAdd.push_back(component);
		return;
	}
	_opaquesToAdd.remove(component);
	_opaquesToRemove.remove(component);
	_opaques.remove(component);

	OpaqueContainer& queue = _opaques;
	for (OpaqueContainer::iterator i = queue.begin(); i != queue.end(); i++)
	{
		RenderComponent* curComponent = *i;
		if (curComponent && curComponent->GetEnable())
		{
			queue.insert(i, component);
			return;
		}
	}
	queue.push_back(component);
}

void Renderer::RemoveOpaqueGeometry(RenderComponent* component)
{
	Assert(component != NULL);
	_opaquesToAdd.remove(component);
	_opaquesToRemove.remove(component);

	if (_insideRenderOrCull)
	{
		_opaquesToRemove.push_back(component);
	}
	else
	{
		_opaques.remove(component);
	}

	RenderComponent* currentOpaques = _currentOpaques;

	if (currentOpaques == component)
	{
		if (_opaques.empty())
			_currentOpaques = NULL;
		else
			_currentOpaques = _opaques.front();
	}
}

void Renderer::AddTranslucentGeometry(RenderComponent* component)
{
	Assert(component != NULL);
	if (_insideRenderOrCull)
	{
		_translucentsRemove.remove(component);
		_translucentsToAdd.push_back(component);
		return;
	}
	_translucentsToAdd.remove(component);
	_translucentsRemove.remove(component);
	_translucents.remove(component);

	OpaqueContainer& queue = _translucents;
	for (OpaqueContainer::iterator i = queue.begin(); i != queue.end(); i++)
	{
		RenderComponent* curComponent = *i;
		if (curComponent && curComponent->GetEnable())
		{
			queue.insert(i, component);
			return;
		}
	}
	queue.push_back(component);
}

void Renderer::RemoveTranslucentGeometry(RenderComponent * component)
{
	Assert(component != NULL);
	_translucentsToAdd.remove(component);
	_translucentsRemove.remove(component);

	if (_insideRenderOrCull)
	{
		_translucentsRemove.push_back(component);
	}
	else
	{
		_translucents.remove(component);
	}

	RenderComponent* currentOpaques = _currentOpaques;

	if (currentOpaques == component)
	{
		if (_translucents.empty())
			_currentOpaques = NULL;
		else
			_currentOpaques = _translucents.front();
	}
}

void Renderer::delayedAddRemoveCameras()
{
	Assert(!_insideRenderOrCull);
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
void Renderer::delayedAddRemoveOpaques()
{
	Assert(!_insideRenderOrCull);
	for (OpaqueContainer::iterator i = _opaquesToRemove.begin(); i != _opaquesToRemove.end(); /**/)
	{
		RenderComponent* com = *i;
		++i; 
		RemoveOpaqueGeometry(com);
	}
	_opaquesToRemove.clear();
	for (OpaqueContainer::iterator i = _opaquesToAdd.begin(); i != _opaquesToAdd.end(); /**/)
	{
		RenderComponent* com = *i;
		++i;
		AddOpaqueGeometry(com);
	}
	_opaquesToAdd.clear();
}

void Renderer::delayedAddRemoveTranslucents()
{
	Assert(!_insideRenderOrCull);
	for (OpaqueContainer::iterator i = _translucentsRemove.begin(); i != _translucentsRemove.end(); /**/)
	{
		RenderComponent* com = *i;
		++i;
		RemoveOpaqueGeometry(com);
	}
	_translucentsRemove.clear();
	for (OpaqueContainer::iterator i = _translucentsToAdd.begin(); i != _translucentsToAdd.end(); /**/)
	{
		RenderComponent* com = *i;
		++i;
		AddOpaqueGeometry(com);
	}
	_translucentsToAdd.clear();
}

void Renderer::translucentGeometrySort(Camera* camera)
{
	for (_LIST(RenderComponent*)::iterator i = _translucents.begin(); i != _translucents.end(); i++)
	{
		float distance = glm::length(camera->GetPosition() - (*i)->GetGameObject().GetComponent(Transform).GetPosition().ToGLM());

		_translucentsSorted[distance] = *i;
	}
}

void Renderer::renderTranslucent(Camera* camera)
{
	translucentGeometrySort(camera);
	for (AUTO_MAP(float, RenderComponent*)::reverse_iterator it = _translucentsSorted.rbegin(); it != _translucentsSorted.rend(); ++it)
	{
		it->second->GetGameObject().GetComponent(Transform).UpdateTransform();
		//Draw translucent component
		it->second->DrawTranslucentSprite();
		it->second->GetGameObject().GetComponent(Transform).Identity();
	}
	_translucentsSorted.clear();
}


}