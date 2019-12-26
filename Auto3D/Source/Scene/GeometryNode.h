#pragma once

#include "Graphics/GraphicsDefs.h"
#include "IO/ResourceRef.h"
#include "OctreeNode.h"
#include "Auto2D/Transform2D.h"

namespace Auto3D
{

class FConstantBuffer;
class FGraphicsModule;
class FIndexBuffer;
class AMaterial;
class FVertexBuffer;
struct FLightList;
class ATexture;

/// FGeometry types.
namespace EGeometryType
{
	enum Type
	{
		STATIC = 0,
		INSTANCED
	};
};


/// Load-time description of a vertex buffer, to be uploaded on the GPU later.
struct AUTO_API FVertexBufferDesc
{
	/// Vertex declaration.
	TVector<FVertexElement> _vertexElements;
	/// Number of vertices.
	size_t _numVertices;
	/// Vertex data.
	TSharedArrayPtr<unsigned char> _vertexData;
};

/// Load-time description of an index buffer, to be uploaded on the GPU later.
struct AUTO_API FIndexBufferDesc
{
	/// Index _size.
	size_t _indexSize;
	/// Number of indices.
	size_t _numIndices;
	/// Index data.
	TSharedArrayPtr<unsigned char> _indexData;
};

/// Load-time description of a geometry.
struct AUTO_API FGeometryDesc
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

/// Description of geometry to be rendered. %AScene nodes that render the same object can share these to reduce memory load and allow instancing.
class AUTO_API FGeometry : public FRefCounted
{
public:
    /// Default-construct.
    FGeometry();
    /// Destruct.
    ~FGeometry();

    /// Draw using the AGraphics subsystem. The constant buffers are not applied automatically, rather they must have been applied beforehand.
    void Draw(FGraphicsModule* graphics);
    /// Draw an instance range. A separate instance data vertex buffer must be bound.
    void DrawInstanced(FGraphicsModule* graphics, size_t start, size_t count);

    /// %FGeometry vertex buffer.
    TSharedPtr<FVertexBuffer> _vertexBuffer;
    /// %FGeometry index buffer.
    TSharedPtr<FIndexBuffer> _indexBuffer;
    /// FConstant buffers.
    TSharedPtr<FConstantBuffer> _constantBuffers[EShaderStage::Count];
    /// %FGeometry's primitive type.
    EPrimitiveType::Type _primitiveType;
    /// Draw range start. Specifies index start if index buffer defined, vertex start otherwise.
    size_t _drawStart;
    /// Draw range count. Specifies number of indices if index buffer defined, number of vertices otherwise.
    size_t _drawCount;
    /// LOD transition distance.
    float _lodDistance;
};

/// Draw call source data.
struct AUTO_API FSourceBatch
{
    /// Construct empty.
    FSourceBatch();
    /// Destruct.
    ~FSourceBatch();

    /// The geometry to render. Must be non-null.
    TSharedPtr<FGeometry> _geometry;
    /// The material to use for rendering. Must be non-null.
    TSharedPtr<AMaterial> _material;
};

/// Base class for scene nodes that contain geometry to be rendered.
class AUTO_API AGeometryNode : public AOctreeNode
{
    DECLARE_CLASS(AGeometryNode, AOctreeNode)
public:
    /// Construct.
    AGeometryNode();
    /// Destruct.
    ~AGeometryNode();

    /// Prepare object for rendering. Reset framenumber and light list and calculate distance from camera. Called by ARenderer.
    void OnPrepareRender(unsigned frameNumber, ACamera* camera) override;

    /// Set geometry type, which is shared by all geometries.
    void SetGeometryType(EGeometryType::Type type);
    /// Set number of geometries.
    void SetNumGeometries(size_t num);
    /// Set geometry at index.
    void SetGeometry(size_t index, FGeometry* geometry);
    /// Set material at every geometry index. Specifying null will use the default material (opaque white.)
    void SetMaterial(AMaterial* material);
    /// Set material at geometry index.
    void SetMaterial(size_t index, AMaterial* material);
    /// Set local space bounding box.
    void SetLocalBoundingBox(const TBoundingBoxF& box);

    /// Return geometry type.
    EGeometryType::Type GetGeometryType() const { return _geometryType; }
    /// Return number of geometries.
    size_t GetNumGeometries() const { return _batches.Size(); }
    /// Return geometry by index.
    FGeometry* GetGeometry(size_t index) const;
    /// Return material by geometry index.
    AMaterial* GetMaterial(size_t index) const;
    /// Return source information for all draw calls.
    const TVector<FSourceBatch>& GetBatches() const { return _batches; }
    /// Return local space bounding box.
    const TBoundingBoxF& GetLocalBoundingBox() const { return _boundingBox; }

    /// Set new light list. Called by ARenderer.
    void SetLightList(FLightList* list) { _lightList = list; }
    /// Return current light list.
    FLightList* GetLightList() const { return _lightList; }
public:
	/// Set materials list. Used in serialization.
	void SetMaterialsAttr(FResourceRefList materials);
	/// Return materials list. Used in serialization.
	FResourceRefList GetMaterialsAttr() const;
protected:
    /// Recalculate the world space bounding box.
    void OnWorldBoundingBoxUpdate() const override;


    /// %ALight list for rendering.
    FLightList* _lightList;
    /// FGeometry type.
    EGeometryType::Type _geometryType;
    /// Draw call source datas.
    TVector<FSourceBatch> _batches;
    /// Local space bounding box.
    TBoundingBoxF _boundingBox;
};

}
