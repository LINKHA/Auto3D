#include "Renderer.h"
#include "Camera.h"
#include "Graphics.h"
#include "BaseSpace.h"
#include "Transform.h"
#include "Light.h"
#include "NewDef.h"


//Temp
#include "ResourceSystem.h"

namespace Auto3D {


Renderer::Renderer(SharedPtr<Ambient> ambient)
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
	auto graphics = GetSubSystem<Graphics>();
	Assert(graphics && graphics->IsInitialized() && !graphics->IsDeviceLost());
	_insideRenderOrCull = true;
	for (LIST<Camera*>::iterator i = _cameras.begin(); i != _cameras.end(); i++)
	{
		Camera* cam = *i;
		if (cam && cam->GetEnable())
		{
			_currentCamera = cam;
			

			RectInt rect = GetSubSystem<Graphics>()->GetWindowRectInt();
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
		_camerasToRemove.remove(c);
		_camerasToAdd.push_back(c);
		return;
	}
	_camerasToAdd.remove(c);
	_camerasToRemove.remove(c);

	_cameras.remove(c);

	LIST<Camera*> &queue = _cameras;

	for (LIST<Camera*>::iterator i = queue.begin(); i != queue.end(); i++)
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
	Assert(c != nullptr);
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
			_currentCamera = nullptr;
		else
			_currentCamera = _cameras.front();
	}
}

void Renderer::AddShadowMap(RenderComponent* component)
{
	Assert(component != nullptr);
	if (_insideRenderOrCull)
	{
		_shadowsMapToRemove.remove(component);
		_shadowsMapToAdd.push_back(component);
		return;
	}
	_shadowsMapToAdd.remove(component);
	_shadowsMapToRemove.remove(component);
	_shadowsMap.remove(component);

	OpaqueContainer& queue = _shadowsMap;
	for (OpaqueContainer::iterator i = queue.begin(); i != queue.end(); i++)
	{
		RenderComponent* curComponent = *i;
		if (curComponent && curComponent->IsEnable())
		{
			queue.insert(i, component);
			return;
		}
	}
	queue.push_back(component);
}

void Renderer::RemoveShadowMap(RenderComponent* component)
{
	Assert(component != nullptr);
	_shadowsMapToAdd.remove(component);
	_shadowsMapToRemove.remove(component);

	if (_insideRenderOrCull)
	{
		_shadowsMapToRemove.push_back(component);
	}
	else
	{
		_shadowsMap.remove(component);
	}

	RenderComponent* currentShadow = _currentShadow;

	if (currentShadow == component)
	{
		if (_shadowsMap.empty())
			_currentShadow = nullptr;
		else
			_currentShadow = _shadowsMap.front();
	}
}

void Renderer::AddOpaqueGeometry(RenderComponent* component)
{
	Assert(component != nullptr);
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
		if (curComponent && curComponent->IsEnable())
		{
			queue.insert(i, component);
			return;
		}
	}
	queue.push_back(component);
}

void Renderer::RemoveOpaqueGeometry(RenderComponent* component)
{
	Assert(component != nullptr);
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
			_currentOpaques = nullptr;
		else
			_currentOpaques = _opaques.front();
	}
}

void Renderer::AddCustomGeometry(RenderComponent* component)
{
	Assert(component != nullptr);
	if (_insideRenderOrCull)
	{
		_customsToRemove.remove(component);
		_customsToAdd.push_back(component);
		return;
	}
	_customsToAdd.remove(component);
	_customsToRemove.remove(component);
	_customs.remove(component);

	OpaqueContainer& queue = _customs;
	for (OpaqueContainer::iterator i = queue.begin(); i != queue.end(); i++)
	{
		RenderComponent* curComponent = *i;
		if (curComponent && curComponent->IsEnable())
		{
			queue.insert(i, component);
			return;
		}
	}
	queue.push_back(component);
}

void Renderer::RemoveCustomGeometry(RenderComponent* component)
{
	Assert(component != nullptr);
	_customsToAdd.remove(component);
	_customsToRemove.remove(component);

	if (_insideRenderOrCull)
	{
		_customsToRemove.push_back(component);
	}
	else
	{
		_customs.remove(component);
	}

	RenderComponent* currentCustom = _currentCustom;

	if (currentCustom == component)
	{
		if (_customs.empty())
			_currentCustom = nullptr;
		else
			_currentCustom = _customs.front();
	}
}

