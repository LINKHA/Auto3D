#include "Renderer.h"
#include "Camera.h"
#include "Graphics.h"
#include "BaseSpace.h"
#include "Transform.h"
#include "GameObject.h"
#include "Light.h"
#include "stl_use.h"
#include "Configs.h"


//Temp
#include "Resource.h"

namespace Auto3D {


Renderer::Renderer(Ambient* ambient)
	:Super(ambient)
{
	
}

Renderer::~Renderer()
{
}

void Renderer::Init()
{
	intelMoutShadwoRenderer();
	intelMoutLightContainer();
}

void Renderer::ReadyToRender()
{
	_shadowRenderer->ReadyRender();
}

void Renderer::Render()
{
	auto* graphics = GetSubSystem<Graphics>();
	Assert(graphics && graphics->IsInitialized() && !graphics->IsDeviceLost());
	_insideRenderOrCull = true;
	for (CameraContainer::Iterator i = _cameras.Begin(); i != _cameras.End(); i++)
	{
		Camera* cam = *i;
		if (cam && cam->GetEnable())
		{
			_currentCamera = cam;
			

			RectInt rect = GetSubSystem<Graphics>()->GetWindowRectInt();

			GetSubSystem<BaseSpace>()->Draw();
			//Rendering path shadow maps
			renderShadowMap();
			///Render based on camera Rect
			glViewport(
				cam->GetViewRect().x * rect.width,
				cam->GetViewRect().y * rect.height,
				cam->GetViewRect().width * rect.width,
				cam->GetViewRect().height * rect.height
			);
			//Use OffScreen
			if (cam->GetAllowOffScreen())
				cam->GetOffScreen()->RenderStart();
			//Rendering path opaques
			renderOpaques();
			renderCustom();
			renderTranslucent();

			if (cam->GetAllowOffScreen())
				cam->GetOffScreen()->RenderEnd();

		}
	}
	_insideRenderOrCull = false;
	delayedAddRemoveCameras();
	delayedAddRemoveShadowMaps();
	delayedAddRemoveOpaques();
	delayedAddRemoveCustoms();
	delayedAddRemoveTranslucents();
	
}
void Renderer::AddCamera(Camera* c)
{
	Assert(c != nullptr);
	if (_insideRenderOrCull)
	{
		_camerasToRemove.Remove(c);
		_camerasToAdd.PushBack(c);
		return;
	}
	_camerasToAdd.Remove(c);
	_camerasToRemove.Remove(c);

	_cameras.Remove(c);

	CameraContainer &queue = _cameras;

	for (CameraContainer::Iterator i = queue.Begin(); i != queue.End(); i++)
	{
		Camera* curCamera = *i;
		if (curCamera && curCamera->GetDepth() > c->GetDepth())
		{
			queue.Insert(i, c);
			return;
		}
	}
	queue.PushBack(c);
}

void Renderer::RemoveCamera(Camera* c)
{
	Assert(c != nullptr);
	_camerasToAdd.Remove(c);
	_camerasToRemove.Remove(c);

	if (_insideRenderOrCull)
	{
		_camerasToRemove.PushBack(c);
	}
	else
	{
		_cameras.Remove(c);
	}

	Camera* currentCamera = _currentCamera;
	if (currentCamera == c)
	{
		if (_cameras.Empty())
			_currentCamera = nullptr;
		else
			_currentCamera = _cameras.Front();
	}
}

void Renderer::AddShadowMap(RenderComponent* component)
{
	Assert(component != nullptr);
	if (_insideRenderOrCull)
	{
		_shadowsMapToRemove.Remove(component);
		_shadowsMapToAdd.PushBack(component);
		return;
	}
	_shadowsMapToAdd.Remove(component);
	_shadowsMapToRemove.Remove(component);
	_shadowsMap.Remove(component);

	OpaqueContainer& queue = _shadowsMap;
	for (OpaqueContainer::Iterator i = queue.Begin(); i != queue.End(); i++)
	{
		RenderComponent* curComponent = *i;
		if (curComponent && curComponent->GetEnable())
		{
			queue.Insert(i, component);
			return;
		}
	}
	queue.PushBack(component);
}

void Renderer::RemoveShadowMap(RenderComponent* component)
{
	Assert(component != nullptr);
	_shadowsMapToAdd.Remove(component);
	_shadowsMapToRemove.Remove(component);

	if (_insideRenderOrCull)
	{
		_shadowsMapToRemove.PushBack(component);
	}
	else
	{
		_shadowsMap.Remove(component);
	}

	RenderComponent* currentShadow = _currentShadow;

	if (currentShadow == component)
	{
		if (_shadowsMap.Empty())
			_currentShadow = nullptr;
		else
			_currentShadow = _shadowsMap.Front();
	}
}

void Renderer::AddOpaqueGeometry(RenderComponent* component)
{
	Assert(component != nullptr);
	if (_insideRenderOrCull)
	{
		_opaquesToRemove.Remove(component);
		_opaquesToAdd.PushBack(component);
		return;
	}
	_opaquesToAdd.Remove(component);
	_opaquesToRemove.Remove(component);
	_opaques.Remove(component);

	OpaqueContainer& queue = _opaques;
	for (OpaqueContainer::Iterator i = queue.Begin(); i != queue.End(); i++)
	{
		RenderComponent* curComponent = *i;
		if (curComponent && curComponent->GetEnable())
		{
			queue.Insert(i, component);
			return;
		}
	}
	queue.PushBack(component);
}

void Renderer::RemoveOpaqueGeometry(RenderComponent* component)
{
	Assert(component != nullptr);
	_opaquesToAdd.Remove(component);
	_opaquesToRemove.Remove(component);


	if (_insideRenderOrCull)
	{
		_opaquesToRemove.PushBack(component);
	}
	else
	{
		_opaques.Remove(component);
	}

	RenderComponent* currentOpaques = _currentOpaques;

	if (currentOpaques == component)
	{
		if (_opaques.Empty())
			_currentOpaques = nullptr;
		else
			_currentOpaques = _opaques.Front();
	}
}

void Renderer::AddCustomGeometry(RenderComponent* component)
{
	Assert(component != nullptr);
	if (_insideRenderOrCull)
	{
		_customsToRemove.Remove(component);
		_customsToAdd.PushBack(component);
		return;
	}
	_customsToAdd.Remove(component);
	_customsToRemove.Remove(component);
	_customs.Remove(component);

	OpaqueContainer& queue = _customs;
	for (OpaqueContainer::Iterator i = queue.Begin(); i != queue.End(); i++)
	{
		RenderComponent* curComponent = *i;
		if (curComponent && curComponent->GetEnable())
		{
			queue.Insert(i, component);
			return;
		}
	}
	queue.PushBack(component);
}

void Renderer::RemoveCustomGeometry(RenderComponent* component)
{
	Assert(component != nullptr);
	_customsToAdd.Remove(component);
	_customsToRemove.Remove(component);

	if (_insideRenderOrCull)
	{
		_customsToRemove.PushBack(component);
	}
	else
	{
		_customs.Remove(component);
	}

	RenderComponent* currentCustom = _currentCustom;

	if (currentCustom == component)
	{
		if (_customs.Empty())
			_currentCustom = nullptr;
		else
			_currentCustom = _customs.Front();
	}
}

void Renderer::AddTranslucentGeometry(RenderComponent* component)
{
	Assert(component != nullptr);
	if (_insideRenderOrCull)
	{
		_translucentsRemove.Remove(component);
		_translucentsToAdd.PushBack(component);
		return;
	}
	_translucentsToAdd.Remove(component);
	_translucentsRemove.Remove(component);
	_translucents.Remove(component);

	OpaqueContainer& queue = _translucents;
	for (OpaqueContainer::Iterator i = queue.Begin(); i != queue.End(); i++)
	{
		RenderComponent* curComponent = *i;
		if (curComponent && curComponent->GetEnable())
		{
			queue.Insert(i, component);
			return;
		}
	}
	queue.PushBack(component);
}

void Renderer::RemoveTranslucentGeometry(RenderComponent* component)
{
	Assert(component != nullptr);
	_translucentsToAdd.Remove(component);
	_translucentsRemove.Remove(component);

	if (_insideRenderOrCull)
	{
		_translucentsRemove.PushBack(component);
	}
	else
	{
		_translucents.Remove(component);
	}

	RenderComponent* currentTranslucent = _currentTranslucent;

	if (currentTranslucent == component)
	{
		if (_translucents.Empty())
			_currentTranslucent = nullptr;
		else
			_currentTranslucent = _translucents.Front();
	}
}

void Renderer::delayedAddRemoveCameras()
{
	Assert(!_insideRenderOrCull);
	for (CameraContainer::Iterator i = _camerasToRemove.Begin(); i != _camerasToRemove.End(); /**/)
	{
		Camera* cam = *i;
		++i; // increment iterator before removing camera; as it changes the list
		RemoveCamera(cam);
	}
	_camerasToRemove.Clear();
	for (CameraContainer::Iterator i = _camerasToAdd.Begin(); i != _camerasToAdd.End(); /**/)
	{
		Camera* cam = *i;
		++i; // increment iterator before adding camera; as it changes the list
		AddCamera(cam);
	}
	_camerasToAdd.Clear();
}

void Renderer::delayedAddRemoveShadowMaps()
{
	Assert(!_insideRenderOrCull);
	for (OpaqueContainer::Iterator i = _shadowsMapToRemove.Begin(); i != _shadowsMapToRemove.End(); /**/)
	{
		RenderComponent* com = *i;
		++i;
		RemoveShadowMap(com);
	}
	_shadowsMapToRemove.Clear();
	for (OpaqueContainer::Iterator i = _shadowsMapToAdd.Begin(); i != _shadowsMapToAdd.End(); /**/)
	{
		RenderComponent* com = *i;
		++i;
		AddShadowMap(com);
	}
	_shadowsMapToAdd.Clear();
}

void Renderer::delayedAddRemoveOpaques()
{
	Assert(!_insideRenderOrCull);
	for (OpaqueContainer::Iterator i = _opaquesToRemove.Begin(); i != _opaquesToRemove.End(); /**/)
	{
		RenderComponent* com = *i;
		++i; 
		RemoveOpaqueGeometry(com);
	}
	_opaquesToRemove.Clear();
	for (OpaqueContainer::Iterator i = _opaquesToAdd.Begin(); i != _opaquesToAdd.End(); /**/)
	{
		RenderComponent* com = *i;
		++i;
		AddOpaqueGeometry(com);
	}
	_opaquesToAdd.Clear();
}

void Renderer::delayedAddRemoveCustoms()
{
	Assert(!_insideRenderOrCull);
	for (OpaqueContainer::Iterator i = _customsToRemove.Begin(); i != _customsToRemove.End(); /**/)
	{
		RenderComponent* com = *i;
		++i;
		RemoveCustomGeometry(com);
	}
	_opaquesToRemove.Clear();
	for (OpaqueContainer::Iterator i = _customsToAdd.Begin(); i != _customsToAdd.End(); /**/)
	{
		RenderComponent* com = *i;
		++i;
		AddCustomGeometry(com);
	}
	_customsToAdd.Clear();
}

void Renderer::delayedAddRemoveTranslucents()
{
	Assert(!_insideRenderOrCull);
	for (OpaqueContainer::Iterator i = _translucentsRemove.Begin(); i != _translucentsRemove.End(); /**/)
	{
		RenderComponent* com = *i;
		++i;
		RemoveOpaqueGeometry(com);
	}
	_translucentsRemove.Clear();
	for (OpaqueContainer::Iterator i = _translucentsToAdd.Begin(); i != _translucentsToAdd.End(); /**/)
	{
		RenderComponent* com = *i;
		++i;
		AddOpaqueGeometry(com);
	}
	_translucentsToAdd.Clear();
}

void Renderer::renderShadowMap()
{
	_shadowRenderer->RenderShadow();
}

void Renderer::renderOpaques()
{
	for (LIST(RenderComponent*)::Iterator it = _opaques.Begin(); it != _opaques.End(); it++)
	{
		(*it)->Draw();
	}
}

void Renderer::renderCustom()
{
	for (LIST(RenderComponent*)::Iterator it = _customs.Begin(); it != _customs.End(); it++)
	{
		(*it)->DrawCustom();
	}
}

void Renderer::translucentGeometrySort()
{
	for (LIST(RenderComponent*)::Iterator i = _translucents.Begin(); i != _translucents.End(); i++)
	{
		float distance = glm::length(_currentCamera->GetPosition() - (*i)->GetGameObject().GetComponent(Transform).GetPosition().ToGLM());

		_translucentsSorted[distance] = *i;
	}
}

void Renderer::intelMoutShadwoRenderer()
{
	if (_shadowRenderer)
		return;
	_shadowRenderer = new ShadowRenderer(_ambient);
}

void Renderer::intelMoutLightContainer()
{
	if (_lightContainer)
		return;
	_lightContainer = new LightContainer(_ambient);
}

void Renderer::renderTranslucent()
{
	translucentGeometrySort();
	for (HASH_MAP(float, RenderComponent*)::ReverseIterator it = _translucentsSorted.RBegin(); it != _translucentsSorted.REnd(); ++it)
	{
		//Draw translucent component
		it->second->DrawTranslucent();
	}
	_translucentsSorted.Clear();
}


}