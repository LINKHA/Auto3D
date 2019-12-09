#pragma once
#include "Adapter/AutoRttr.h"
#include <string.h>

namespace ns_3d
{

class node
{
	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
public:
	node(std::string name, node* parent = nullptr) {}
	virtual ~node() {}
	void set_name(const std::string& name)
	{
		m_name = name;
	}
	const std::string& get_name() const
	{
		return m_name;
	}
	std::vector<node*> get_children() const
	{
		return m_children;
	}
	void set_visible(bool visible, bool cascade = true) {}
	virtual void render() {}
protected:
	node*               m_parent;
	std::string         m_name;
	std::vector<node*>  m_children;
	
};

}