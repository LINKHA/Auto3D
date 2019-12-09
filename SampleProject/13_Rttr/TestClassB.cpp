#include "TestClassB.h"


RTTR_REGISTRATION
{
	using namespace Ns3D;
	using namespace rttr;
	registration::class_<Mesh>("Ns3D::Mesh")
		.constructor(&Mesh::CreateMesh)
		.property("_renderMode", &Mesh::GetRenderMode, &Mesh::SetRenderMode)
		.enumeration<Mesh::RenderMode>("Ns3D::RenderMode")
		(
			value("POINTS", Mesh::RenderMode::POINTS),
			value("WIREFRAME", Mesh::RenderMode::WIREFRAME),
			value("SOLID", Mesh::RenderMode::SOLID)
		);
}