#pragma once

#include "Auto.h"
#include "LogDef.h"
#include "Singleton.h"

namespace Auto3D {

class NodeLayoutCache : public Singleton<NodeLayoutCache>
{
	friend class NodeLayout;
public:
	NodeLayoutCache()
	{
		_layouts.emplace("Default");
		_layouts.emplace("BackGround");
		_layouts.emplace("UI");
	}

	~NodeLayoutCache() = default;

	int GetLatoutsCount()
	{
		return _layouts.size();
	}
	void AddLayout(STRING type)
	{
		for (auto it = _layouts.begin(); it != _layouts.end(); it++)
		{
			if ((*it) == type)
			{
				WarningString("You already have this layout that doesn't do anything");
				return;
			}
		}
		_layouts.emplace(type);
	}
	void RemoveLayout(STRING type)
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
private:
	HASH_SET<STRING> _layouts;
};

class NodeLayout
{
public:
	NodeLayout(STRING type = "Default")
	{
		auto& layoutCache = NodeLayoutCache::Instance();
		auto it = layoutCache._layouts.begin();
		for (/**/; it != layoutCache._layouts.end(); it++)
		{
			if ((*it) == type)
			{
				layout = type;
			}
		}
		if (it == layoutCache._layouts.end())
			WarningString("Layout can't be found and set Default");
	}
	
	~NodeLayout() = default;

	STRING layout;
};

}