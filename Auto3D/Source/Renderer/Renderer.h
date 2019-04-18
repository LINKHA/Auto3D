#pragma once

#include "../Base/AutoPtr.h"
#include "../Graphics/Texture.h"
#include "../Math/Color.h"
#include "../Math/Frustum.h"
#include "../Resource/Image.h"
#include "Batch.h"

namespace Auto3D
{

class ConstantBuffer;
class GeometryNode;
class Octree;
class Scene;
class VertexBuffer;

/// Shader constant buffers used by high-level rendering.
namespace RendererConstantBuffer
{
	enum Type
	{
		FRAME = 0,
		OBJECT,
		MATERIAL,
		LIGHTS
	};
};


/// Parameter indices in constant buffers used by high-level rendering.
static const size_t VS_FRAME_VIEW_MATRIX = 0;
static const size_t VS_FRAME_PROJECTION_MATRIX = 1;
static const size_t VS_FRAME_VIEWPROJ_MATRIX = 2;
static const size_t VS_FRAME_DEPTH_PARAMETERS = 3;
static const size_t VS_OBJECT_WORLD_MATRIX = 0;
static const size_t VS_LIGHT_SHADOW_MATRICES = 0;
static const size_t PS_FRAME_AMBIENT_COLOR = 0;
static const size_t PS_LIGHT_POSITIONS = 0;
static const size_t PS_LIGHT_DIRECTIONS = 1;
static const size_t PS_LIGHT_ATTENUATIONS = 2;
static const size_t PS_LIGHT_COLORS = 3;
static const size_t PS_LIGHT_SHADOW_PARAMETERS = 4;
static const size_t PS_LIGHT_DIR_SHADOW_SPLITS = 5;
static const size_t PS_LIGHT_DIR_SHADOW_FADE = 6;
static const size_t PS_LIGHT_POINT_SHADOW_PARAMETERS = 7;

/// Texture coordinate index for the instance world matrix.
static const size_t INSTANCE_TEXCOORD = 4;

/// Description of a pass from the client to the renderer.
struct AUTO_API PassDesc
{
    /// Construct undefined.
    PassDesc()
    {
    }
    
    /// Construct with parameters.
    PassDesc(const String& name, BatchSortMode::Type sort = BatchSortMode::STATE, bool lit = true) :
        _name(name),
        _sort(sort),
        _lit(lit)
    {
    }

    /// %Pass name.
    String _name;
    /// Sorting mode.
    BatchSortMode::Type _sort;
    /// Lighting flag.
    bool _lit;
};

/// High-level rendering subsystem. Performs rendering of 3D scenes.
class AUTO_API Renderer : public BaseSubsystem
{
    REGISTER_OBJECT_CLASS(Renderer , BaseSubsystem)

public:
    /// Construct and register subsystem.
    Renderer();
    /// Destruct.
    ~Renderer();
	/// Render scene
	void Render(Scene* scene, Camera* camera);
    /// Set number, _size and format of shadow maps. These will be divided among the lights that need to render shadow maps.
    void SetupShadowMaps(size_t num, int _size, ImageFormat::Type _format);
    /// Prepare a view for rendering. Convenience function that calls CollectObjects(), CollectLightInteractions() and CollectBatches() in one go. Return true on success.
    bool PrepareView(Scene* scene, Camera* camera, const Vector<PassDesc>& passes);
    /// Initialize rendering of a new view and collect visible objects from the camera's point of view. Return true on success (scene, camera and octree are non-null.)
    bool CollectObjects(Scene* scene, Camera* camera);
    /// Collect light interactions with geometries from the current view. If lights are shadowed, collects batches for shadow casters.
    void CollectLightInteractions();
    /// Collect and sort batches from the visible objects. To not go through the objects several times, all the passes should be specified at once instead of multiple calls to CollectBatches().
    void CollectBatches(const Vector<PassDesc>& passes);
    /// Collect and sort batches from the visible objects. Convenience function for one pass only.
    void CollectBatches(const PassDesc& pass);
    /// Render shadow maps. Should be called after all CollectBatches() calls but before RenderBatches(). Note that you must reassign your rendertarget and viewport after calling this.
    void RenderShadowMaps();
    /// Render several passes to the currently set rendertarget and viewport. Avoids setting the per-frame constants multiple times.
    void RenderBatches(const Vector<PassDesc>& passes);
    /// Render a pass to the currently set rendertarget and viewport. Convenience function for one pass only.
    void RenderBatches(const String& pass);

