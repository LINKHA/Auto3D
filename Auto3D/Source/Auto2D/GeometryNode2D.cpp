#include "GeometryNode2D.h"
#include "Debug/Log.h"
#include "Graphics/Texture.h"

namespace Auto3D
{

GeometryNode2D::GeometryNode2D() :
	_geometryType(EGeometryType::STATIC)
{
	SetFlag(NF_2D_GEOMETRY, true);
}

GeometryNode2D::~GeometryNode2D()
{
}

void GeometryNode2D::RegisterObject()
{
	RegisterFactory<GeometryNode2D>();
	CopyBaseAttributes<GeometryNode2D, SpatialNode2D>();
}

void GeometryNode2D::SetGeometryType(EGeometryType::Type type)
{
	_geometryType = type;
}

void GeometryNode2D::SetGeometry(FGeometry* geometry)
{
	if (!geometry)
	{
		ErrorString("Can not assign null geometry");
		return;
	}
	_geometry = geometry;

}

FGeometry* GeometryNode2D::GetGeometry() const
{
	return _geometry;
}

void GeometryNode2D::SetTexture(ATexture* texture)
{
	_texture = texture;

	SetGeometry(texture->GetGeometry());
}

}