#pragma once

#include "Scene/GeometryNode.h"

namespace Auto3D
{
class AModel;
/// %Scene node that renders an unanimated model.
class AUTO_API AStaticModel : public AGeometryNode
{
    REGISTER_OBJECT_CLASS(AStaticModel, AGeometryNode)

public:
    /// Construct.
    AStaticModel();
    /// Destruct.
    ~AStaticModel();

    /// Register factory and attributes.
    static void RegisterObject();

    /// Prepare object for rendering. Reset framenumber and light list and calculate distance from camera, and check for LOD level changes. Called by Renderer.
    void OnPrepareRender(unsigned frameNumber, ACamera* camera) override;

    /// Set the model resource.
    void SetModel(AModel* model);
    /// Set LOD bias. Values higher than 1 use higher quality LOD (acts if distance is smaller.)
    void SetLodBias(float bias);

    /// Return the model resource.
    AModel* GetModel() const;
    /// Return LOD bias.
    float LodBias() const { return _lodBias; }

private:
    /// Set model attribute. Used in serialization.
    void SetModelAttr(const ResourceRef& model);
    /// Return model attribute. Used in serialization.
    ResourceRef ModelAttr() const;

    /// Current model resource.
    TSharedPtr<AModel> _model;
    /// LOD bias value.
    float _lodBias;
    /// Lod levels flag.
    bool _hasLodLevels;
};

}
