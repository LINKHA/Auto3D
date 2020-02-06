#pragma once
#include "AutoConfig.h"
#include "Container/Vector.h"
#include "Renderer/Pass.h"
#include "Math/Matrix3x4.h"

namespace Auto3D
{
/// FGeometry types.
namespace EGeometryType
{
	enum Type
	{
		STATIC = 0,
		INSTANCED
	};
};

/// Rendering path sorting modes.
namespace ERenderCommandSortMode
{
	enum Type
	{
		NONE = 0,
		STATE,
		BACK_TO_FRONT,
		FRONT_TO_BACK,
	};
}

struct AUTO_API FBatch
{
	FBatch() :
		_type(EGeometryType::STATIC),
		_worldMatrix(nullptr),
		_sortKey(0)
	{}

	/// Calculate sort _key for state sorting.
	void CalculateSortKey()
	{
		_sortKey = ((((unsigned long long)_pass._shaderHash) & 0xffff) << 48)|
		((((unsigned long long)_pass._geometryHash) & 0xffff) << 32); 
	}

	/// AMaterial pass.
	FPass _pass;
	/// FGeometry type.
	EGeometryType::Type _type;

	union
	{
		/// Non-instanced use world matrix.
		const TMatrix3x4F* _worldMatrix;
		/// Instanced mode start index.
		size_t _instanceStart;
	};

	union
	{
		/// Sort _key for state sorting.
		unsigned long long _sortKey;
		/// Distance for sorting.
		float _distance;
		/// Instanced mode instance count.
		size_t _instanceCount;
	};

};


/// Per-pass batch queue structure.
struct AUTO_API FRenderQueue
{
	FRenderQueue();

	/// Clear structures.
	void Clear();
	/// Sort batches and build instances.
	void Sort(TVector<TMatrix3x4F>& instanceTransforms);

	/// Build instances from adjacent batches with same state.
	static void BuildInstances(TVector<FBatch>& batches, TVector<TMatrix3x4F>& instanceTransforms);

	/// Batches, which may be instanced or non-instanced.
	TVector<FBatch> _batches;
	/// Additive lighting batches.
	TVector<FBatch> _additiveBatches;
	/// Sorting mode.
	ERenderCommandSortMode::Type _sort;
	/// Lighting flag.
	bool _lit;
	/// Base pass index.
	unsigned char _baseIndex;
	/// Additive pass index (if needed.)
	unsigned char _additiveIndex;
};



}