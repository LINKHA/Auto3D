#pragma once
#include "Container/String.h"

namespace Auto3D
{

/// Rendering path command types.
namespace RenderCommandType
{
	enum Type
	{
		NONE = 0,
		CLEAR,
		DRAW_GEOMETRY,
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


/// Description of a pass from the client to the renderer.
struct AUTO_API RenderPassDesc
{
	/// Construct undefined.
	RenderPassDesc()
	{
	}

	/// Construct with parameters.
	RenderPassDesc(const FString& name, RenderCommandSortMode::Type sort = RenderCommandSortMode::STATE, bool lit = true) :
		_name(name),
		_sort(sort),
		_lit(lit)
	{
	}

	/// %Pass name.
	FString _name;
	/// Sorting mode.
	RenderCommandSortMode::Type _sort;
	/// Lighting flag.
	bool _lit;
};

}