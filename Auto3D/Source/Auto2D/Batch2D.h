#pragma once
#include "AutoConfig.h"
#include "GeometryNode2D.h"


namespace Auto3D
{

class ATexture;

struct AUTO_API FBatch2D
{
	/// Calculate sort _key for state sorting.
	void CalculateSortKey()
	{
		_sortKey = ((((unsigned long long) _geometry) & 0xffff) << 48) |
			((((unsigned long long)_type) & 0xffff) << 32) |
			((((unsigned long long)_texture) & 0xffff) << 16);
	}

	/// FGeometry.
	FGeometry* _geometry;
	/// FGeometry type.
	EGeometryType::Type _type;

	ATexture* _texture;

	union
	{
		/// Sort _key for state sorting.
		unsigned long long _sortKey;
		/// Distance for sorting.
		float _distance;
		/// Instanced mode instance count.
		size_t _instanceCount;
	};

	union
	{
		/// Non-instanced use world matrix.
		const TMatrix3x4F* _worldMatrix;
		/// Instanced mode start index.
		size_t _instanceStart;
	};
};

/// Per-pass batch queue structure.
struct AUTO_API FBatch2DQueue
{
	/// Clear structures.
	void Clear();
	/// Sort batches and build instances.
	void Sort(TVector<TMatrix3x4F>& instanceTransforms);

	/// Build instances from adjacent batches with same state.
	static void BuildInstances(TVector<FBatch2D>& batches, TVector<TMatrix3x4F>& instanceTransforms);

	/// Batches, which may be instanced or non-instanced.
	TVector<FBatch2D> _batches;
	/// Base pass index.
	unsigned char _baseIndex;
	/// Additive pass index (if needed.)
	unsigned char _additiveIndex;
};

}