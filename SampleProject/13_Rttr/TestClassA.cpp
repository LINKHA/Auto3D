#include "TestClassA.h"

AUTO_RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace Ns3D;
	registration::class_<Node>("Ns3D::Node")
		.constructor<FString, Node*>()
		(
			policy::ctor::as_std_shared_ptr, // should create an instance of the class as shared_ptr<ns_3d::node>
			default_arguments(nullptr)       // second argument is optional, so we provide the default value for it
		)
		.property("name", &Node::get_name, &Node::set_name)
		(
			metadata("TOOL_TIP", "Set the name of node.")  // stores metadata associated with this property
		)
		.property("parent", &Ns3D::Node::_parent, registration::private_access)// register directly a member object pointer; mark it as 'private'
		.property_readonly("children", &Node::get_children) // a read-only property; so not set possible
		.method("set_visible", &Node::set_visible)
		(
			default_arguments(true),              // the default value for 'cascade'
			parameter_names("visible", "cascade") // provide the names of the parameter; optional, but might be useful for clients
		)
		.method("render", &Node::render)
		;
}