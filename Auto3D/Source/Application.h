#pragma once
#include "AutoConfig.h"
#include "Container/String.h"

#include <memory>

namespace Auto3D
{

class AEngine;
/// The superclass implementation of the project space, where the engine is implemented
class AUTO_API FApplication
{
public:
	FApplication();
	~FApplication();

	/// This is AEngine important funcation init awake runloop and finish run
	void Run();
	/// Show an error message (last log message if empty), terminate the main loop, and set failure exit code.
	void ErrorExit(const FString& message = FString::EMPTY);

private:
	/// Auto3D AEngine
	//std::unique_ptr<AEngine> _engine;

};

}