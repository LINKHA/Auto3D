#pragma once
#include "Object.h"
#include "Graphics.h"
#include "Renderer.h"
#include "RenderPath.h"

namespace Auto3D {

class Graphics;
class Renderer;
class RenderSurface;
class BatchQueue;

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
	/// Check if a command is enabled and has content to render. To be called only after render update has completed for the frame.
	bool isNecessary(const RenderPathCommand& command);
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
	/// Batch queues by pass index.
	HASH_MAP<unsigned, BatchQueue> _batchQueues;
};

}