#include "GeometryNode2D.h"
#include "Debug/Log.h"
#include "Graphics/Texture.h"
#include "Auto2D/Sprite2D.h"

namespace Auto3D
{

REGISTER_CLASS(AGeometryNode2D)
{
	REGISTER_CALSS_FACTORY_IMP(AGeometryNode2D)
	.constructor<>()
	;

	// Sprite2D cpp fail,Initializes the global constructor so apply here
	REGISTER_CALSS_FACTORY_IMP(ASprite2D)
	.constructor<>()
	;
}

AGeometryNode2D::AGeometryNode2D() :
	_geometryType(EGeometryType::STATIC)
{
	SetFlag(NF_2D_GEOMETRY, true);
}

AGeometryNode2D::~AGeometryNode2D()
{
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