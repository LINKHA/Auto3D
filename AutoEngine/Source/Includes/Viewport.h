#pragma once
#include "Object.h"
#include "Rect.h"
namespace Auto3D {

class RenderPath;

class Viewport : public Object
{
	REGISTER_OBJECT_CLASS(Viewport, Object)
public:
	/// Construct with defaults
	explicit Viewport(SharedPtr<Ambient> ambient);

	/// Set view rectangle. A zero rectangle (0 0 0 0) means to use the rendertarget's full dimensions.
	void SetRect(const RectInt& rect);
	/// Set rendering path.
	void SetRenderPath(SharedPtr<RenderPath> renderPath);
private:
	/// Scene pointer.
	//WeakPtr<Scene> _scene;
	/// Camera pointer.
	//WeakPtr<Camera> _camera;
	/// Culling camera pointer.
	//WeakPtr<Camera> _cullCamera;
	/// Viewport rectangle.
	RectInt _rect;
	/// Rendering path.
	SharedPtr<RenderPath> _renderPath;
};

}