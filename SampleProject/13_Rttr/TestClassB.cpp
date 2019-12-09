#include "TestClassB.h"


RTTR_REGISTRATION
{
	using namespace Ns3D;
	using namespace rttr;
	registration::class_<Mesh>("Ns3D::Mesh")
		.constructor(&Mesh::create_mesh)
		.property("render_mode", &Mesh::get_render_mode, &Mesh::set_render_mode)
		.enumeration<Mesh::render_mode>("Ns3D::render_mode")
		(
			value("POINTS", Mesh::render_mode::POINTS),
			value("WIREFRAME", Mesh::render_mode::WIREFRAME),
			value("SOLID", Mesh::render_mode::SOLID)
		);
}