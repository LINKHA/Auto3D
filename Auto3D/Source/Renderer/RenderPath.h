#pragma once
#include "Container/String.h"

namespace Auto3D
{

/// Rendering path command types.
namespace ERenderCommandType
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
namespace ERenderCommandSortMode
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
struct AUTO_API FRenderPassDesc
{
	/// Construct undefined.
	FRenderPassDesc()
	{
	}

	/// Construct with parameters.
	FRenderPassDesc(const FString& name, ERenderCommandSortMode::Type sort = ERenderCommandSortMode::STATE, bool lit = true) :
		_name(name),
		_sort(sort),
		_lit(lit)
	{
	}

	/// %FPass name.
	FString _name;
	/// Sorting mode.
	ERenderCommandSortMode::Type _sort;
	/// Lighting flag.
	bool _lit;
};

}