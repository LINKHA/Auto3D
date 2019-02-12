#include "Renderer.h"
#include "Camera.h"
#include "Graphics.h"
#include "Transform.h"
#include "Light.h"
#include "RenderPath.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Geometry.h"

#include "NewDef.h"

//Temp
#include "ResourceSystem.h"

namespace Auto3D {

static const float DIR_LIGHT_VERTEX_DATA[] =
{
	-1, 1, 0,
	1, 1, 0,
	1, -1, 0,
	-1, -1, 0,
};

static const unsigned short DIR_LIGHT_INDEX_DATA[] =
{
	0, 1, 2,
	2, 3, 0,
};

static const float SPOT_LIGHT_VERTEX_DATA[] =
{
	0.00001f, 0.00001f, 0.00001f,
	0.00001f, -0.00001f, 0.00001f,
	-0.00001f, -0.00001f, 0.00001f,
	-0.00001f, 0.00001f, 0.00001f,
	1.00000f, 1.00000f, 0.99999f,
	1.00000f, -1.00000f, 0.99999f,
	-1.00000f, -1.00000f, 0.99999f,
	-1.00000f, 1.00000f, 0.99999f,
};

static const unsigned short SPOT_LIGHT_INDEX_DATA[] =
{
	3, 0, 1,
	3, 1, 2,
	0, 4, 5,
	0, 5, 1,
	3, 7, 4,
	3, 4, 0,
	7, 3, 2,
	7, 2, 6,
	6, 2, 1,
	6, 1, 5,
	7, 5, 4,
	7, 6, 5
};

static const float POINT_LIGHT_VERTEX_DATA[] =
{
	-0.423169f, -1.000000f, 0.423169f,
	-0.423169f, -1.000000f, -0.423169f,
	0.423169f, -1.000000f, -0.423169f,
	0.423169f, -1.000000f, 0.423169f,
	0.423169f, 1.000000f, -0.423169f,
	-0.423169f, 1.000000f, -0.423169f,
	-0.423169f, 1.000000f, 0.423169f,
	0.423169f, 1.000000f, 0.423169f,
	-1.000000f, 0.423169f, -0.423169f,
	-1.000000f, -0.423169f, -0.423169f,
	-1.000000f, -0.423169f, 0.423169f,
	-1.000000f, 0.423169f, 0.423169f,
	0.423169f, 0.423169f, -1.000000f,
	0.423169f, -0.423169f, -1.000000f,
	-0.423169f, -0.423169f, -1.000000f,
	-0.423169f, 0.423169f, -1.000000f,
	1.000000f, 0.423169f, 0.423169f,
	1.000000f, -0.423169f, 0.423169f,
	1.000000f, -0.423169f, -0.423169f,
	1.000000f, 0.423169f, -0.423169f,
	0.423169f, -0.423169f, 1.000000f,
	0.423169f, 0.423169f, 1.000000f,
	-0.423169f, 0.423169f, 1.000000f,
	-0.423169f, -0.423169f, 1.000000f
};

static const unsigned short POINT_LIGHT_INDEX_DATA[] =
{
	0, 1, 2,
	0, 2, 3,
	4, 5, 6,
	4, 6, 7,
	8, 9, 10,
	8, 10, 11,
	12, 13, 14,
	12, 14, 15,
	16, 17, 18,
	16, 18, 19,
	20, 21, 22,
	20, 22, 23,
	0, 10, 9,
	0, 9, 1,
	13, 2, 1,
	13, 1, 14,
	23, 0, 3,
	23, 3, 20,
	17, 3, 2,
	17, 2, 18,
	21, 7, 6,
	21, 6, 22,
	7, 16, 19,
	7, 19, 4,
	5, 8, 11,
	5, 11, 6,
	4, 12, 15,
	4, 15, 5,
	22, 11, 10,
	22, 10, 23,
	8, 15, 14,
	8, 14, 9,
	12, 19, 18,
	12, 18, 13,
	16, 21, 20,
	16, 20, 17,
	0, 23, 10,
	1, 9, 14,
	2, 13, 18,
	3, 17, 20,
	6, 11, 22,
	5, 15, 8,
	4, 19, 12,
	7, 21, 16
};

Renderer::Renderer(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
	
}

Renderer::~Renderer()
{
}

void Renderer::Init()
{
	_shadowRenderer = MakeShared<ShadowRenderer>(_ambient);
	_lightContainer = MakeShared<LightContainer>(_ambient);

	auto graphics = GetSubSystem<Graphics>();
	auto cache = GetSubSystem<ResourceSystem>();

	_graphics = graphics;

	_defaultRenderPath = MakeShared<RenderPath>();
	
	createGeometries();
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
	for (LIST<SharedPtr<Camera> >::iterator i = _cameras.begin(); i != _cameras.end(); i++)
	{
		SharedPtr<Camera> cam = *i;
		if (cam && cam->IsEnable())
		{
			_currentCamera = cam;

			RectInt rect = graphics->GetWindowRectInt();
			//Rendering path shadow maps
			renderShadowMap();
			///Render based on camera Rect
			graphics->SetViewport(
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

void Renderer::createGeometries()
{
	//Dir light
	SharedPtr<VertexBuffer> dlvb = MakeShared<VertexBuffer>(_ambient);
	dlvb->SetShadowed(true);
	dlvb->SetSize(4, VERTEX_MASK_POSITION);
	dlvb->SetData(DIR_LIGHT_VERTEX_DATA);

	SharedPtr<IndexBuffer> dlib = MakeShared<IndexBuffer>(_ambient);
	dlib->SetShadowed(true);
	dlib->SetSize(6, false);
	dlib->SetData(DIR_LIGHT_INDEX_DATA);

	_dirLightGeometry = MakeShared<Geometry>(_ambient);
	_dirLightGeometry->SetVertexBuffer(0, dlvb);
	_dirLightGeometry->SetIndexBuffer(dlib);
	_dirLightGeometry->SetDrawRange(PrimitiveType::TringleList, 0, dlib->GetIndexCount());


	// Spot light
	SharedPtr<VertexBuffer> slvb = MakeShared<VertexBuffer>(_ambient);
	slvb->SetShadowed(true);
	slvb->SetSize(8, VERTEX_MASK_POSITION);
	slvb->SetData(SPOT_LIGHT_VERTEX_DATA);

	SharedPtr<IndexBuffer> slib = MakeShared<IndexBuffer>(_ambient);
	slib->SetShadowed(true);
	slib->SetSize(36, false);
	slib->SetData(SPOT_LIGHT_INDEX_DATA);

	_spotLightGeometry = MakeShared<Geometry>(_ambient);
	_spotLightGeometry->SetVertexBuffer(0, slvb);
	_spotLightGeometry->SetIndexBuffer(slib);
	_spotLightGeometry->SetDrawRange(PrimitiveType::TringleList, 0, slib->GetIndexCount());

	
	//Point light
	SharedPtr<VertexBuffer> plvb = MakeShared<VertexBuffer>(_ambient);
	plvb->SetShadowed(true);
	plvb->SetSize(24, VERTEX_MASK_POSITION);
	plvb->SetData(POINT_LIGHT_VERTEX_DATA);

	SharedPtr<IndexBuffer> plib = MakeShared<IndexBuffer>(_ambient);
	plib->SetShadowed(true);
	plib->SetSize(132, false);
	plib->SetData(POINT_LIGHT_INDEX_DATA);

	_pointLightGeometry = MakeShared<Geometry>(_ambient);
	_pointLightGeometry->SetVertexBuffer(0, plvb);
	_pointLightGeometry->SetIndexBuffer(plib);
	_pointLightGeometry->SetDrawRange(PrimitiveType::TringleList, 0, plib->GetIndexCount());

}

void Renderer::AddCamera(SharedPtr<Camera> camera)
{
	Assert(camera != nullptr);
	if (_insideRenderOrCull)
	{
		_camerasToRemove.remove(camera);
		_camerasToAdd.push_back(camera);
		return;
	}
	_camerasToAdd.remove(camera);
	_camerasToRemove.remove(camera);

	_cameras.remove(camera);

	LIST<SharedPtr<Camera> > &queue = _cameras;

	for (LIST<SharedPtr<Camera> >::iterator i = queue.begin(); i != queue.end(); i++)
	{
		SharedPtr<Camera> curCamera = *i;
		if (curCamera && curCamera->GetDepth() > camera->GetDepth())
		{
			queue.insert(i, camera);
			return;
		}
	}
	queue.push_back(camera);
}

void Renderer::RemoveCamera(SharedPtr<Camera> camera)
{
	Assert(camera != nullptr);
	_camerasToAdd.remove(camera);
	_camerasToRemove.remove(camera);

	if (_insideRenderOrCull)
	{
		_camerasToRemove.push_back(camera);
	}
	else
	{
		_cameras.remove(camera);
	}

	SharedPtr<Camera> currentCamera = _currentCamera;
	if (currentCamera == camera)
	{
		if (_cameras.empty())
			_currentCamera = nullptr;
		else
			_currentCamera = _cameras.front();
	}
}

void Renderer::AddShadowMap(SharedPtr<RenderComponent> component)
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

	ShadowMapContainer& queue = _shadowsMap;
	for (ShadowMapContainer::iterator i = queue.begin(); i != queue.end(); i++)
	{
		SharedPtr<RenderComponent> curComponent = *i;
		if (curComponent && curComponent->IsEnable())
		{
			queue.insert(i, component);
			return;
		}
	}
	queue.push_back(component);
}

void Renderer::RemoveShadowMap(SharedPtr<RenderComponent> component)
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

	SharedPtr<RenderComponent> currentShadow = _currentShadow;

	if (currentShadow == component)
	{
		if (_shadowsMap.empty())
			_currentShadow = nullptr;
		else
			_currentShadow = _shadowsMap.front();
	}
}

void Renderer::AddOpaqueGeometry(SharedPtr<RenderComponent> component)
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
		SharedPtr<RenderComponent> curComponent = *i;
		if (curComponent && curComponent->IsEnable())
		{
			queue.insert(i, component);
			return;
		}
	}
	queue.push_back(component);
}

void Renderer::RemoveOpaqueGeometry(SharedPtr<RenderComponent> component)
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

