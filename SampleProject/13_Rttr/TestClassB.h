#pragma once
#include "Adapter/AutoRttr.h"
#include <string.h>

#include "TestClassA.h"

namespace ns_3d
{
class mesh : public node
{
	RTTR_ENABLE(node) // include the names of all direct base classes
public:
	static std::shared_ptr<mesh> create_mesh(std::string file_name)
	{
		return std::shared_ptr<mesh>(new mesh(file_name));
	}
	virtual void render() {}
	enum class render_mode
	{
		POINTS,
		WIREFRAME,
		SOLID
	};
	void set_render_mode(render_mode mode)
	{
		_mode = mode;
	}
	render_mode get_render_mode() const
	{
		return _mode;
	}
protected:
	mesh(std::string name, node* parent = nullptr)
		:node(name,parent)
	{
		m_name = name;
		m_parent = parent;
	}
	render_mode _mode;
};
}
