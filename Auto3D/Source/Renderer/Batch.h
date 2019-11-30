#pragma once
#include "Math/AreaAllocator.h"
#include "Engine/Components/Camera.h"
#include "Scene/GeometryNode.h"
#include "Engine/Components/Material.h"
#include "RenderPath.h"

namespace Auto3D
{

class ALight;
struct FLightPass;

/// Maximum number of lights per pass.
static const size_t MAX_LIGHTS_PER_PASS = 4;


/// Description of a draw call.
struct AUTO_API FBatch
{
    /// Calculate sort _key for state sorting.
    void CalculateSortKey()
    {
        _sortKey = ((((unsigned long long)_pass->GetShaderHash() * _type) & 0xffff) << 48) |
            ((((unsigned long long)_lights) & 0xffff) << 32) |
            ((((unsigned long long)_pass->Parent()) & 0xffff) << 16) |
            (((unsigned long long)_geometry) & 0xffff);
    }

    /// FGeometry.
    FGeometry* _geometry;
    /// AMaterial pass.
    FPass* _pass;
    /// ALight pass.
    FLightPass* _lights;
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

/// %TList of lights for a geometry node.
struct AUTO_API FLightList
{
    /// %TList _key.
    unsigned long long _key;
    /// Lights.
    TVector<ALight*> _lights;
    /// Associated light passes.
    TVector<FLightPass*> _lightPasses;
    /// Use count
    size_t _useCount;
};

/// ALight information for a rendering pass, including properly formatted constant data.
struct AUTO_API FLightPass
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
struct AUTO_API FShadowView
{
    /// Clear existing shadow casters and batch queue.
    void Clear();

    /// %ALight that is using this view.
    ALight* _light;
    /// Viewport within the shadow map.
    TRectI _viewport;
    /// Shadow batch queue.
    FRenderQueue _shadowQueue;
    /// Shadow camera.
    ACamera _shadowCamera;
};

/// Shadow map data structure. May be shared by several lights.
struct AUTO_API FShadowMap
{
    /// Default-construct.
    FShadowMap();
    /// Destruct.
    ~FShadowMap();

    /// Clear allocator and use flag.
    void Clear();

    /// Rectangle allocator.
    FAreaAllocator _allocator;
    /// Shadow map texture.
    TSharedPtr<ATexture> _texture;
    /// Shadow views that use this shadow map.
    TVector<FShadowView*> _shadowViews;
    /// Use flag. When false, clearing the shadow map and rendering the views can be skipped.
    bool _used;
};

}