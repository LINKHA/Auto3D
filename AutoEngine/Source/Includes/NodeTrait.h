#pragma once

#include "Auto.h"
#include "LogDef.h"


namespace Auto3D {

class NodeLayoutCache
{
	friend class NodeLayout;
public:
	/**
	* @brief : Construct
	*/
	NodeLayoutCache();
	/**
	* @brief : Destructor
	*/
	~NodeLayoutCache() = default;
	/**
	* @brief : Get latours count
	*/
	int GetLatoutsCount();
	/**
	* @brief : Add layout 
	*/
	void AddLayout(STRING type);
	/**
	* @brief : Remove layout
	*/
	void RemoveLayout(STRING type);
private:
	/// Layouts
	HASH_SET<STRING> _layouts;
};

class NodeTagCache
{
	friend class NodeTag;
public:
	/**
	* @brief : Construct
	*/
	NodeTagCache();
	/**
	* @brief : Destructor
	*/
	~NodeTagCache() = default;
	/**
	* @brief : Get tags count
	*/
	int GetTagsCount();
	/**
	* @brief : Add layout
	*/
	void AddTag(STRING type);
	/**
	* @brief : Remove layout
	*/
	void RemoveTag(STRING type);
private:
	/// Tags
	HASH_SET<STRING> _tags;
};

class NodeLayout
{
public:
	/**
	* @brief : Construct
	*/
	NodeLayout(STRING type = "Default");
	/**
	* @brief : Destructor
	*/
	~NodeLayout() = default;
	/// Layout string
	STRING layout;
};
class NodeTag
{
public:
	/**
	* @brief : Construct
	*/
	NodeTag(STRING type = "Default");
	/**
	* @brief : Destructor
	*/
	~NodeTag() = default;
	/// Tag string
	STRING tag;
};


}