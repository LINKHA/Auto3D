#include "../Debug/Profiler.h"
#include "../Graphics/Texture.h"
#include "Batch.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

inline bool CompareBatchState(Batch& lhs, Batch& rhs)
{
    return lhs.sortKey < rhs.sortKey;
}

inline bool CompareBatchDistanceFrontToBack(Batch& lhs, Batch& rhs)
{
    return lhs.distance < rhs.distance;
}

inline bool CompareBatchDistanceBackToFront(Batch& lhs, Batch& rhs)
{
    return lhs.distance > rhs.distance;
}

void BatchQueue::Clear()
{
    batches.Clear();
    additiveBatches.Clear();
}

void BatchQueue::Sort(Vector<Matrix3x4>& instanceTransforms)
{
    switch (sort)
    {
    case SORT_STATE:
        Auto3D::Sort(batches.Begin(), batches.End(), CompareBatchState);
        Auto3D::Sort(additiveBatches.Begin(), additiveBatches.End(), CompareBatchState);
        break;

    case SORT_FRONT_TO_BACK:
        Auto3D::Sort(batches.Begin(), batches.End(), CompareBatchDistanceFrontToBack);
        // After drawing the base batches, the Z buffer has been prepared. Additive batches can be sorted per state now
        Auto3D::Sort(additiveBatches.Begin(), additiveBatches.End(), CompareBatchState);
        break;

    case SORT_BACK_TO_FRONT:
        Auto3D::Sort(batches.Begin(), batches.End(), CompareBatchDistanceBackToFront);
        Auto3D::Sort(additiveBatches.Begin(), additiveBatches.End(), CompareBatchDistanceBackToFront);
        break;

    default:
        break;
    }

    // Build instances where adjacent batches have same state
    BuildInstances(batches, instanceTransforms);
    BuildInstances(additiveBatches, instanceTransforms);
}

void BatchQueue::BuildInstances(Vector<Batch>& batches, Vector<Matrix3x4>& instanceTransforms)
{
    Batch* start = nullptr;
    for (auto it = batches.Begin(), end = batches.End(); it != end; ++it)
    {
        Batch* current = &*it;
        if (start && current->type == GEOM_STATIC && current->pass == start->pass && current->geometry == start->geometry &&
            current->lights == start->lights)
        {
            if (start->type == GEOM_INSTANCED)
            {
                instanceTransforms.Push(*current->worldMatrix);
                ++start->instanceCount;
            }
            else
            {
                // Begin new instanced batch
                start->type = GEOM_INSTANCED;
                size_t instanceStart = instanceTransforms.Size();
                instanceTransforms.Push(*start->worldMatrix);
                instanceTransforms.Push(*current->worldMatrix);
                start->instanceStart = instanceStart; // Overwrites non-instance world matrix
                start->instanceCount = 2; // Overwrites sort key / distance
            }
        }
        else
            start = (current->type == GEOM_STATIC) ? current : nullptr;
    }
}

ShadowMap::ShadowMap()
{
    // Construct texture but do not define its size yet
    texture = new Texture();
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::Clear()
{
    allocator.Reset(texture->Width(), texture->Height(), 0, 0, false);
    shadowViews.Clear();
    used = false;
}

void ShadowView::Clear()
{
    shadowQueue.Clear();
}

}
