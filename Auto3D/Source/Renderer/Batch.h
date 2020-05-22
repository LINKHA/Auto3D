#pragma once
#include "AutoConfig.h"
#include "Container/Vector.h"
#include "Renderer/GeometryPass.h"
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
		_type(EGeometryType::STATIC)
	{}

	/// AMaterial pass.
	FGeometryPass _pass;
	/// FGeometry type.
	EGeometryType::Type _type;


	/// Instanced mode start index.
	size_t _instanceStart;

	union
	{
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
	void Sort();

	/// Build instances from adjacent batches with same state.
	static void BuildInstances(TVector<FBatch>& batches);

	/// Batches, which may be instanced or non-instanced.
	TVector<FBatch> _batches;
	/// Sorting mode.
	ERenderCommandSortMode::Type _sort;
	/// Base pass index.
	unsigned char _baseIndex;
};

}