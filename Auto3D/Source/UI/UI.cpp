#include "UI.h"
#include "../Graphics/Graphics.h"
#include "../Debug/Profiler.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

UI::UI() :
	_graphics(nullptr),
	_initialized(false),
	_uiRendered(false)
{
	RegisterSubsystem(this);

	Initialize();
}

UI::~UI()
{
	RemoveSubsystem(this);
}

void UI::Initialize()
{
	auto* graphics = Subsystem<Graphics>();

	if (!graphics || !graphics->IsInitialized())
		return;
	PROFILE(InitUI);

	_graphics = graphics;


}

void UI::Render(bool renderUICommand)
{
	PROFILE(RenderUI);
	// Perform the default backbuffer render only if not rendered yet, or additional renders through RenderUI command
	if (renderUICommand || !_uiRendered)
	{

	}
	
}

}