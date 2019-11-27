#pragma once

#include "Graphics/GraphicsDefs.h"
#include "Math/BoundingBox.h"
#include "Resource/Resource.h"
#include "Scene/GeometryNode.h"

namespace Auto3D
{

class VertexBuffer;
class IndexBuffer;
class Geometry;


/// Model's bone description.
struct AUTO_API Bone
{
    /// Default-construct.
    Bone();
    /// Destruct.
    ~Bone();

    /// Name.
    String _name;
    /// Reset _position.
    Vector3F _initialPosition;
    /// Reset rotation.
    Quaternion _initialRotation;
    /// Reset scale.
    Vector3F _initialScale;
    /// Offset matrix for skinning.
    Matrix3x4F _offsetMatrix;
    /// Collision radius.
    float _radius;
    /// Collision bounding box.
    BoundingBoxF _boundingBox;
    /// Parent bone index.
    size_t _parentIndex;
    /// Associated scene node.
    WeakPtr<Node> _node;
    /// Animated flag.
    bool _animated;
};

/// 3D model resource.
class AUTO_API Model : public Resource
{
	REGISTER_OBJECT_CLASS(Model, Resource)

public:
    /// Construct.
    Model();
    /// Destruct.
    ~Model();

    /// Register object factory.
    static void RegisterObject();

    /// Load model from a stream. Return true on success.
    bool BeginLoad(Stream& source) override;
    /// Finalize model loading in the main thread. Return true on success.
    bool EndLoad() override;

    /// Set number of geometries.
    void SetNumGeometries(size_t num);
    /// Set number of LOD levels in a geometry.
    void SetNumLodLevels(size_t index, size_t num);
    /// Set local space bounding box.
    void SetLocalBoundingBox(const BoundingBoxF& box);
    /// Set bones.
    void SetBones(const Vector<Bone>& bones, size_t rootBoneIndex);
    /// Set per-geometry bone mappings.
    void SetBoneMappings(const Vector<Vector<size_t> >& boneMappings);
    
    /// Return number of geometries.
    size_t GetNumGeometries() const { return _geometries.Size(); }
    /// Return number of LOD levels in a geometry.
    size_t GetNumLodLevels(size_t index) const;
    /// Return the geometry at batch index and LOD level.
    Geometry* GetGeometry(size_t index, size_t lodLevel) const;
    /// Return the LOD geometries at batch index.
    const Vector<SharedPtr<Geometry> >& GetLodGeometries(size_t index) const { return _geometries[index]; }
    /// Return the local space bounding box.
    const BoundingBoxF& GetLocalBoundingBox() const { return _boundingBox; }
    /// Return the model's bones.
    const Vector<Bone>& GetBones() const { return _bones; }
    /// Return the root bone index.
    size_t GetRootBoneIndex() const { return _rootBoneIndex; }
    /// Return per-geometry bone mapping.
    const Vector<Vector<size_t> > GetBoneMappings() const { return _boneMappings; }

private:
    /// Geometry LOD levels.
    Vector<Vector<SharedPtr<Geometry> > > _geometries;
    /// Local space bounding box.
    BoundingBoxF _boundingBox;
    /// %Model's bones.
    Vector<Bone> _bones;
    /// Root bone index.
    size_t _rootBoneIndex;
    /// Per-geometry bone mappings.
    Vector<Vector<size_t> > _boneMappings;
    /// Vertex buffer data for loading.
    Vector<VertexBufferDesc> _vbDescs;
    /// Index buffer data for loading.
    Vector<IndexBufferDesc> _ibDescs;
    /// Geometry descriptions for loading.
    Vector<Vector<GeometryDesc> > _geomDescs;
};

}
