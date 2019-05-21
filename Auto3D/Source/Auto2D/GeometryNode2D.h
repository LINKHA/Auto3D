#pragma once
#include "../Renderer/GeometryNode.h"

namespace Auto3D
{

class AUTO_API GeometryNode2D : public SpatialNode2D
{
	REGISTER_OBJECT_CLASS(GeometryNode, OctreeNode)

public:
	/// Construct.
	GeometryNode2D();
	/// Destruct.
	~GeometryNode2D();
	/// Register factory and attributes.
	static void RegisterObject();
	/// Set geometry type, which is shared by all geometries.
	void SetGeometryType(GeometryType::Type type);
	/// Set geometry at index.
	void SetGeometry(Geometry* geometry);
	/// Return geometry type.
	GeometryType::Type GetGeometryType() const { return _geometryType; }
	/// Return geometry by index.
	Geometry* GetGeometry() const;
	/// Set texture.
	void SetTexture(Texture* texture);
	/// Return texture.
	Texture* GetTexture() const { return _texture; }
protected:
	/// Geometry type.
	GeometryType::Type _geometryType;
	/// Draw call source datas.
	SharedPtr<Geometry> _geometry;
	/// Texture.
	SharedPtr<Texture> _texture;
};

}