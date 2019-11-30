#pragma once
#include "Math/AreaAllocator.h"
#include "Engine/Components/Camera.h"
#include "Scene/GeometryNode.h"
#include "Engine/Components/Material.h"
#include "RenderPath.h"

namespace Auto3D
{

class ALight;
struct LightPass;

/// Maximum number of lights per pass.
static const size_t MAX_LIGHTS_PER_PASS = 4;


/// Description of a draw call.
struct AUTO_API Batch
{
    /// Calculate sort _key for state sorting.
    void CalculateSortKey()
    {
        _sortKey = ((((unsigned long long)_pass->GetShaderHash() * _type) & 0xffff) << 48) |
            ((((unsigned long long)_lights) & 0xffff) << 32) |
            ((((unsigned long long)_pass->Parent()) & 0xffff) << 16) |
            (((unsigned long long)_geometry) & 0xffff);
    }

    /// Geometry.
    Geometry* _geometry;
    /// AMaterial pass.
    FPass* _pass;
    /// ALight pass.
    LightPass* _lights;
    /// Geometry type.
    GeometryType::Type _type;

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
struct AUTO_API RenderQueue
{
    /// Clear structures.
    void Clear();
    /// Sort batches and build instances.
    void Sort(TVector<TMatrix3x4F>& instanceTransforms);

    /// Build instances from adjacent batches with same state.
    static void BuildInstances(TVector<Batch>& batches, TVector<TMatrix3x4F>& instanceTransforms);

    /// Batches, which may be instanced or non-instanced.
    TVector<Batch> _batches;
    /// Additive lighting batches.
    TVector<Batch> _additiveBatches;
    /// Sorting mode.
	RenderCommandSortMode::Type _sort;
    /// Lighting flag.
    bool _lit;
    /// Base pass index.
    unsigned char _baseIndex;
    /// Additive pass index (if needed.)
    unsigned char _additiveIndex;
};

/// %TList of lights for a geometry node.
struct AUTO_API LightList
{
    /// %TList _key.
    unsigned long long _key;
    /// Lights.
    TVector<ALight*> _lights;
    /// Associated light passes.
    TVector<LightPass*> _lightPasses;
    /// Use count
    size_t _useCount;
};

/// ALight information for a rendering pass, including properly formatted constant data.
struct AUTO_API LightPass
{
    /// ALight positions.
    TVector4F _lightPositions[MAX_LIGHTS_PER_PASS];
    /// ALight directions.
    TVector4F _lightDirections[MAX_LIGHTS_PER_PASS];
    /// ALight attenuation parameters.
    TVector4F _lightAttenuations[MAX_LIGHTS_PER_PASS];
    /// ALight colors.
    FColor _lightColors[MAX_LIGHTS_PER_PASS];
    /// Shadow map sampling parameters.
    TVector4F _shadowParameters[MAX_LIGHTS_PER_PASS];
    /// Point light shadow viewport parameters.
    TVector4F _pointShadowParameters[MAX_LIGHTS_PER_PASS];
    /// Directional light shadow split depths.
    TVector4F _dirShadowSplits;
    /// Directional light shadow fade parameters.
    TVector4F _dirShadowFade;
    /// Shadow mapping matrices.
    TMatrix4x4F _shadowMatrices[MAX_LIGHTS_PER_PASS];
    /// Shadow maps.
    ATexture* _shadowMaps[MAX_LIGHTS_PER_PASS];
    /// Vertex shader variation bits.
    unsigned short _vsBits;
    /// Pixel shader variation bits.
    unsigned short _psBits;
};

/// Shadow rendering view data structure.
struct AUTO_API ShadowView
{
    /// Clear existing shadow casters and batch queue.
    void Clear();

    /// %ALight that is using this view.
    ALight* _light;
    /// Viewport within the shadow map.
    TRectI _viewport;
    /// Shadow batch queue.
    RenderQueue _shadowQueue;
    /// Shadow camera.
    ACamera _shadowCamera;
};

/// Shadow map data structure. May be shared by several lights.
struct AUTO_API ShadowMap
{
    /// Default-construct.
    ShadowMap();
    /// Destruct.
    ~ShadowMap();

    /// Clear allocator and use flag.
    void Clear();

    /// Rectangle allocator.
    FAreaAllocator _allocator;
    /// Shadow map texture.
    TSharedPtr<ATexture> _texture;
    /// Shadow views that use this shadow map.
    TVector<ShadowView*> _shadowViews;
    /// Use flag. When false, clearing the shadow map and rendering the views can be skipped.
    bool _used;
};

/// AWater texture rendering view data structure.
struct AUTO_API WaterTextureView
{
	/// Clear existing shadow casters and batch queue.
	void Clear();

	/// Viewport within the shadow map.
	TRectI _viewport;
	/// AWater texture batch queue.
	RenderQueue _waterTextureQueue;
	/// AWater camera.
	ACamera _waterCamera;
};

/// AWater texture data structure.
struct AUTO_API WaterTexture
{
	/// Default-construct.
	WaterTexture();
	/// Destruct.
	~WaterTexture();

	/// Clear allocator and use flag.
	void Clear();

	/// Rectangle allocator.
	FAreaAllocator _allocator;
	/// Shadow map texture.
	TSharedPtr<ATexture> _texture;
	/// AWater texture view this use water texture.
	TVector<WaterTextureView*> _waterTextureView;
	/// Use flag. When false, clearing the shadow map and rendering the views can be skipped.
	bool _used;
};

}