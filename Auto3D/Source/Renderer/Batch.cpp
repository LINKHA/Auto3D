#include "../Debug/Profiler.h"
#include "../Graphics/Texture.h"
#include "Batch.h"

#include "../Debug/DebugNew.h"

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

void BatchQueue::Clear()
{
    _batches.Clear();
    _additiveBatches.Clear();
}

void BatchQueue::Sort(Vector<Matrix3x4>& instanceTransforms)
{
    switch (_sort)
    {
    case SORT_STATE:
        Auto3D::Sort(_batches.Begin(), _batches.End(), CompareBatchState);
        Auto3D::Sort(_additiveBatches.Begin(), _additiveBatches.End(), CompareBatchState);
        break;

    case SORT_FRONT_TO_BACK:
        Auto3D::Sort(_batches.Begin(), _batches.End(), CompareBatchDistanceFrontToBack);
        // After drawing the base batches, the Z buffer has been prepared. Additive batches can be sorted per state now
        Auto3D::Sort(_additiveBatches.Begin(), _additiveBatches.End(), CompareBatchState);
        break;

    case SORT_BACK_TO_FRONT:
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

void BatchQueue::BuildInstances(Vector<Batch>& batches, Vector<Matrix3x4>& instanceTransforms)
{
    Batch* start = nullptr;
    for (auto it = batches.Begin(), end = batches.End(); it != end; ++it)
    {
        Batch* current = &*it;
        if (start && current->_type == GEOM_STATIC && current->_pass == start->_pass && current->_geometry == start->_geometry &&
            current->_lights == start->_lights)
        {
            if (start->_type == GEOM_INSTANCED)
            {
                instanceTransforms.Push(*current->_worldMatrix);
                ++start->_instanceCount;
            }
            else
            {
                // Begin new instanced batch
                start->_type = GEOM_INSTANCED;
                size_t instanceStart = instanceTransforms.Size();
                instanceTransforms.Push(*start->_worldMatrix);
                instanceTransforms.Push(*current->_worldMatrix);
                start->_instanceStart = instanceStart; // Overwrites non-instance world matrix
                start->_instanceCount = 2; // Overwrites sort _key / distance
            }
        }
        else
            start = (current->_type == GEOM_STATIC) ? current : nullptr;
    }
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
    _allocator.Reset(_texture->Width(), _texture->Height(), 0, 0, false);
    _shadowViews.Clear();
    _used = false;
}

void ShadowView::Clear()
{
    _shadowQueue.Clear();
}

}
