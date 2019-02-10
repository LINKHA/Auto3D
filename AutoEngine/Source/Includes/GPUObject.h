#pragma once
#include "Auto.h"
#include "Graphics.h"
namespace Auto3D {
class Graphics;


/** 
* API-specific GPU object representation
*/
union GPUObjectHandle
{
	/// Object pointer (Direct3D)
	void* ptr;
	/// Object name (OpenGL)
	unsigned name;
};

/**
* Base class for GPU resources
*/
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
	/**
	* @brief : Mark the GPU resource destroyed on graphics context destruction
	*/
	virtual void OnDeviceLost();
	/**
	* @brief : Recreate the GPU resource and restore data if applicable
	*/
	virtual void OnDeviceReset() {}
	/**
	* @brief : Unconditionally release the GPU resource
	*/
	virtual void Release() {}
	/**
	* @brief : Get graphics sub system
	*/
	WeakPtr<Graphics> GetGraphics() const { return _graphics;}
	/** 
	* @brief : Get GPU object handle 
	*/
	GPUObjectHandle GetHandle() { return _object; }
protected:
	/// Graphics subsystem
	WeakPtr<Graphics> _graphics;
	/// Object pointer or name.
	GPUObjectHandle _object{};
};

}