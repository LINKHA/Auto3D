#pragma once

namespace Auto3D
{

class IModuleInterface
{
public:

	/// Even though this is an interface class we need a virtual destructor here because modules are deleted via a pointer to this interface
	virtual ~IModuleInterface()
	{
	}


	/// Called right after the module DLL has been loaded and the module object has been created
	/// Load dependent modules here, and they will be guaranteed to be available during ShutdownModule. ie:
	virtual void StartupModule()
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


	/// Called before the module is unloaded, right before the module object is destroyed.
	///	During normal shutdown, this is called in reverse order that modules finish StartupModule().
	///	This means that, as long as a module references dependent modules in it's StartupModule(), it
	///	can safely reference those dependencies in ShutdownModule() as well.
		
	virtual void ShutdownModule()
	{
	}

	
	/// Override this to set whether your module is allowed to be unloaded on the fly
	///@return	Whether the module supports shutdown separate from the rest of the engine.
	
	virtual bool SupportsDynamicReloading()
	{
		return true;
	}

	///Override this to set whether your module would like cleanup on application shutdown
	///@return	Whether the module supports shutdown on application exit
	virtual bool SupportsAutomaticShutdown()
	{
		return true;
	}
};

}