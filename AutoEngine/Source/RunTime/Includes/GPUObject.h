#pragma once
#include "Auto.h"
#include "Graphics.h"
namespace Auto3D {
class Graphics;

/// API-specific GPU object representation
union GPUObjectHandle
{
	/// Object pointer (Direct3D)
	void* ptr;
	/// Object name (OpenGL)
	unsigned name;
};

class GPUObject
{
public:
	/**
	* @brief : Construct with graphics subsystem pointer
	*/
	explicit GPUObject(SharedPtr<Graphics> graphics);
	/**
	* @brief : Destruct. Remove from the Graphics
	*/
	virtual ~GPUObject();

	GPUObjectHandle GetHandle() { return _object; }
protected:
	/// Graphics subsystem
	WeakPtr<Graphics> _graphics;
	/// Object pointer or name.
	GPUObjectHandle _object{};
};

}