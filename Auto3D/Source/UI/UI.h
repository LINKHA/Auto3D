#pragma once
#include "../Object/GameManager.h"
#include "UIBatch.h"

namespace Auto3D {

class Graphics;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class ShaderVariation;

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
	void Render(bool renderUICommand = false);

	bool PrepareView();

	bool IsInitialize() { return _initialized; }

private:
	/// Graphics subsystem.
	WeakPtr<Graphics> _graphics;
	/// UI rendering batches.
	Vector<UIBatch> _batches;
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
};


}