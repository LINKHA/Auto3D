#include "Auto.h"

namespace Auto3D {

enum class EngineState
{
	Default = 0,
	NotWake,
	Initing,
	Rendering,
	Updateing,
	Exiting,
};

struct EngineInfo
{
	/// Frames per second
	float fps = 0;
	/// Engine frame number
	float frameCount = 0;
	/// Engine state
	EngineState state = EngineState::NotWake;
};

}