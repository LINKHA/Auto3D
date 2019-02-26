#include "../Debug/Log.h"
#include "../Graphics/ConstantBuffer.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/IndexBuffer.h"
#include "../Graphics/VertexBuffer.h"
#include "../Resource/ResourceCache.h"
#include "Camera.h"
#include "GeometryNode.h"
#include "Material.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

Geometry::Geometry() : 
    _primitiveType(TRIANGLE_LIST),
    _drawStart(0),
    _drawCount(0),
    _lodDistance(0.0f)
{
}

Geometry::~Geometry()
{
}

void Geometry::Draw(Graphics* graphics)
{
    graphics->SetVertexBuffer(0, _vertexBuffer.Get());
    if (_indexBuffer.Get())
    {
        graphics->SetIndexBuffer(_indexBuffer.Get());
        graphics->DrawIndexed(_primitiveType, _drawStart,_drawCount, 0);
    }
    else
        graphics->Draw(_primitiveType, _drawStart, _drawCount);
}

void Geometry::DrawInstanced(Graphics* graphics, size_t start, size_t count)
{
    graphics->SetVertexBuffer(0, _vertexBuffer.Get());
    if (_indexBuffer.Get())
    {
        graphics->SetIndexBuffer(_indexBuffer.Get());
        graphics->DrawIndexedInstanced(_primitiveType, _drawStart, _drawCount, 0, start, count);
    }
    else
        graphics->DrawInstanced(_primitiveType, _drawStart, _drawCount, start, count);
}

SourceBatch::SourceBatch()
{
}

SourceBatch::~SourceBatch()
{
}

GeometryNode::GeometryNode() :
    _lightList(nullptr),
    _geometryType(GEOM_STATIC)
{
    SetFlag(NF_GEOMETRY, true);
}

GeometryNode::~GeometryNode()
{
}

void GeometryNode::RegisterObject()
{
    RegisterFactory<GeometryNode>();
    CopyBaseAttributes<GeometryNode, OctreeNode>();
    RegisterMixedRefAttribute("materials", &GeometryNode::MaterialsAttr, &GeometryNode::SetMaterialsAttr,
        ResourceRefList(Material::TypeStatic()));
}

void GeometryNode::OnPrepareRender(unsigned frameNumber, Camera* camera)
{
    _lastFrameNumber = frameNumber;
    _lightList = nullptr;
    _distance = camera->Distance(WorldPosition());
}

void GeometryNode::SetGeometryType(GeometryType type)
{
    _geometryType = type;
}

void GeometryNode::SetNumGeometries(size_t num)
{
    _batches.Resize(num);
    
    // Ensure non-null materials
    for (auto it = _batches.Begin(); it != _batches.End(); ++it)
    {
        if (!it->_material.Get())
            it->_material = Material::DefaultMaterial();
    }
}

void GeometryNode::SetGeometry(size_t index, Geometry* geometry)
{
    if (!geometry)
    {
        ErrorString("Can not assign null geometry");
        return;
    }

    if (index < _batches.Size())
        _batches[index]._geometry = geometry;
    else
        ErrorStringF("Out of bounds batch index %d for setting geometry", (int)index);
}

void GeometryNode::SetMaterial(Material* material)
{
    if (!material)
        material = Material::DefaultMaterial();

    for (size_t i = 0; i < _batches.Size(); ++i)
        _batches[i]._material = material;
}

void GeometryNode::SetMaterial(size_t index, Material* material)
{
    if (index < _batches.Size())
    {
        if (!material)
            material = Material::DefaultMaterial();
        _batches[index]._material = material;
    }
    else
        ErrorStringF("Out of bounds batch index %d for setting material", (int)index);
}

void GeometryNode::SetLocalBoundingBox(const BoundingBox& box)
{
    _boundingBox = box;
    // Changing the bounding box may require octree reinsertion
    OctreeNode::OnTransformChanged();
}

Geometry* GeometryNode::GetGeometry(size_t index) const
{
    return index < _batches.Size() ? _batches[index]._geometry.Get() : nullptr;
}

Material* GeometryNode::GetMaterial(size_t index) const
{
    return index < _batches.Size() ? _batches[index]._material.Get() : nullptr;
}

void GeometryNode::OnWorldBoundingBoxUpdate() const
{
    _worldBoundingBox = _boundingBox.Transformed(WorldTransform());
    SetFlag(NF_BOUNDING_BOX_DIRTY, false);
}

void GeometryNode::SetMaterialsAttr(const ResourceRefList& materials)
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    for (size_t i = 0; i < materials._names.Size(); ++i)
        SetMaterial(i, cache->LoadResource<Material>(materials._names[i]));
}

ResourceRefList GeometryNode::MaterialsAttr() const
{
    ResourceRefList ret(Material::TypeStatic());
    
    ret._names.Resize(_batches.Size());
    for (size_t i = 0; i < _batches.Size(); ++i)
        ret._names[i] = ResourceName(_batches[i]._material.Get());

    return ret;
}

}
