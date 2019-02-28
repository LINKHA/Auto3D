#pragma once

#include "../Graphics/GraphicsDefs.h"
#include "../IO/ResourceRef.h"
#include "OctreeNode.h"

namespace Auto3D
{

class ConstantBuffer;
class Graphics;
class IndexBuffer;
class Material;
class VertexBuffer;
struct LightList;

/// Geometry types.
enum GeometryType
{
    GEOM_STATIC = 0,
    GEOM_INSTANCED
};

/// Description of geometry to be rendered. %Scene nodes that render the same object can share these to reduce memory load and allow instancing.
struct AUTO_API Geometry : public RefCounted
{
    /// Default-construct.
    Geometry();
    /// Destruct.
    ~Geometry();

    /// Draw using the Graphics subsystem. The constant buffers are not applied automatically, rather they must have been applied beforehand.
    void Draw(Graphics* graphics);
    /// Draw an instance range. A separate instance data vertex buffer must be bound.
    void DrawInstanced(Graphics* graphics, size_t start, size_t count);

    /// %Geometry vertex buffer.
    SharedPtr<VertexBuffer> _vertexBuffer;
    /// %Geometry index buffer.
    SharedPtr<IndexBuffer> _indexBuffer;
    /// Constant buffers.
    SharedPtr<ConstantBuffer> _constantBuffers[MAX_SHADER_STAGES];
    /// %Geometry's primitive type.
    PrimitiveType _primitiveType;
    /// Draw range start. Specifies index start if index buffer defined, vertex start otherwise.
    size_t _drawStart;
    /// Draw range count. Specifies number of indices if index buffer defined, number of vertices otherwise.
    size_t _drawCount;
    /// LOD transition distance.
    float _lodDistance;
};

/// Draw call source data.
struct AUTO_API SourceBatch
{
    /// Construct empty.
    SourceBatch();
    /// Destruct.
    ~SourceBatch();

    /// The geometry to render. Must be non-null.
    SharedPtr<Geometry> _geometry;
    /// The material to use for rendering. Must be non-null.
    SharedPtr<Material> _material;
};

/// Base class for scene nodes that contain geometry to be rendered.
class AUTO_API GeometryNode : public OctreeNode
{
    REGISTER_OBJECT_CLASS(GeometryNode, OctreeNode)

public:
    /// Construct.
    GeometryNode();
    /// Destruct.
    ~GeometryNode();

    /// Register factory and attributes.
    static void RegisterObject();

    /// Prepare object for rendering. Reset framenumber and light list and calculate distance from camera. Called by Renderer.
    void OnPrepareRender(unsigned frameNumber, Camera* camera) override;

    /// Set geometry type, which is shared by all geometries.
    void SetGeometryType(GeometryType type);
    /// Set number of geometries.
    void SetNumGeometries(size_t num);
    /// Set geometry at index.
    void SetGeometry(size_t index, Geometry* geometry);
    /// Set material at every geometry index. Specifying null will use the default material (opaque white.)
    void SetMaterial(Material* material);
    /// Set material at geometry index.
    void SetMaterial(size_t index, Material* material);
    /// Set local space bounding box.
    void SetLocalBoundingBox(const BoundingBox& box);

    /// Return geometry type.
    GeometryType GetGeometryType() const { return _geometryType; }
    /// Return number of geometries.
    size_t GetNumGeometries() const { return _batches.Size(); }
    /// Return geometry by index.
    Geometry* GetGeometry(size_t index) const;
    /// Return material by geometry index.
    Material* GetMaterial(size_t index) const;
    /// Return source information for all draw calls.
    const Vector<SourceBatch>& GetBatches() const { return _batches; }
    /// Return local space bounding box.
    const BoundingBox& GetLocalBoundingBox() const { return _boundingBox; }

    /// Set new light list. Called by Renderer.
    void SetLightList(LightList* list) { _lightList = list; }
    /// Return current light list.
    LightList* GetLightList() const { return _lightList; }

protected:
    /// Recalculate the world space bounding box.
    void OnWorldBoundingBoxUpdate() const override;
    /// Set materials list. Used in serialization.
    void SetMaterialsAttr(const ResourceRefList& materials);
    /// Return materials list. Used in serialization.
    ResourceRefList MaterialsAttr() const;

    /// %Light list for rendering.
    LightList* _lightList;
    /// Geometry type.
    GeometryType _geometryType;
    /// Draw call source datas.
    Vector<SourceBatch> _batches;
    /// Local space bounding box.
    BoundingBox _boundingBox;
};

}