void Renderer::AddTranslucentGeometry(RenderComponent* component)
{
	Assert(component != nullptr);
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
		if (curComponent && curComponent->IsEnable())
		{
			queue.insert(i, component);
			return;
		}
	}
	queue.push_back(component);
}

void Renderer::RemoveTranslucentGeometry(RenderComponent * component)
{
	Assert(component != nullptr);
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

	RenderComponent* currentTranslucent = _currentTranslucent;

	if (currentTranslucent == component)
	{
		if (_translucents.empty())
			_currentTranslucent = nullptr;
		else
			_currentTranslucent = _translucents.front();
	}
}

void Renderer::delayedAddRemoveCameras()
{
	Assert(!_insideRenderOrCull);
	for (LIST<Camera*>::iterator i = _camerasToRemove.begin(); i != _camerasToRemove.end(); /**/)
	{
		Camera* cam = *i;
		++i; // increment iterator before removing camera; as it changes the list
		RemoveCamera(cam);
	}
	_camerasToRemove.clear();
	for (LIST<Camera*>::iterator i = _camerasToAdd.begin(); i != _camerasToAdd.end(); /**/)
	{
		Camera* cam = *i;
		++i; // increment iterator before adding camera; as it changes the list
		AddCamera(cam);
	}
	_camerasToAdd.clear();
}

void Renderer::delayedAddRemoveShadowMaps()
{
	Assert(!_insideRenderOrCull);
	for (OpaqueContainer::iterator i = _shadowsMapToRemove.begin(); i != _shadowsMapToRemove.end(); /**/)
	{
		RenderComponent* com = *i;
		++i;
		RemoveShadowMap(com);
	}
	_shadowsMapToRemove.clear();
	for (OpaqueContainer::iterator i = _shadowsMapToAdd.begin(); i != _shadowsMapToAdd.end(); /**/)
	{
		RenderComponent* com = *i;
		++i;
		AddShadowMap(com);
	}
	_shadowsMapToAdd.clear();
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

void Renderer::delayedAddRemoveCustoms()
{
	Assert(!_insideRenderOrCull);
	for (OpaqueContainer::iterator i = _customsToRemove.begin(); i != _customsToRemove.end(); /**/)
	{
		RenderComponent* com = *i;
		++i;
		RemoveCustomGeometry(com);
	}
	_opaquesToRemove.clear();
	for (OpaqueContainer::iterator i = _customsToAdd.begin(); i != _customsToAdd.end(); /**/)
	{
		RenderComponent* com = *i;
		++i;
		AddCustomGeometry(com);
	}
	_customsToAdd.clear();
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

void Renderer::renderShadowMap()
{
	_shadowRenderer->RenderShadow();
}

void Renderer::renderOpaques()
{
	for (LIST<RenderComponent*>::iterator it = _opaques.begin(); it != _opaques.end(); it++)
	{
		(*it)->Draw();
	}
}

void Renderer::renderCustom()
{
	for (LIST<RenderComponent*>::iterator it = _customs.begin(); it != _customs.end(); it++)
	{
		(*it)->Draw();
	}
}

void Renderer::renderTranslucent()
{
	translucentGeometrySort();
	for (PAIR_MAP<float, RenderComponent*>::reverse_iterator it = _translucentsSorted.rbegin(); it != _translucentsSorted.rend(); ++it)
	{
		//Draw translucent component
		it->second->Draw();
	}
	_translucentsSorted.clear();
}

void Renderer::translucentGeometrySort()
{
	for (LIST<RenderComponent*>::iterator i = _translucents.begin(); i != _translucents.end(); i++)
	{
		float distance = glm::length((_currentCamera->GetPosition() - (*i)->GetNode().GetComponent<Transform>()->GetPosition()).ToGLM());

		_translucentsSorted[distance] = *i;
	}
}

void Renderer::intelMoutShadwoRenderer()
{
	if (_shadowRenderer)
		return;
	_shadowRenderer = MakeShared<ShadowRenderer>(_ambient);
}

void Renderer::intelMoutLightContainer()
{
	if (_lightContainer)
		return;
	_lightContainer = MakeShared<LightContainer>(_ambient);
}


}