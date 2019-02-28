#pragma once
#include "../Object/GameManager.h"
#include "UIBatch.h"

namespace Auto3D {

class Graphics;

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

	bool IsInitialize() { _initialized; }

private:
	/// Graphics subsystem.
	WeakPtr<Graphics> _graphics;
	/// UI rendering batches.
	Vector<UIBatch> _batches;
	/// Initialized flag.
	bool _initialized;
	/// Flag for UI already being rendered this frame.
	bool _uiRendered;
};


}