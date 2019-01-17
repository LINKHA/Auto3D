#include "NodeTrait.h"

namespace Auto3D {

//////////////////////////////////////////////////////////////////////////
//NodeLayoutCache
//////////////////////////////////////////////////////////////////////////

NodeLayoutCache::NodeLayoutCache()
{
	_layouts.emplace("Default");
	_layouts.emplace("BackGround");
	_layouts.emplace("UI");
}

int NodeLayoutCache::GetLatoutsCount()
{
	return _layouts.size();
}

void NodeLayoutCache::AddLayout(STRING type)
{
	for (auto it = _layouts.begin(); it != _layouts.end(); it++)
	{
		if ((*it) == type)
		{
			return;
		}
	}
	_layouts.emplace(type);
}

void NodeLayoutCache::RemoveLayout(STRING type)
{
	for (auto it = _layouts.begin(); it != _layouts.end(); it++)
	{
		if ((*it) == type)
		{
			_layouts.erase(it);
			return;
		}
	}
	WarningString("Layout can't be found and doesn't do anything");
}
//////////////////////////////////////////////////////////////////////////
//NodeTagCache
//////////////////////////////////////////////////////////////////////////
NodeTagCache::NodeTagCache()
{
	_tags.emplace("Default");
	_tags.emplace("Player");
	_tags.emplace("Enemy");
	_tags.emplace("Wall");
	_tags.emplace("Water");
	_tags.emplace("Button");
}

int NodeTagCache::GetTagsCount()
{
	return _tags.size();
}
void NodeTagCache::AddTag(STRING type)
{
	for (auto it = _tags.begin(); it != _tags.end(); it++)
	{
		if ((*it) == type)
		{
			return;
		}
	}
	_tags.emplace(type);
}
void NodeTagCache::RemoveTag(STRING type)
{
	for (auto it = _tags.begin(); it != _tags.end(); it++)
	{
		if ((*it) == type)
		{
			_tags.erase(it);
			return;
		}
	}
	WarningString("Tag can't be found and doesn't do anything");
}
//////////////////////////////////////////////////////////////////////////
//NodeLayout
//////////////////////////////////////////////////////////////////////////
NodeLayout::NodeLayout(STRING type)
{
	auto layoutCache = Singleton<NodeLayoutCache>::Instance();
	for (auto it = layoutCache._layouts.begin(); it != layoutCache._layouts.end(); it++)
	{
		if ((*it) == type)
		{
			layout = type;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//NodeTag
//////////////////////////////////////////////////////////////////////////
NodeTag::NodeTag(STRING type)
{
	auto tagCache = Singleton<NodeTagCache>::Instance();
	for (auto it = tagCache._tags.begin(); it != tagCache._tags.end(); it++)
	{
		if ((*it) == type)
		{
			tag = type;
		}
	}
}

}