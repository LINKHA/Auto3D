#pragma once
#include "../Object/GameManager.h"
#include "UIBatch.h"

namespace Auto3D {

class Canvas;
class UICamera;


class Graphics;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class ShaderVariation;
class VertexElement;
class Shader;

/// Physics sub system 
class AUTO_API UI : public BaseSubsystem
{
	REGISTER_OBJECT_CLASS(UI, BaseSubsystem)
public:
	/// Construct
	UI();
	/// Destructor
	~UI();
	/// Initialize when screen mode initially set.
	void Initialize();
	/// Render the UI. If renderUICommand is false (default), is assumed to be the default UI render to backbuffer called by Engine, and will be performed only once. Additional UI renders to a different rendertarget may be triggered from the renderpath.
	void Render(Canvas* scene, UICamera* camera);

	bool PrepareView(Canvas* canvas, UICamera* camera);

	bool IsInitialized() { return _initialized; }

	/// Initialize rendering of a new view and collect visible objects from the camera's point of view. Return true on success (scene, camera and octree are non-null.)
	bool CollectUIObjects(Canvas* scene, UICamera* camera);
	/// Collect and sort batches from the visible objects. To not go through the objects several times, all the passes should be specified at once instead of multiple calls to CollectBatches().
	void CollectUIBatches();

private:
	/// Graphics subsystem.
	WeakPtr<Graphics> _graphics;
	/// UI rendering batches.
	Vector<UIBatch> _batches;

	/// Current scene.
	Canvas* _scene;
	/// Current scene camera.
	UICamera* _camera;


	/// Initialized flag.
	bool _initialized;
	/// Flag for UI already being rendered this frame.
	bool _uiRendered;

	AutoPtr<VertexBuffer> _vb;

	SharedPtr<VertexBuffer> _ivb;

	AutoPtr<IndexBuffer> _ib;

	AutoPtr<ConstantBuffer> _pcb;

	ShaderVariation* _vsv;

	ShaderVariation* _psv;


	AutoPtr<Shader> vs;

	AutoPtr<Shader> ps;
};

/// Register UI related object factories and attributes.
AUTO_API void RegisterUILibrary();

}