    /// Per-frame vertex shader constant buffer.
    SharedPtr<ConstantBuffer> _vsFrameConstantBuffer;
    /// Per-frame pixel shader constant buffer.
    SharedPtr<ConstantBuffer> _psFrameConstantBuffer;
    /// Per-object vertex shader constant buffer.
    SharedPtr<ConstantBuffer> _vsObjectConstantBuffer;
    /// Lights vertex shader constant buffer.
    SharedPtr<ConstantBuffer> _vsLightConstantBuffer;
    /// Lights pixel shader constant buffer.
    SharedPtr<ConstantBuffer> _psLightConstantBuffer;
private:
    /// Initialize. Needs the Graphics subsystem and rendering context to exist.
    void Initialize();
    /// (Re)define face selection textures.
    void DefineFaceSelectionTextures();
    /// Octree callback for collecting lights and geometries.
    void CollectGeometriesAndLights(Vector<OctreeNode*>::ConstIterator begin, Vector<OctreeNode*>::ConstIterator end, bool inside);
    /// Assign a light list to a node. Creates new light lists as necessary to _handle multiple lights.
    void AddLightToNode(GeometryNode* node, Light* light, LightList* lightList);
    /// Collect shadow caster batches.
    void CollectShadowBatches(const Vector<GeometryNode*>& nodes, BatchQueue& batchQueue, const Frustum& frustum, bool checkShadowCaster, bool checkFrustum);
    /// Render batches from a specific queue and camera.
    void RenderBatches(const Vector<Batch>& batches, Camera* camera, bool setPerFrameContants = true, bool overrideDepthBias = false, int depthBias = 0, float slopeScaledDepthBias = 0.0f);
    /// Load shaders for a pass.
    void LoadPassShaders(Pass* pass);
    /// Return or create a shader variation for a pass. Vertex shader variations _handle different geometry types and pixel shader variations _handle different light combinations.
    ShaderVariation* FindShaderVariation(ShaderStage::Type stage, Pass* pass, unsigned short bits);
    
    /// Graphics subsystem pointer.
    WeakPtr<Graphics> _graphics;
    /// Current scene.
    Scene* _scene;
    /// Current scene camera.
    Camera* _camera;
    /// Current octree.
    Octree* _octree;
    /// Camera's view frustum.
    Frustum _frustum;
    /// Camera's view mask.
    unsigned _viewMask;
    /// Geometries in frustum.
    Vector<GeometryNode*> _geometries;
    /// Lights in frustum.
    Vector<Light*> _lights;
    /// Batch queues per pass.
    HashMap<unsigned char, BatchQueue> _batchQueues;
    /// Instance transforms for uploading to the instance vertex buffer.
    Vector<Matrix3x4F> _instanceTransforms;
    /// Lit geometries query result.
    Vector<GeometryNode*> _litGeometries;
    /// %Light lists.
    HashMap<unsigned long long, LightList> _lightLists;
    /// %Light passes.
    HashMap<unsigned long long, LightPass> _lightPasses;
    /// Ambient only light pass.
    LightPass _ambientLightPass;
    /// Current frame number.
    unsigned _frameNumber;
    /// Instance vertex buffer dirty flag.
    bool _instanceTransformsDirty;
    /// Shadow maps.
    Vector<ShadowMap> _shadowMaps;
    /// Shadow views.
    Vector<AutoPtr<ShadowView> > _shadowViews;
    /// Used shadow views so far.
    size_t _usedShadowViews;
    /// Instance transform vertex buffer.
    AutoPtr<VertexBuffer> _instanceVertexBuffer;
    /// Vertex elements for the instance vertex buffer.
    Vector<VertexElement> _instanceVertexElements;
    /// First point light face selection cube map.
    AutoPtr<Texture> _faceSelectionTexture1;
    /// Second point light face selection cube map.
    AutoPtr<Texture> _faceSelectionTexture2;
};

/// Register Renderer related object factories and attributes.
AUTO_API void RegisterRendererLibrary();

}

