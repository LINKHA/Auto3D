#pragma once
#include "AutoConfig.h"
#include "Core/NameTypes.h"

namespace Auto3D
{

class AUTO_API IModuleInterface
{
public:
	/// Even though this is an interface class we need a virtual destructor here because modules are deleted via a pointer to this interface
	virtual ~IModuleInterface() = default;

	/// Called right after the module DLL has been loaded and the module object has been created
	/// Load dependent modules here, and they will be guaranteed to be available during ShutdownModule. ie:
	virtual void StartupModule()
	{
	}

	/// Called before the module is unloaded, right before the module object is destroyed.
	///	During normal shutdown, this is called in reverse order that modules finish StartupModule().
	///	This means that, as long as a module references dependent modules in it's StartupModule(), it
	///	can safely reference those dependencies in ShutdownModule() as well.
	virtual void ShutdownModule()
	{
	}

	/// Called before the module has been unloaded
	virtual void PreUnloadCallback()
	{
	}

	/// Called after the module has been reloaded
	virtual void PostLoadCallback()
	{
	}

	///Override this to set whether your module would like cleanup on application shutdown
	///@return	Whether the module supports shutdown on application exit
	virtual bool SupportsAutomaticShutdown()
	{
		return true;
	}
};

}