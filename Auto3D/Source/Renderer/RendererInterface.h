#pragma once
#include "Core/Modules/ModuleInterface.h"

namespace Auto3D
{
/// The public interface of the renderer module.
class AUTO_API IRendererModule : public IModuleInterface
{
public:
	IRendererModule() {}
	virtual ~IRendererModule() {}
};

}