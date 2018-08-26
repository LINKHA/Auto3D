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
#include "LoadResource.h"
#include "Resource.h"

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
//ShadowRenderer
/////////////////////////////////////////////////////////////////////////////////////////////
ShadowRenderer::ShadowRenderer(Ambient* ambient)
	: Super(ambient)
	, _shadowMapDepthShader(shader_path + "au_shadow_mapping_depth.auvs"
		, shader_path + "au_shadow_mapping_depth.aufs")
	, _shadowMapPointDepth(shader_path + "au_point_shadows_depth.auvs"
		, shader_path + "au_point_shadows_depth.aufs"
		, shader_path + "au_point_shadows_depth.augs")
{	
}
ShadowRenderer::~ShadowRenderer()
{}
void ShadowRenderer::ReadyRender()
{
	auto* renderer = GetSubSystem<Renderer>();
	_lights = renderer->_lightContainer->GetAllLights();
	_shadowComponents = renderer->GetAllShadowMaps();
	renderer->_lightContainer->IsRender(true);
	//!!! Temp use one
#pragma warning
	for (_VECTOR(Light*)::iterator it = _lights.begin(); it != _lights.end(); it++)
	{
		renderer->_lightContainer->SetCurrentLight(*it);
		//!!!
#pragma warning
		(*it)->GetShadowAssist()->BindDepathMap();
		/////////////////////////////////////////////////////////////////////////////////////////////
		for (_LIST(RenderComponent*)::iterator it = _shadowComponents.begin(); it != _shadowComponents.end(); it++)
		{
			(*it)->DrawReady();
		}
#pragma warning
		//_woodTexture = LocalTextureLoad("../Resource/texture/wood.jpg");
		_woodTexture = GetSubSystem<Resource>()->TextureLoad("../Resource/texture/wood.jpg");
	}
	renderer->_lightContainer->IsRender(false);
}
void ShadowRenderer::RenderShadow()
{
	//!!! Temp use one
#pragma warning
	for (_VECTOR(Light*)::iterator it = _lights.begin(); it != _lights.end(); it++)
	{
		if ((*it)->GetType() == LightType::kDirectional)
		{
			glm::mat4 lightSpaceMatrix = (*it)->GetLightSpaceMatrix();
			glViewport(0, 0, (*it)->GetShadowAssist()->GetShadowWidth(), (*it)->GetShadowAssist()->GetShadowHeight());
			glBindFramebuffer(GL_FRAMEBUFFER, (*it)->GetShadowAssist()->GetDepthMapFBO());
			glClear(GL_DEPTH_BUFFER_BIT);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _woodTexture);

			_shadowMapDepthShader.Use();
			_shadowMapDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
			//Ergodic shadows to Draw shadow
			for (_LIST(RenderComponent*)::iterator it = _shadowComponents.begin(); it != _shadowComponents.end(); it++)
			{
				(*it)->DrawShadow();
			}
		}
		else if ((*it)->GetType() == LightType::kPoint || (*it)->GetType() == LightType::kSpot)
		{

			int shadowWidth = (*it)->GetShadowAssist()->GetShadowWidth();
			int shadowHeight = (*it)->GetShadowAssist()->GetShadowHeight();
			glm::vec3 lightPos = (*it)->GetLightPosition();

			glm::mat4 shadowProj = glm::perspective(90.0f, (float)shadowWidth / (float)shadowHeight, (*it)->GetNearPlane(), (*it)->GetFarPlane());
			std::vector<glm::mat4> shadowTransforms;
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			glViewport(0, 0, shadowWidth, shadowHeight);
			glBindFramebuffer(GL_FRAMEBUFFER, (*it)->GetShadowAssist()->GetDepthMapFBO());
			glClear(GL_DEPTH_BUFFER_BIT);
			_shadowMapPointDepth.Use();
			for (unsigned int i = 0; i < 6; ++i)
				_shadowMapPointDepth.SetMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
			_shadowMapPointDepth.SetFloat("far_plane", (*it)->GetFarPlane());
			_shadowMapPointDepth.SetVec3("lightPos", lightPos);
			//Ergodic shadows to Draw shadow
			for (_LIST(RenderComponent*)::iterator it = _shadowComponents.begin(); it != _shadowComponents.end(); it++)
			{
				(*it)->DrawShadow();
			}
		}
		else
		{
			ErrorString("Fail render depth map");
			continue;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		RectInt t = GetSubSystem<Graphics>()->GetWindowRectInt();
		glViewport(0, 0, t.width, t.height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	for (CameraContainer::iterator i = _cameras.begin(); i != _cameras.end(); i++)
	{
		Camera* cam = *i;
		if (cam && cam->GetEnable())
		{
			_currentCamera = cam;
			

			RectInt rect = GetSubSystem<Graphics>()->GetWindowRectInt();
			glViewport(
				cam->GetViewRect().x * rect.width,
				cam->GetViewRect().y * rect.height,
				cam->GetViewRect().width * rect.width,
				cam->GetViewRect().height * rect.height
			);

			GetSubSystem<BaseSpace>()->Draw();
			//Rendering path shadow maps
			renderShadowMap();

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
	_shadowRenderer->RenderShadow();
}

void Renderer::renderOpaques()
{
	for (_LIST(RenderComponent*)::iterator it = _opaques.begin(); it != _opaques.end(); it++)
	{
		(*it)->Draw();
	}
}

void Renderer::renderCustom()
{
	for (_LIST(RenderComponent*)::iterator it = _customs.begin(); it != _customs.end(); it++)
	{
		(*it)->DrawCustom();
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

void Renderer::intelMoutLightContainer()
{
	if (_lightContainer)
		return;
	_lightContainer = new LightContainer(_ambient);
}

void Renderer::renderTranslucent()
{
	translucentGeometrySort();
	for (AUTO_MAP(float, RenderComponent*)::reverse_iterator it = _translucentsSorted.rbegin(); it != _translucentsSorted.rend(); ++it)
	{
		//Draw translucent component
		it->second->DrawTranslucent();
	}
	_translucentsSorted.clear();
}


}