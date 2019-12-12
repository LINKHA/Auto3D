#include "TestClassA.h"

RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace Ns3D;
	registration::class_<Node>("Ns3D::Node")
		.constructor<FString, Node*>()
		(
			policy::ctor::as_std_shared_ptr, // should create an instance of the class as shared_ptr<ns_3d::node>
			default_arguments(nullptr)       // second argument is optional, so we provide the default value for it
		)
		.property("name", &Node::GetName, &Node::SetName)
		(
			metadata("TOOL_TIP", "Set the name of node.")  // stores metadata associated with this property
		)
		.property("parent", &Ns3D::Node::_parent, registration::private_access)// register directly a member object pointer; mark it as 'private'
		.property_readonly("children", &Node::GetChildren) // a read-only property; so not set possible
		.property("boolV", &Ns3D::Node::boolV, registration::private_access)
		.property("charV", &Ns3D::Node::charV, registration::private_access)
		.property("unsignedV", &Ns3D::Node::unsignedV, registration::private_access)
		.property("intV", &Ns3D::Node::intV, registration::private_access)
		.property("tVector2IV", &Ns3D::Node::tVector2IV, registration::private_access)
		.property("tRectIV", &Ns3D::Node::tRectIV, registration::private_access)
		.property("floatV", &Ns3D::Node::floatV, registration::private_access)
		.property("tVector2FV", &Ns3D::Node::tVector2FV, registration::private_access)
		.property("fQuaternionV", &Ns3D::Node::fQuaternionV, registration::private_access)
		.property("fColorV", &Ns3D::Node::fColorV, registration::private_access)
		.property("tRectFV", &Ns3D::Node::tRectFV, registration::private_access)
		.method("SetVisible", &Node::SetVisible)
		(
			default_arguments(true),              // the default value for 'cascade'
			parameter_names("visible", "cascade") // provide the names of the parameter; optional, but might be useful for clients
		)
		.method("Render", &Node::Render)
		;
}