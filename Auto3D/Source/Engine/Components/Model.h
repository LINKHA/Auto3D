#pragma once

#include "Graphics/GraphicsDefs.h"
#include "Math/BoundingBox.h"
#include "Resource/Resource.h"
#include "Scene/GeometryNode.h"

namespace Auto3D
{

class FVertexBuffer;
class FIndexBuffer;
class FGeometry;


/// AModel's bone description.
struct AUTO_API FBone
{
    /// Default-construct.
    FBone();
    /// Destruct.
    ~FBone();

    /// FName.
    FString _name;
    /// Reset _position.
    TVector3F _initialPosition;
    /// Reset rotation.
    FQuaternion _initialRotation;
    /// Reset scale.
    TVector3F _initialScale;
    /// Offset matrix for skinning.
    TMatrix3x4F _offsetMatrix;
    /// Collision radius.
    float _radius;
    /// Collision bounding box.
    TBoundingBoxF _boundingBox;
    /// Parent bone index.
    size_t _parentIndex;
    /// Associated scene node.
    TWeakPtr<ANode> _node;
    /// Animated flag.
    bool _animated;
};

/// 3D model resource.
class AUTO_API AModel : public AResource
{
	DECLARE_CLASS(AModel, AResource)

public:
    /// Construct.
    AModel();
    /// Destruct.
    ~AModel();

    /// Register object factory.
    static void RegisterObject();

    /// Load model from a stream. Return true on success.
    bool BeginLoad(FStream& source) override;
    /// Finalize model loading in the main thread. Return true on success.
    bool EndLoad() override;

    /// Set number of geometries.
    void SetNumGeometries(size_t num);
    /// Set number of LOD levels in a geometry.
    void SetNumLodLevels(size_t index, size_t num);
    /// Set local space bounding box.
    void SetLocalBoundingBox(const TBoundingBoxF& box);
    /// Set bones.
    void SetBones(const TVector<FBone>& bones, size_t rootBoneIndex);
    /// Set per-geometry bone mappings.
    void SetBoneMappings(const TVector<TVector<size_t> >& boneMappings);
    
    /// Return number of geometries.
    size_t GetNumGeometries() const { return _geometries.Size(); }
    /// Return number of LOD levels in a geometry.
    size_t GetNumLodLevels(size_t index) const;
    /// Return the geometry at batch index and LOD level.
    FGeometry* GetGeometry(size_t index, size_t lodLevel) const;
    /// Return the LOD geometries at batch index.
    const TVector<TSharedPtr<FGeometry> >& GetLodGeometries(size_t index) const { return _geometries[index]; }
    /// Return the local space bounding box.
    const TBoundingBoxF& GetLocalBoundingBox() const { return _boundingBox; }
    /// Return the model's bones.
    const TVector<FBone>& GetBones() const { return _bones; }
    /// Return the root bone index.
    size_t GetRootBoneIndex() const { return _rootBoneIndex; }
    /// Return per-geometry bone mapping.
    const TVector<TVector<size_t> > GetBoneMappings() const { return _boneMappings; }

private:
    /// FGeometry LOD levels.
    TVector<TVector<TSharedPtr<FGeometry> > > _geometries;
    /// Local space bounding box.
    TBoundingBoxF _boundingBox;
    /// %AModel's bones.
    TVector<FBone> _bones;
    /// Root bone index.
    size_t _rootBoneIndex;
    /// Per-geometry bone mappings.
    TVector<TVector<size_t> > _boneMappings;
    /// Vertex buffer data for loading.
    TVector<FVertexBufferDesc> _vbDescs;
    /// Index buffer data for loading.
    TVector<FIndexBufferDesc> _ibDescs;
    /// FGeometry descriptions for loading.
    TVector<TVector<FGeometryDesc> > _geomDescs;
};

}
