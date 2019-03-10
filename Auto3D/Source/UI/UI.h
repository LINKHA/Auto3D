#pragma once
#include "../Object/GameManager.h"
#include "UIBatch.h"

namespace Auto3D {

class Canvas;
class UICamera;
class UINode;

class Graphics;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class ShaderVariation;
class VertexElement;
class Shader;
/// Texture coordinate index for the instance world matrix.
static const size_t U_INSTANCE_TEXCOORD = 4;

/// Shader constant buffers used by high-level rendering.
struct __UIConstantBuffer
{
	enum _UIConstantBuffer
	{
		FRAME = 0,
		OBJECT,
		MATERIAL,
		LIGHTS
	};
};
using UIConstantBuffer = __UIConstantBuffer::_UIConstantBuffer;

/// Physics sub system 
class AUTO_API UI : public BaseSubsystem
{
	REGISTER_OBJECT_CLASS(UI, BaseSubsystem)
public:
	/// Construct
	UI();
	/// Destructor
	~UI();
	/// Render the UI. If renderUICommand is false (default), is assumed to be the default UI render to backbuffer called by Engine, and will be performed only once. Additional UI renders to a different rendertarget may be triggered from the renderpath.
	void Render(Canvas* scene, UICamera* camera);
	/// Prepare view of objects and batch
	bool PrepareView(Canvas* canvas, UICamera* camera);
	/// Return initialized flag
	bool IsInitialized() { return _initialized; }
	/// Initialize rendering of a new view and collect visible objects from the camera's point of view. Return true on success (scene, camera and octree are non-null.)
	bool CollectUIObjects(Canvas* scene, UICamera* camera);
	/// Collect and sort batches from the visible objects. To not go through the objects several times, all the passes should be specified at once instead of multiple calls to CollectBatches().
	void CollectUIBatches();
	/// Render of batchs
	void RenderBatches();
private:
	/// Initialize when screen mode initially set.
	void Initialize();
	/// Render batches from a specific queue and camera.
	void RenderBatches(const Vector<UIBatch>& batches, UICamera* camera);
	/// Graphics subsystem.
	WeakPtr<Graphics> _graphics;
	/// UI rendering batches.
	Vector<UIBatch> _batches;
	/// Current canvas.
	Canvas* _canvas;
	/// Current ui camera.
	UICamera* _camera;
	/// Geometry nodes
	Vector<UIGeometryNode*> _geometryNode;
	/// UI does not have multiple queues
	UIBatchQueue _batchQueue;
	/// Initialized flag.
	bool _initialized;
	/// Flag for UI already being rendered this frame.
	bool _uiRendered;
	/// Instance vertex buffer dirty flag.
	bool _instanceTransformsDirty;
	/// Instance transforms for uploading to the instance vertex buffer.
	Vector<Matrix3x4F> _instanceTransforms;
	/// Per-frame vertex shader constant buffer.
	SharedPtr<ConstantBuffer> _vsFrameConstantBuffer;
	/// Per-object vertex shader constant buffer.
	SharedPtr<ConstantBuffer> _vsObjectConstantBuffer;
	/// Per-frame pixel shader constant buffer.
	SharedPtr<ConstantBuffer> _psFrameConstantBuffer;
	/// Instance transform vertex buffer.
	AutoPtr<VertexBuffer> _instanceVertexBuffer;
	/// Vertex elements for the instance vertex buffer.
	Vector<VertexElement> _instanceVertexElements;
	/// ShaderVariation vs 
	ShaderVariation* _vsv;
	/// ShaderVariation ps
	ShaderVariation* _psv;
	/// Instance shaderVariation vs 
	ShaderVariation* _ivsv;
	/// Instance ShaderVariation ps
	ShaderVariation* _ipsv;
	/// Shader of vs
	SharedPtr<Shader> vs;
	/// Shader of ps
	SharedPtr<Shader> ps;
};

/// Register UI related object factories and attributes.
AUTO_API void RegisterUILibrary();

}