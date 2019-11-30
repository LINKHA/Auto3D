#pragma once

#include "Graphics/GraphicsDefs.h"
#include "IO/ResourceRef.h"
#include "OctreeNode.h"
#include "Auto2D/SpatialNode2D.h"

namespace Auto3D
{

class FConstantBuffer;
class AGraphics;
class FIndexBuffer;
class AMaterial;
class FVertexBuffer;
struct LightList;
class ATexture;

/// Geometry types.
namespace GeometryType
{
	enum Type
	{
		STATIC = 0,
		INSTANCED
	};
};


/// Load-time description of a vertex buffer, to be uploaded on the GPU later.
struct AUTO_API VertexBufferDesc
{
	/// Vertex declaration.
	TVector<FVertexElement> _vertexElements;
	/// Number of vertices.
	size_t _numVertices;
	/// Vertex data.
	TSharedArrayPtr<unsigned char> _vertexData;
};

/// Load-time description of an index buffer, to be uploaded on the GPU later.
struct AUTO_API IndexBufferDesc
{
	/// Index _size.
	size_t _indexSize;
	/// Number of indices.
	size_t _numIndices;
	/// Index data.
	TSharedArrayPtr<unsigned char> _indexData;
};

/// Load-time description of a geometry.
struct AUTO_API GeometryDesc
{
	/// LOD distance.
	float _lodDistance;
	/// Primitive type.
	EPrimitiveType::Type _primitiveType;
	/// Vertex buffer ref.
	unsigned _vbRef;
	/// Index buffer ref.
	unsigned _ibRef;
	/// Draw range start.
	unsigned _drawStart;
	/// Draw range element count.
	unsigned _drawCount;
};

/// Description of geometry to be rendered. %Scene nodes that render the same object can share these to reduce memory load and allow instancing.
class AUTO_API Geometry : public FRefCounted
{
public:
    /// Default-construct.
    Geometry();
    /// Destruct.
    ~Geometry();

    /// Draw using the AGraphics subsystem. The constant buffers are not applied automatically, rather they must have been applied beforehand.
    void Draw(AGraphics* graphics);
    /// Draw an instance range. A separate instance data vertex buffer must be bound.
    void DrawInstanced(AGraphics* graphics, size_t start, size_t count);

    /// %Geometry vertex buffer.
    TSharedPtr<FVertexBuffer> _vertexBuffer;
    /// %Geometry index buffer.
    TSharedPtr<FIndexBuffer> _indexBuffer;
    /// FConstant buffers.
    TSharedPtr<FConstantBuffer> _constantBuffers[EShaderStage::Count];
    /// %Geometry's primitive type.
    EPrimitiveType::Type _primitiveType;
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
    TSharedPtr<Geometry> _geometry;
    /// The material to use for rendering. Must be non-null.
    TSharedPtr<AMaterial> _material;
};

/// Base class for scene nodes that contain geometry to be rendered.
class AUTO_API AGeometryNode : public AOctreeNode
{
    REGISTER_OBJECT_CLASS(AGeometryNode, AOctreeNode)

public:
    /// Construct.
    AGeometryNode();
    /// Destruct.
    ~AGeometryNode();

    /// Register factory and attributes.
    static void RegisterObject();

    /// Prepare object for rendering. Reset framenumber and light list and calculate distance from camera. Called by Renderer.
    void OnPrepareRender(unsigned frameNumber, ACamera* camera) override;

    /// Set geometry type, which is shared by all geometries.
    void SetGeometryType(GeometryType::Type type);
    /// Set number of geometries.
    void SetNumGeometries(size_t num);
    /// Set geometry at index.
    void SetGeometry(size_t index, Geometry* geometry);
    /// Set material at every geometry index. Specifying null will use the default material (opaque white.)
    void SetMaterial(AMaterial* material);
    /// Set material at geometry index.
    void SetMaterial(size_t index, AMaterial* material);
    /// Set local space bounding box.
    void SetLocalBoundingBox(const TBoundingBoxF& box);

    /// Return geometry type.
    GeometryType::Type GetGeometryType() const { return _geometryType; }
    /// Return number of geometries.
    size_t GetNumGeometries() const { return _batches.Size(); }
    /// Return geometry by index.
    Geometry* GetGeometry(size_t index) const;
    /// Return material by geometry index.
    AMaterial* GetMaterial(size_t index) const;
    /// Return source information for all draw calls.
    const TVector<SourceBatch>& GetBatches() const { return _batches; }
    /// Return local space bounding box.
    const TBoundingBoxF& GetLocalBoundingBox() const { return _boundingBox; }

    /// Set new light list. Called by Renderer.
    void SetLightList(LightList* list) { _lightList = list; }
    /// Return current light list.
    LightList* GetLightList() const { return _lightList; }

protected:
    /// Recalculate the world space bounding box.
    void OnWorldBoundingBoxUpdate() const override;
    /// Set materials list. Used in serialization.
    void SetMaterialsAttr(const FResourceRefList& materials);
    /// Return materials list. Used in serialization.
    FResourceRefList MaterialsAttr() const;

    /// %ALight list for rendering.
    LightList* _lightList;
    /// Geometry type.
    GeometryType::Type _geometryType;
    /// Draw call source datas.
    TVector<SourceBatch> _batches;
    /// Local space bounding box.
    TBoundingBoxF _boundingBox;
};

}
