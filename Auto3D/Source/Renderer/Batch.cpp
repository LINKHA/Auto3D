#include "Debug/Profiler.h"
#include "Graphics/Texture.h"
#include "Batch.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

inline bool CompareBatchState(FBatch& lhs, FBatch& rhs)
{
    return lhs._sortKey < rhs._sortKey;
}

inline bool CompareBatchDistanceFrontToBack(FBatch& lhs, FBatch& rhs)
{
    return lhs._distance < rhs._distance;
}

inline bool CompareBatchDistanceBackToFront(FBatch& lhs, FBatch& rhs)
{
    return lhs._distance > rhs._distance;
}

void FRenderQueue::Clear()
{
    _batches.Clear();
    _additiveBatches.Clear();
}

void FRenderQueue::Sort(TVector
	<TMatrix3x4F>& instanceTransforms)
{
    switch (_sort)
    {
    case ERenderCommandSortMode::STATE:
        Auto3D::Sort(_batches.Begin(), _batches.End(), CompareBatchState);
        Auto3D::Sort(_additiveBatches.Begin(), _additiveBatches.End(), CompareBatchState);
        break;

    case ERenderCommandSortMode::FRONT_TO_BACK:
        Auto3D::Sort(_batches.Begin(), _batches.End(), CompareBatchDistanceFrontToBack);
        // After drawing the base batches, the Z buffer has been prepared. Additive batches can be sorted per state now
        Auto3D::Sort(_additiveBatches.Begin(), _additiveBatches.End(), CompareBatchState);
        break;

    case ERenderCommandSortMode::BACK_TO_FRONT:
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

void FRenderQueue::BuildInstances(TVector<FBatch>& batches, TVector<TMatrix3x4F>& instanceTransforms)
{
    FBatch* start = nullptr;

    for (auto it = batches.Begin(), end = batches.End(); it != end; ++it)
    {
        FBatch* current = &*it;

        if (start && current->_type == EGeometryType::STATIC && current->_pass == start->_pass && current->_geometry == start->_geometry &&
            current->_lights == start->_lights)
        {
            if (start->_type == EGeometryType::INSTANCED)
            {
                instanceTransforms.Push(*current->_worldMatrix);
                ++start->_instanceCount;
            }
            else
            {
                // Begin new instanced batch
                start->_type = EGeometryType::INSTANCED;
                size_t instanceStart = instanceTransforms.Size();
                instanceTransforms.Push(*start->_worldMatrix);
                instanceTransforms.Push(*current->_worldMatrix);
                start->_instanceStart = instanceStart; // Overwrites non-instance world matrix
                start->_instanceCount = 2; // Overwrites sort _key / distance
            }
        }
        else
            start = (current->_type == EGeometryType::STATIC) ? current : nullptr;
    }
}

void FShadowView::Clear()
{
	_shadowQueue.Clear();
}

FShadowMap::FShadowMap()
{
    // Construct texture but do not define its _size yet
    _texture = new ATexture();
}

FShadowMap::~FShadowMap()
{
}

void FShadowMap::Clear()
{
    _allocator.Reset(_texture->GetWidth(), _texture->GetHeight(), 0, 0, false);
    _shadowViews.Clear();
    _used = false;
}

}
