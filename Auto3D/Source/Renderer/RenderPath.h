#pragma once

/// Rendering path command types.
namespace RenderCommandType
{
	enum Type
	{
		NONE = 0,
		CLEAR,
		FORWARD_LIGHT,
		DEFERRED_LIGHT,
		RENDER_UI
	};
}

/// Rendering path sorting modes.
namespace RenderCommandSortMode
{
	enum Type
	{
		NONE = 0,
		STATE,
		BACK_TO_FRONT,
		FRONT_TO_BACK,
	};
}