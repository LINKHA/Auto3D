#pragma once
#include "Auto.h"
#include "AreaAllocator.h"
#include "Light.h"
#include "Material.h"
#include "Geometry.h"
#include "Camera.h"

namespace Auto3D {
class Texture;
struct LightPass;

/// Maximum number of lights per pass.
static const size_t MAX_LIGHTS_PER_PASS = 4;
/// Batch sorting modes.
enum BatchSortMode
{
	SORT_NONE = 0,
	SORT_STATE,
	SORT_BACK_TO_FRONT,
	SORT_FRONT_TO_BACK,
};

/// Description of a draw call.
struct Batch
{
	/// Calculate sort key for state sorting.
	void CalculateSortKey()
	{

	}

	/// Geometry.
	SharedPtr<Geometry> geometry;
	/// Material pass.
	SharedPtr<Pass> pass;
	/// Light pass.
	SharedPtr<LightPass> lights;
	/// Geometry type.
	GeometryType type;

	union
	{
		/// Non-instanced use world matrix.
		const Matrix3x4F* worldMatrix;
		/// Instanced mode start index.
		size_t instanceStart;
	};

	union
	{
		/// Sort key for state sorting.
		unsigned long long sortKey;
		/// Distance for sorting.
		float distance;
		/// Instanced mode instance count.
		size_t instanceCount;
	};
};
/// Per-pass batch queue structure.
struct BatchQueue
{
	/// Clear structures.
	void Clear();
	/// Sort batches and build instances.
	void Sort(VECTOR<Matrix3x4F>& instanceTransforms);

	/// Build instances from adjacent batches with same state.
	static void BuildInstances(VECTOR<Batch>& batches, VECTOR<Matrix3x4F>& instanceTransforms);

	/// Batches, which may be instanced or non-instanced.
	VECTOR<Batch> batches;
	/// Additive lighting batches.
	VECTOR<Batch> additiveBatches;
	/// Sorting mode.
	BatchSortMode sort;
	/// Lighting flag.
	bool lit;
	/// Base pass index.
	unsigned char baseIndex;
	/// Additive pass index (if needed.)
	unsigned char additiveIndex;
};

/// Shadow rendering view data structure.
struct ShadowView
{
	/// Clear existing shadow casters and batch queue.
	void Clear();

	/// %Light that is using this view.
	SharedPtr<Light> light;
	/// Viewport within the shadow map.
	RectInt viewport;
	/// Shadow batch queue.
	BatchQueue shadowQueue;
	/// Shadow camera.
	Camera shadowCamera;
};

/// Shadow map data structure. May be shared by several lights.
class ShadowMap : public Object
{
	REGISTER_OBJECT_CLASS(ShadowMap, Object)
public:
	// Default-construct.
	explicit ShadowMap(SharedPtr<Ambient> ambient);


	/// Clear allocator and use flag.
	void Clear();

	/// Rectangle allocator.
	AreaAllocator _allocator;
	/// Shadow map texture.
	SharedPtr<Texture> _texture;
	/// Shadow views that use this shadow map.
	VECTOR<SharedPtr<ShadowView> > _shadowViews;
	/// Use flag. When false, clearing the shadow map and rendering the views can be skipped.
	bool _used;
};


/// %Light information for a rendering pass, including properly formatted constant data.
struct LightPass
{
	/// %Light positions.
	Vector4F lightPositions[MAX_LIGHTS_PER_PASS];
	/// %Light directions.
	Vector4F lightDirections[MAX_LIGHTS_PER_PASS];
	/// %Light attenuation parameters.
	Vector4F lightAttenuations[MAX_LIGHTS_PER_PASS];
	/// %Light colors.
	Color lightColors[MAX_LIGHTS_PER_PASS];
	/// Shadow map sampling parameters.
	Vector4F shadowParameters[MAX_LIGHTS_PER_PASS];
	/// Point light shadow viewport parameters.
	Vector4F pointShadowParameters[MAX_LIGHTS_PER_PASS];
	/// Directional light shadow split depths.
	Vector4F dirShadowSplits;
	/// Directional light shadow fade parameters.
	Vector4F dirShadowFade;
	/// Shadow mapping matrices.
	Matrix4x4F shadowMatrices[MAX_LIGHTS_PER_PASS];
	/// Shadow maps.
	SharedPtr<Texture> shadowMaps[MAX_LIGHTS_PER_PASS];
	/// Vertex shader variation bits.
	unsigned short vsBits;
	/// Pixel shader variation bits.
	unsigned short psBits;
};

}