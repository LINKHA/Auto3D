#include "TestClassB.h"


RTTR_REGISTRATION
{
	using namespace ns_3d;
	using namespace rttr;
	registration::class_<mesh>("ns_3d::mesh")
		.constructor(&mesh::create_mesh)
		.property("render_mode", &mesh::get_render_mode, &mesh::set_render_mode)
		.enumeration<mesh::render_mode>("ns_3d::render_mode")
		(
			value("POINTS",     mesh::render_mode::POINTS),
			value("WIREFRAME",  mesh::render_mode::WIREFRAME),
			value("SOLID",      mesh::render_mode::SOLID)
		);
}