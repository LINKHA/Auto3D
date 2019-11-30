#pragma once
#include "Scene/GeometryNode.h"

namespace Auto3D
{

class AUTO_API GeometryNode2D : public SpatialNode2D
{
	REGISTER_OBJECT_CLASS(AGeometryNode, AOctreeNode)

public:
	/// Construct.
	GeometryNode2D();
	/// Destruct.
	~GeometryNode2D();
	/// Register factory and attributes.
	static void RegisterObject();
	/// Set geometry type, which is shared by all geometries.
	void SetGeometryType(EGeometryType::Type type);
	/// Set geometry at index.
	void SetGeometry(FGeometry* geometry);
	/// Return geometry type.
	EGeometryType::Type GetGeometryType() const { return _geometryType; }
	/// Return geometry by index.
	FGeometry* GetGeometry() const;
	/// Set texture.
	void SetTexture(ATexture* texture);
	/// Return texture.
	ATexture* GetTexture() const { return _texture; }
protected:
	/// FGeometry type.
	EGeometryType::Type _geometryType;
	/// Draw call source datas.
	TSharedPtr<FGeometry> _geometry;
	/// ATexture.
	TSharedPtr<ATexture> _texture;
};

}