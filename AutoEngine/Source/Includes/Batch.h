#pragma once
#include "Auto.h"
#include "Geometry.h"
#include "Drawable.h"

namespace Auto3D {
class View;
class Camera;

/// Queued 3D geometry draw call.
struct Batch
{
	/// Construct with defaults.
	Batch() = default;

	/// Construct from a drawable's source batch.
	explicit Batch(const SourceBatch& rhs)
		// :_distance(rhs._distance)
		//,_renderOrder(rhs.material_ ? rhs.material_->GetRenderOrder() : DEFAULT_RENDER_ORDER),
		//,_geometry(rhs._geometry),

	{
	}

	/// Calculate state sorting key, which consists of base pass flag, light, pass and geometry.
	void CalculateSortKey();
	/// Prepare for rendering.
	void Prepare(SharedPtr<View> view, SharedPtr<Camera> camera, bool setModelTransform, bool allowDepthWrite) const;
	/// Prepare and draw.
	void Draw(SharedPtr<View> view, SharedPtr<Camera> camera, bool allowDepthWrite) const;

	/// Geometry
	SharedPtr<Geometry> _geometry{};

	/// State sorting key.
	unsigned long long _sortKey{};
	/// Distance from camera.
	float _distance{};
	/// 8-bit render order modifier from material.
	unsigned char _renderOrder{};
};

/// Instanced 3D geometry draw call.
struct BatchGroup : public Batch
{
	/// Construct with defaults.
	BatchGroup()
		: _startIndex(MATH_MAX_UNSIGNED)
	{
	}

	/// Construct from a batch.
	explicit BatchGroup(const Batch& batch)
		: Batch(batch)
		, _startIndex(MATH_MAX_UNSIGNED)
	{
	}

	/// Destruct.
	~BatchGroup() = default;

	/// Instance stream start index, or M_MAX_UNSIGNED if transforms not pre-set.
	unsigned _startIndex;
};

/// Instanced draw call grouping key.
struct BatchGroupKey
{
	/// Construct undefined.
	BatchGroupKey() = default;

	/// Construct from a batch.
	explicit BatchGroupKey(const Batch& batch) 
		: _geometry(batch._geometry)
	{
	}

	/// Geometry.
	SharedPtr<Geometry> _geometry;

};

/// Queue that contains both instanced and non-instanced draw calls.
struct BatchQueue
{
	void SortBackToFront();
	/// Sort instanced and non-instanced draw calls front to back.
	void SortFrontToBack();

	/// Return whether the batch group is empty.
	bool IsEmpty() const { return _batches.empty() && _batchGroups.empty(); }

	/// Unsorted non-instanced draw calls.
	VECTOR<SharedPtr<Batch> > _batches;
	/// Sorted non-instanced draw calls.
	VECTOR<SharedPtr<Batch> > _sortedBatches;
	/// Instanced draw calls.
	PAIR_MAP<BatchGroupKey, SharedPtr<BatchGroup> > _batchGroups;
	/// Sorted instanced draw calls.
	VECTOR<SharedPtr<BatchGroup> > _sortedBatchGroups;
};

}