	SharedPtr<RenderComponent> currentOpaques = _currentOpaques;

	if (currentOpaques == component)
	{
		if (_opaques.empty())
			_currentOpaques = nullptr;
		else
			_currentOpaques = _opaques.front();
	}
}

void Renderer::AddCustomGeometry(SharedPtr<RenderComponent> component)
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
		SharedPtr<RenderComponent> curComponent = *i;
		if (curComponent && curComponent->IsEnable())
		{
			queue.insert(i, component);
			return;
		}
	}
	queue.push_back(component);
}

void Renderer::RemoveCustomGeometry(SharedPtr<RenderComponent> component)
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

	SharedPtr<RenderComponent> currentCustom = _currentCustom;

	if (currentCustom == component)
	{
		if (_customs.empty())
			_currentCustom = nullptr;
		else
			_currentCustom = _customs.front();
	}
}

void Renderer::AddTranslucentGeometry(SharedPtr<RenderComponent> component)
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
		SharedPtr<RenderComponent> curComponent = *i;
		if (curComponent && curComponent->IsEnable())
		{
			queue.insert(i, component);
			return;
		}
	}
	queue.push_back(component);
}

void Renderer::RemoveTranslucentGeometry(SharedPtr<RenderComponent> component)
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

	SharedPtr<RenderComponent> currentTranslucent = _currentTranslucent;

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
	for (LIST<SharedPtr<Camera>>::iterator i = _camerasToRemove.begin(); i != _camerasToRemove.end(); /**/)
	{
		SharedPtr<Camera> cam = *i;
		++i; // increment iterator before removing camera; as it changes the list
		RemoveCamera(cam);
	}
	_camerasToRemove.clear();
	for (LIST<SharedPtr<Camera>>::iterator i = _camerasToAdd.begin(); i != _camerasToAdd.end(); /**/)
	{
		SharedPtr<Camera> cam = *i;
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
		SharedPtr<RenderComponent> com = *i;
		++i;
		RemoveShadowMap(com);
	}
	_shadowsMapToRemove.clear();
	for (OpaqueContainer::iterator i = _shadowsMapToAdd.begin(); i != _shadowsMapToAdd.end(); /**/)
	{
		SharedPtr<RenderComponent> com = *i;
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
		SharedPtr<RenderComponent> com = *i;
		++i; 
		RemoveOpaqueGeometry(com);
	}
	_opaquesToRemove.clear();
	for (OpaqueContainer::iterator i = _opaquesToAdd.begin(); i != _opaquesToAdd.end(); /**/)
	{
		SharedPtr<RenderComponent> com = *i;
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
		SharedPtr<RenderComponent> com = *i;
		++i;
		RemoveCustomGeometry(com);
	}
	_opaquesToRemove.clear();
	for (OpaqueContainer::iterator i = _customsToAdd.begin(); i != _customsToAdd.end(); /**/)
	{
		SharedPtr<RenderComponent> com = *i;
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
		SharedPtr<RenderComponent> com = *i;
		++i;
		RemoveOpaqueGeometry(com);
	}
	_translucentsRemove.clear();
	for (OpaqueContainer::iterator i = _translucentsToAdd.begin(); i != _translucentsToAdd.end(); /**/)
	{
		SharedPtr<RenderComponent> com = *i;
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
	for (LIST<SharedPtr<RenderComponent>>::iterator it = _opaques.begin(); it != _opaques.end(); it++)
	{
		(*it)->Draw();
	}
}

void Renderer::renderCustom()
{
	for (LIST<SharedPtr<RenderComponent>>::iterator it = _customs.begin(); it != _customs.end(); it++)
	{
		(*it)->Draw();
	}
}

void Renderer::renderTranslucent()
{
	translucentGeometrySort();
	for (PAIR_MAP<float, SharedPtr<RenderComponent>>::reverse_iterator it = _translucentsSorted.rbegin(); it != _translucentsSorted.rend(); ++it)
	{
		//Draw translucent component
		it->second->Draw();
	}
	_translucentsSorted.clear();
}

void Renderer::translucentGeometrySort()
{
	for (LIST<SharedPtr<RenderComponent>>::iterator i = _translucents.begin(); i != _translucents.end(); i++)
	{
		float distance = glm::length((_currentCamera->GetPosition() - (*i)->GetNode()->GetComponent<Transform>()->GetPosition()).ToGLM());

		_translucentsSorted[distance] = *i;
	}
}


}