#pragma once

#include "Auto.h"
#include "LogDef.h"
#include "Singleton.h"

namespace Auto3D {

class NodeTagCache : public Singleton<NodeTagCache>
{
	friend class NodeTag;
public:
	NodeTagCache()
	{
		_tags.emplace("Default");
		_tags.emplace("Player");
		_tags.emplace("Enemy");
		_tags.emplace("Wall");
		_tags.emplace("Water");
		_tags.emplace("Button");
	}

	~NodeTagCache() = default;

	int GetTagsCount()
	{
		return _tags.size();
	}
	void AddTag(STRING type)
	{
		for (auto it = _tags.begin(); it != _tags.end(); it++)
		{
			if ((*it) == type)
			{
				WarningString("You already have this tag that doesn't do anything");
				return;
			}
		}
		_tags.emplace(type);
	}
	void RemoveTag(STRING type)
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
private:
	HASH_SET<STRING> _tags;
};

class NodeTag
{
public:
	NodeTag(STRING type = "Default")
	{
		auto& tagCache = NodeTagCache::Instance();
		auto it = tagCache._tags.begin();
		for (/**/; it != tagCache._tags.end(); it++)
		{
			if ((*it) == type)
			{
				tag = type;
			}
		}
		if(it== tagCache._tags.end())
			WarningString("Tag can't be found and set Default");
	}
	~NodeTag() = default;

	STRING tag;
};

}