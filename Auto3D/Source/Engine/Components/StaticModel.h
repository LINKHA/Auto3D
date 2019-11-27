#pragma once

#include "Scene/GeometryNode.h"

namespace Auto3D
{
class Model;
/// %Scene node that renders an unanimated model.
class AUTO_API StaticModel : public GeometryNode
{
    REGISTER_OBJECT_CLASS(StaticModel, GeometryNode)

public:
    /// Construct.
    StaticModel();
    /// Destruct.
    ~StaticModel();

    /// Register factory and attributes.
    static void RegisterObject();

    /// Prepare object for rendering. Reset framenumber and light list and calculate distance from camera, and check for LOD level changes. Called by Renderer.
    void OnPrepareRender(unsigned frameNumber, Camera* camera) override;

    /// Set the model resource.
    void SetModel(Model* model);
    /// Set LOD bias. Values higher than 1 use higher quality LOD (acts if distance is smaller.)
    void SetLodBias(float bias);

    /// Return the model resource.
    Model* GetModel() const;
    /// Return LOD bias.
    float LodBias() const { return _lodBias; }

private:
    /// Set model attribute. Used in serialization.
    void SetModelAttr(const ResourceRef& model);
    /// Return model attribute. Used in serialization.
    ResourceRef ModelAttr() const;

    /// Current model resource.
    SharedPtr<Model> _model;
    /// LOD bias value.
    float _lodBias;
    /// Lod levels flag.
    bool _hasLodLevels;
};

}
