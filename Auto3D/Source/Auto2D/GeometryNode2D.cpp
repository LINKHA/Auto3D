#include "GeometryNode2D.h"
#include "Debug/Log.h"
#include "Graphics/Texture.h"

namespace Auto3D
{

AGeometryNode2D::AGeometryNode2D() :
	_geometryType(EGeometryType::STATIC)
{
	SetFlag(NF_2D_GEOMETRY, true);
}

AGeometryNode2D::~AGeometryNode2D()
{
}

void AGeometryNode2D::RegisterObject()
{
	RegisterFactory<AGeometryNode2D>();
}

void AGeometryNode2D::SetGeometryType(EGeometryType::Type type)
{
	_geometryType = type;
}

void AGeometryNode2D::SetGeometry(FGeometry* geometry)
{
	if (!geometry)
	{
		ErrorString("Can not assign null geometry");
		return;
	}
	_geometry = geometry;

}

FGeometry* AGeometryNode2D::GetGeometry() const
{
	return _geometry;
}

void AGeometryNode2D::SetTexture(ATexture* texture)
{
	_texture = texture;

	SetGeometry(texture->GetGeometry());
}

}