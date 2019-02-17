#pragma once
#include "Object.h"
#include "Graphics.h"
#include "Renderer.h"

namespace Auto3D {

class Graphics;
class Renderer;
class RenderSurface;

class View : public Object
{
	REGISTER_OBJECT_CLASS(View, Object)
public:
	explicit View(SharedPtr<Ambient> ambient);
	/// Render batches.
	void Render();
	/// Return the rendertarget. 0 if using the backbuffer.
	SharedPtr<RenderSurface> GetRenderTarget() const { return _renderTarget; }
private:
	/// Update geometries and sort batches.
	void updateGeometries();
private:
	/// Graphics subsystem.
	WeakPtr<Graphics> _graphics;
	/// Renderer subsystem.
	WeakPtr<Renderer> _renderer;
	/// Shared source view. Null if this view is using its own culling.
	WeakPtr<View> _sourceView;
	/// Destination color rendertarget.
	SharedPtr<RenderSurface> _renderTarget{};
	/// Viewport rectangle.
	Vector4 _viewRect;
	/// Geometries updated flag.
	bool _geometriesUpdated{};
	/// Renderpath.
	SharedPtr<RenderPath> _renderPath{};
};

}