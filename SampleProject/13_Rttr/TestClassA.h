#pragma once
#include "Adapter/AutoRttr.h"
#include "Container/String.h"
#include <string.h>

using namespace Auto3D;

namespace ns_3d
{

class node
{
	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
public:
	node(FString name, node* parent = nullptr) {}
	virtual ~node() {}
	void set_name(const FString& name)
	{
		m_name = name;
	}
	const FString& get_name() const
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
	FString         m_name;
	std::vector<node*>  m_children;
	
};

}