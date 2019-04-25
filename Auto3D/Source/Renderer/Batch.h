#pragma once
#include "../Math/AreaAllocator.h"
#include "Camera.h"
#include "GeometryNode.h"
#include "Material.h"
#include "RenderPath.h"

namespace Auto3D
{

class Light;
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
    /// Material pass.
    Pass* _pass;
    /// Light pass.
    LightPass* _lights;
    /// Geometry type.
    GeometryType::Type _type;

    union
    {
        /// Non-instanced use world matrix.
        const Matrix3x4F* _worldMatrix;
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
    void Sort(Vector<Matrix3x4F>& instanceTransforms);

    /// Build instances from adjacent batches with same state.
    static void BuildInstances(Vector<Batch>& batches, Vector<Matrix3x4F>& instanceTransforms);

    /// Batches, which may be instanced or non-instanced.
    Vector<Batch> _batches;
    /// Additive lighting batches.
    Vector<Batch> _additiveBatches;
    /// Sorting mode.
	RenderCommandSortMode::Type _sort;
    /// Lighting flag.
    bool _lit;
    /// Base pass index.
    unsigned char _baseIndex;
    /// Additive pass index (if needed.)
    unsigned char _additiveIndex;
};

/// %List of lights for a geometry node.
struct AUTO_API LightList
{
    /// %List _key.
    unsigned long long _key;
    /// Lights.
    Vector<Light*> _lights;
    /// Associated light passes.
    Vector<LightPass*> _lightPasses;
    /// Use count
    size_t _useCount;
};

/// Light information for a rendering pass, including properly formatted constant data.
struct AUTO_API LightPass
{
    /// Light positions.
    Vector4F _lightPositions[MAX_LIGHTS_PER_PASS];
    /// Light directions.
    Vector4F _lightDirections[MAX_LIGHTS_PER_PASS];
    /// Light attenuation parameters.
    Vector4F _lightAttenuations[MAX_LIGHTS_PER_PASS];
    /// Light colors.
    Color _lightColors[MAX_LIGHTS_PER_PASS];
    /// Shadow map sampling parameters.
    Vector4F _shadowParameters[MAX_LIGHTS_PER_PASS];
    /// Point light shadow viewport parameters.
    Vector4F _pointShadowParameters[MAX_LIGHTS_PER_PASS];
    /// Directional light shadow split depths.
    Vector4F _dirShadowSplits;
    /// Directional light shadow fade parameters.
    Vector4F _dirShadowFade;
    /// Shadow mapping matrices.
    Matrix4x4F _shadowMatrices[MAX_LIGHTS_PER_PASS];
    /// Shadow maps.
    Texture* _shadowMaps[MAX_LIGHTS_PER_PASS];
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

    /// %Light that is using this view.
    Light* _light;
    /// Viewport within the shadow map.
    RectI _viewport;
    /// Shadow batch queue.
    RenderQueue _shadowQueue;
    /// Shadow camera.
    Camera _shadowCamera;
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
    AreaAllocator _allocator;
    /// Shadow map texture.
    SharedPtr<Texture> _texture;
    /// Shadow views that use this shadow map.
    Vector<ShadowView*> _shadowViews;
    /// Use flag. When false, clearing the shadow map and rendering the views can be skipped.
    bool _used;
};

}