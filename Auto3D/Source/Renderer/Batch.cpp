#include "Debug/Profiler.h"
#include "Graphics/Texture.h"
#include "Batch.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

inline bool CompareBatchState(Batch& lhs, Batch& rhs)
{
    return lhs._sortKey < rhs._sortKey;
}

inline bool CompareBatchDistanceFrontToBack(Batch& lhs, Batch& rhs)
{
    return lhs._distance < rhs._distance;
}

inline bool CompareBatchDistanceBackToFront(Batch& lhs, Batch& rhs)
{
    return lhs._distance > rhs._distance;
}

void RenderQueue::Clear()
{
    _batches.Clear();
    _additiveBatches.Clear();
}

void RenderQueue::Sort(Vector
	<Matrix3x4F>& instanceTransforms)
{
    switch (_sort)
    {
    case RenderCommandSortMode::STATE:
        Auto3D::Sort(_batches.Begin(), _batches.End(), CompareBatchState);
        Auto3D::Sort(_additiveBatches.Begin(), _additiveBatches.End(), CompareBatchState);
        break;

    case RenderCommandSortMode::FRONT_TO_BACK:
        Auto3D::Sort(_batches.Begin(), _batches.End(), CompareBatchDistanceFrontToBack);
        // After drawing the base batches, the Z buffer has been prepared. Additive batches can be sorted per state now
        Auto3D::Sort(_additiveBatches.Begin(), _additiveBatches.End(), CompareBatchState);
        break;

    case RenderCommandSortMode::BACK_TO_FRONT:
        Auto3D::Sort(_batches.Begin(), _batches.End(), CompareBatchDistanceBackToFront);
        Auto3D::Sort(_additiveBatches.Begin(), _additiveBatches.End(), CompareBatchDistanceBackToFront);
        break;

    default:
        break;
    }

    // Build instances where adjacent batches have same state
    BuildInstances(_batches, instanceTransforms);
    BuildInstances(_additiveBatches, instanceTransforms);
}

void RenderQueue::BuildInstances(Vector<Batch>& batches, Vector<Matrix3x4F>& instanceTransforms)
{
    Batch* start = nullptr;

    for (auto it = batches.Begin(), end = batches.End(); it != end; ++it)
    {
        Batch* current = &*it;

        if (start && current->_type == GeometryType::STATIC && current->_pass == start->_pass && current->_geometry == start->_geometry &&
            current->_lights == start->_lights)
        {
            if (start->_type == GeometryType::INSTANCED)
            {
                instanceTransforms.Push(*current->_worldMatrix);
                ++start->_instanceCount;
            }
            else
            {
                // Begin new instanced batch
                start->_type = GeometryType::INSTANCED;
                size_t instanceStart = instanceTransforms.Size();
                instanceTransforms.Push(*start->_worldMatrix);
                instanceTransforms.Push(*current->_worldMatrix);
                start->_instanceStart = instanceStart; // Overwrites non-instance world matrix
                start->_instanceCount = 2; // Overwrites sort _key / distance
            }
        }
        else
            start = (current->_type == GeometryType::STATIC) ? current : nullptr;
    }
}

void ShadowView::Clear()
{
	_shadowQueue.Clear();
}

ShadowMap::ShadowMap()
{
    // Construct texture but do not define its _size yet
    _texture = new Texture();
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::Clear()
{
    _allocator.Reset(_texture->GetWidth(), _texture->GetHeight(), 0, 0, false);
    _shadowViews.Clear();
    _used = false;
}

WaterTexture::WaterTexture()
{
	// Construct texture but do not define its size yet
	_texture = new Texture();
}

WaterTexture::~WaterTexture()
{
}

void WaterTextureView::Clear()
{
	_waterTextureQueue.Clear();
}

void WaterTexture::Clear()
{
	_allocator.Reset(_texture->GetWidth(), _texture->GetHeight(), 0, 0, false);
	_waterTextureView.Clear();
	_used = false;
}


}
