#pragma once

#include "Container/AutoPtr.h"
#include "Graphics/Texture.h"
#include "Math/Color.h"
#include "Math/Frustum.h"
#include "Engine/Components/Image.h"

#include "RenderPath.h"
#include "Batch.h"

namespace Auto3D
{

class FConstantBuffer;
class AGeometryNode;
class AOctree;
class AWorld;
class FVertexBuffer;

/// AShader constant buffers used by high-level rendering.
namespace ERendererConstantBuffer
{
	enum Type
	{
		FRAME = 0,
		OBJECT,
		LIGHTS,
		MATERIAL
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
static const size_t PS_FRAME_VIEW_POSITION = 1;
static const size_t PS_LIGHT_POSITIONS = 0;
static const size_t PS_LIGHT_DIRECTIONS = 1;
static const size_t PS_LIGHT_ATTENUATIONS = 2;
static const size_t PS_LIGHT_COLORS = 3;
static const size_t PS_LIGHT_SHADOW_PARAMETERS = 4;
static const size_t PS_LIGHT_DIR_SHADOW_SPLITS = 5;
static const size_t PS_LIGHT_DIR_SHADOW_FADE = 6;
static const size_t PS_LIGHT_POINT_SHADOW_PARAMETERS = 7;

/// ATexture coordinate index for the instance world matrix.
static const size_t INSTANCE_TEXCOORD = 4;


/// High-level rendering subsystem. Performs rendering of 3D scenes.
class AUTO_API FRendererModule : public FRefCounted
{
public:
    /// Construct and register subsystem.
    FRendererModule();
    /// Destruct.
    ~FRendererModule();

	/// Render scene
	void Render(AWorld* scene, ACamera* camera);
    /// Set number, size and format of shadow maps. These will be divided among the lights that need to render shadow maps.
    void SetupShadowMaps(size_t num, int size, EImageFormat::Type format);
	/// Prepare a view for rendering. Convenience function that calls CollectObjects(), CollectLightInteractions() and CollectBatches() in one go. Return true on success.
    bool PrepareView(AWorld* scene, ACamera* camera, const TVector<FRenderPassDesc>& passes);
    /// Initialize rendering of a new view and collect visible objects from the camera's point of view. Return true on success (scene, camera and octree are non-null.)
    bool CollectObjects(AWorld* scene, ACamera* camera);
    /// Collect light interactions with geometries from the current view. If lights are shadowed, collects batches for shadow casters.
    void CollectLightInteractions();
    /// Collect and sort batches from the visible objects. To not go through the objects several times, all the passes should be specified at once instead of multiple calls to CollectBatches().
    void CollectBatches(const TVector<FRenderPassDesc>& passes);
    /// Collect and sort batches from the visible objects. Convenience function for one pass only.
    void CollectBatches(const FRenderPassDesc& pass);
	/// Render shadow maps. Should be called after all CollectBatches() calls but before RenderBatches(). Note that you must reassign your rendertarget and viewport after calling this.
    void RenderShadowMaps();
    /// Render several passes to the currently set rendertarget and viewport. Avoids setting the per-frame constants multiple times.
    void RenderBatches(const TVector<FRenderPassDesc>& passes);
    /// Render a pass to the currently set rendertarget and viewport. Convenience function for one pass only.
    void RenderBatches(const FString& pass);

    /// Per-frame vertex shader constant buffer.
    TSharedPtr<FConstantBuffer> _vsFrameConstantBuffer;
    /// Per-frame pixel shader constant buffer.
    TSharedPtr<FConstantBuffer> _psFrameConstantBuffer;
    /// Per-object vertex shader constant buffer.
    TSharedPtr<FConstantBuffer> _vsObjectConstantBuffer;
    /// Lights vertex shader constant buffer.
    TSharedPtr<FConstantBuffer> _vsLightConstantBuffer;
    /// Lights pixel shader constant buffer.
    TSharedPtr<FConstantBuffer> _psLightConstantBuffer;

	/// Start up event;
	FEvent _startUpEvent;
private:
    /// Initialize. Needs the AGraphics subsystem and rendering context to exist.
    void Initialize();
    /// (Re)define face selection textures.
    void DefineFaceSelectionTextures();
    /// AOctree callback for collecting lights and geometries.
    void CollectGeometriesAndLights(TVector<AOctreeNode*>::ConstIterator begin, TVector<AOctreeNode*>::ConstIterator end, bool inside);
    /// Assign a light list to a node. Creates new light lists as necessary to _handle multiple lights.
    void AddLightToNode(AGeometryNode* node, ALight* light, FLightList* lightList);
    /// Collect shadow caster batches.
    void CollectShadowBatches(const TVector<AGeometryNode*>& nodes, FRenderQueue& batchQueue, const FFrustum& frustum, bool checkShadowCaster, bool checkFrustum);
    /// Render batches from a specific queue and camera.
    void RenderBatches(const TVector<FBatch>& batches, ACamera* camera, bool setPerFrameContants = true, bool overrideDepthBias = false, int depthBias = 0, float slopeScaledDepthBias = 0.0f);
    /// Load shaders for a pass.
    void LoadPassShaders(FPass* pass);
    /// Return or create a shader variation for a pass. Vertex shader variations _handle different geometry types and pixel shader variations _handle different light combinations.
    FShaderVariation* FindShaderVariation(EShaderStage::Type stage, FPass* pass, unsigned short bits);
    
    /// AGraphics subsystem pointer.
    TWeakPtr<FGraphicsModule> _graphics;
    /// Current scene.
    AWorld* _world;
    /// Current scene camera.
    ACamera* _camera;
    /// Current octree.
    AOctree* _octree;
    /// ACamera's view frustum.
    FFrustum _frustum;
    /// ACamera's view mask.
    unsigned _viewLayoutMask;
    /// Geometries in frustum.
    TVector<AGeometryNode*> _geometries;
    /// Lights in frustum.
    TVector<ALight*> _lights;
    /// FBatch queues per pass.
    THashMap<unsigned char, FRenderQueue> _batchQueues;
    /// Instance transforms for uploading to the instance vertex buffer.
    TVector<TMatrix3x4F> _instanceTransforms;
    /// Lit geometries query result.
    TVector<AGeometryNode*> _litGeometries;
    /// %ALight lists.
    THashMap<unsigned long long, FLightList> _lightLists;
    /// %ALight passes.
    THashMap<unsigned long long, FLightPass> _lightPasses;
    /// Ambient only light pass.
    FLightPass _ambientLightPass;
    /// Current frame number.
    unsigned _frameNumber;
    /// Instance vertex buffer dirty flag.
    bool _instanceTransformsDirty;
    /// Shadow maps.
    TVector<FShadowMap> _shadowMaps;
    /// Shadow views.
    TVector<TAutoPtr<FShadowView> > _shadowViews;
    /// Used shadow views so far.
    size_t _usedShadowViews;
    /// Instance transform vertex buffer.
    TAutoPtr<FVertexBuffer> _instanceVertexBuffer;
    /// Vertex elements for the instance vertex buffer.
    TVector<FVertexElement> _instanceVertexElements;
    /// First point light face selection cube map.
    TAutoPtr<ATexture> _faceSelectionTexture1;
    /// Second point light face selection cube map.
    TAutoPtr<ATexture> _faceSelectionTexture2;
};

}

