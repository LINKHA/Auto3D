#pragma once
#include "Adapter/AutoRttr.h"
#include <string.h>

#include "TestClassA.h"

namespace Ns3D
{
class Mesh : public Node
{
	RTTR_ENABLE(Node) // include the names of all direct base classes
public:
	static std::shared_ptr<Mesh> create_mesh(FString file_name)
	{
		return std::shared_ptr<Mesh>(new Mesh(file_name));
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
	Mesh(FString name, Node* parent = nullptr)
		:Node(name,parent)
	{
		_name = name;
		_parent = parent;
	}
	render_mode _mode;
};
}
