#pragma once
#include "Core/GameManager.h"
#include "Batch2D.h"

namespace Auto3D {

class Scene2D;
class Camera2D;
class Node2D;

class Graphics;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class ShaderVariation;
struct VertexElement;
class Shader;

/// Texture coordinate index for the instance world matrix.
static const size_t U_INSTANCE_TEXCOORD = 4;

/// Shader constant buffers used by high-level rendering.
namespace ConstantBuffer2D
{
	enum Type
	{
		FRAME = 0,
		OBJECT,
		MATERIAL,
		LIGHTS
	};
};


/// Physics sub system 
class AUTO_API Renderer2D : public BaseModule
{
	REGISTER_OBJECT_CLASS(Renderer2D, BaseModule)
public:
	/// Construct
	Renderer2D();
	/// Destructor
	~Renderer2D();
	/// Render the Renderer2D. If render command is false (default), is assumed to be the default Renderer2D render to backbuffer called by Engine, and will be performed only once. Additional Renderer2D renders to a different rendertarget may be triggered from the renderpath.
	void Render(Scene2D* scene, Camera2D* camera);
	/// Prepare view of objects and batch
	bool PrepareView(Scene2D* scend2d, Camera2D* camera);
	/// Return initialized flag
	bool IsInitialized() { return _initialized; }
	/// Initialize rendering of a new view and collect visible objects from the camera's point of view. Return true on success (scene, camera and octree are non-null.)
	bool Collect2dObjects(Scene2D* scene, Camera2D* camera);
	/// Collect and sort batches from the visible objects. To not go through the objects several times, all the passes should be specified at once instead of multiple calls to CollectBatches().
	void Collect2dBatches();
	/// Render of batchs
	void RenderBatches();
private:
	/// Initialize when screen mode initially set.
	void Initialize();
	/// Render batches from a specific queue and camera.
	void RenderBatches(const Vector<Batch2D>& batches, Camera2D* camera);
	/// Graphics subsystem.
	WeakPtr<Graphics> _graphics;
	/// Renderer2D rendering batches.
	Vector<Batch2D> _batches;
	/// Current scene.
	Scene2D* _scenes;
	/// Current 2d camera.
	Camera2D* _camera;
	/// Geometry nodes
	Vector<GeometryNode2D*> _geometryNode;
	/// Renderer does not have multiple queues
	Batch2DQueue _batchQueue;
	/// Initialized flag.
	bool _initialized;
	/// Flag for Renderer2D already being rendered this frame.
	bool _rendered;
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
	/// Render 2D shaderVariation vs.
	ShaderVariation* _vsv;
	/// Render 2D shaderVariation ps.
	ShaderVariation* _psv;
	/// Instance shaderVariation vs 
	ShaderVariation* _ivsv;
	/// Instance ShaderVariation ps
	ShaderVariation* _ipsv;
};

/// Register Renderer2D related object factories and attributes.
AUTO_API void RegisterRenderer2DLibrary();

}