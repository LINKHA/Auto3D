#include "Renderer.h"
#include "Camera.h"
#include "Graphics.h"
#include "MSAA.h"
#include "BaseSpace.h"
#include "Transform.h"
#include "GameObject.h"
#include "stl_use.h"
namespace Auto3D {


/////////////////////////////////////////////////////////////////////////////////////////////
//LightContainer
/////////////////////////////////////////////////////////////////////////////////////////////
LightContainer::LightContainer(Ambient* ambient)
	: Super(ambient)
	, _isRenderOrCull(false)
{}
LightContainer::~LightContainer()
{}
void LightContainer::AddLight(Light* source)
{
	Assert(source);
	//Maybe Delay add?
	_lights.push_back(source);
}
void LightContainer::RemoveLight(Light* source)
{
	Assert(source);
	for (Lights::iterator it = _lights.begin(); it != _lights.end(); ++it)
	{
		if (*it = source)
		{
			//Maybe Delay delete?
			_lights.erase(it);
		}
	}
	if (_lastMainLight == source)
		_lastMainLight = nullptr;
}
int LightContainer::Size()
{
	return _lights.size();
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Renderer
/////////////////////////////////////////////////////////////////////////////////////////////
ShadowRenderer::ShadowRenderer(Ambient* ambient)
	: Super(ambient)
{
	_renderer = GetSubSystem<Renderer>();
}
ShadowRenderer::~ShadowRenderer()
{}
void ShadowRenderer::ReadyRender()
{
	_LIST(RenderComponent*) shadowMap = _renderer->_shadowsMap;
	for (_LIST(RenderComponent*)::iterator it = shadowMap.begin(); it != shadowMap.end(); it++)
	{
		//Reday to render
		//(*it)->DrawShadow();
	}
}
void ShadowRenderer::RenderShadow()
{
	_LIST(RenderComponent*) shadowMap = _renderer->_shadowsMap;
	for (_LIST(RenderComponent*)::iterator it = shadowMap.begin(); it != shadowMap.end(); it++)
	{
		(*it)->DrawShadow();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////
//Renderer
/////////////////////////////////////////////////////////////////////////////////////////////
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
			//Rendering path
			renderShadowMap();
			renderOpaques();
			renderCustom();
			renderTranslucent();

			if(cam->GetAllowMSAA())
				cam->GetMSAA()->RenderEnd();
			if (cam->GetAllowPostProcess())
				cam->GetBuffersScreen()->RenderEnd();
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
		if (curComponent && curComponent->GetEnable())
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
		if (curComponent && curComponent->GetEnable())
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
	for (_LIST(RenderComponent*)::iterator it = _shadowsMap.begin(); it != _shadowsMap.end(); it++)
	{
		(*it)->GetGameObject().GetComponent(Transform).UpdateTransform();
		(*it)->DrawShadow();
		(*it)->GetGameObject().GetComponent(Transform).Identity();
	}
}

void Renderer::renderOpaques()
{
	for (_LIST(RenderComponent*)::iterator it = _opaques.begin(); it != _opaques.end(); it++)
	{
		(*it)->GetGameObject().GetComponent(Transform).UpdateTransform();
		(*it)->Draw();
		(*it)->GetGameObject().GetComponent(Transform).Identity();
	}
}

void Renderer::renderCustom()
{
	for (_LIST(RenderComponent*)::iterator it = _customs.begin(); it != _customs.end(); it++)
	{
		(*it)->GetGameObject().GetComponent(Transform).UpdateTransform();
		(*it)->DrawCustom();
		(*it)->GetGameObject().GetComponent(Transform).Identity();
	}
}

void Renderer::translucentGeometrySort()
{
	for (_LIST(RenderComponent*)::iterator i = _translucents.begin(); i != _translucents.end(); i++)
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

void Renderer::renderTranslucent()
{
	translucentGeometrySort();
	for (AUTO_MAP(float, RenderComponent*)::reverse_iterator it = _translucentsSorted.rbegin(); it != _translucentsSorted.rend(); ++it)
	{
		it->second->GetGameObject().GetComponent(Transform).UpdateTransform();
		//Draw translucent component
		it->second->DrawTranslucent();

		it->second->GetGameObject().GetComponent(Transform).Identity();
	}
	_translucentsSorted.clear();
}


}