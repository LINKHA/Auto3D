#include "AutoConfig.h"
#include "Platform/PlatformDef.h"
#include "Platform/Cmd.h"

namespace Auto3D
{

int cmdGraphics(CmdContext* /*context*/, void* /*userData*/, int argc, char const* const* argv);

int cmdExit(CmdContext* /*context*/, void* /*userData*/, int argc, char const* const* argv);

bool ProcessWindowEvents(WindowState& state, uint32_t& debug, uint32_t& reset);

bool ProcessEvents(uint32_t& width, uint32_t& height, uint32_t& debug, uint32_t& reset, MouseState* mouse);

struct FProcessWindow
{
	static uint32_t _debug;
	static uint32_t _reset;
	static bool _exit;
	static WindowState _window[ENTRY_CONFIG_MAX_WINDOWS];
};

}