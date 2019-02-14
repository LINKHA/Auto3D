#include "Viewport.h"
#include "Renderer.h"

namespace Auto3D {

Viewport::Viewport(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, _rect(RectInt(0.0f, 0.0f, 0.0f, 0.0f))
{
	SetRenderPath(SharedPtr<RenderPath>());
}

Viewport::~Viewport() = default;

void Viewport::SetRect(const RectInt& rect)
{
	_rect = rect;
}

void Viewport::SetRenderPath(SharedPtr<RenderPath> renderPath)
{
	if (renderPath)
		_renderPath = renderPath;
	else
	{
		auto renderer = GetSubSystem<Renderer>();
		if (renderer)
			_renderPath = renderer->GetDefaultRenderPath();
	}
}

}