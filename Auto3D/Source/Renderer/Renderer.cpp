#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/Graphics.h"
#include "Graphics/Shader.h"
#include "Graphics/ShaderVariation.h"
#include "Graphics/Texture.h"
#include "Graphics/VertexBuffer.h"
#include "Resource/ResourceCache.h"
#include "Scene/Scene.h"
#include "Math/Matrix4x4.h"
#include "Math/Matrix3x4.h"

#include "RenderPath.h"
#include "Engine/Components/Light.h"
#include "Engine/Components/Material.h"
#include "Engine/Components/IBLMaterial.h"
#include "Engine/Components/Model.h"
#include "Scene/Octree.h"
#include "Renderer.h"
#include "Engine/Components/StaticModel.h"
#include "Engine/Components/SkyBox.h"
#include "Engine/Components/Font.h"

#include "Core/Modules/ModuleManager.h"
#include "Debug/DebugNew.h"

namespace Auto3D
{

static const unsigned LVS_GEOMETRY = (0x1 | 0x2);
static const unsigned LVS_NUMSHADOWCOORDS = (0x4 | 0x8 | 0x10);

static const unsigned LPS_AMBIENT = 0x1;
static const unsigned LPS_NUMSHADOWCOORDS = (0x2 | 0x4 | 0x8);
static const unsigned LPS_LIGHT0 = (0x10 | 0x20 | 0x40);
static const unsigned LPS_LIGHT1 = (0x80 | 0x100 | 0x200);
static const unsigned LPS_LIGHT2 = (0x400 | 0x800 | 0x1000);
static const unsigned LPS_LIGHT3 = (0x2000 | 0x4000 | 0x8000);

static const CullMode::Type cullModeFlip[] =
{
    CullMode::NONE,
    CullMode::NONE,
    CullMode::BACK,
    CullMode::FRONT
};

const FString geometryDefines[] =
{
    "",
    "INSTANCED"
};

const FString lightDefines[] = 
{
    "AMBIENT",
    "NUMSHADOWCOORDS",
    "DIRLIGHT",
    "POINTLIGHT",
    "SPOTLIGHT",
    "SHADOW"
};

inline bool CompareLights(Light* lhs, Light* rhs)
{
    return lhs->Distance() < rhs->Distance();
}

Renderer::Renderer() :
	_camera(nullptr),
	_octree(nullptr),
	_scenes(nullptr),
	_usedShadowViews(0),
	_viewLayoutMask(NULL),
    _frameNumber(0),
    _instanceTransformsDirty(false)
{
}

Renderer::~Renderer()
{
}

void Renderer::Render(Scene* scene, Camera* camera)
{
	PROFILE(RenderScene);
	TVector<RenderPassDesc> passes;
	passes.Push(RenderPassDesc("opaque", RenderCommandSortMode::FRONT_TO_BACK, true));
	passes.Push(RenderPassDesc("alpha", RenderCommandSortMode::BACK_TO_FRONT, true));

	PrepareView(scene, camera, passes);

	RenderShadowMaps();

	_graphics->ResetRenderTargets();
	_graphics->ResetViewport();
	_graphics->Clear(CLEAR_COLOR | CLEAR_DEPTH | CLEAR_STENCIL, Color::BLACK);

	RenderBatches(passes);

}
void Renderer::SetupShadowMaps(size_t num, int size, ImageFormat::Type format)
{
    if (size < 1)
        size = 1;
    size = NextPowerOfTwo(size);

    _shadowMaps.Resize(num);
    for (auto it = _shadowMaps.Begin(); it != _shadowMaps.End(); ++it)
    {
        if (it->_texture->Define(TextureType::TEX_2D, ResourceUsage::RENDERTARGET, Vector2I(size, size), format, 1))
        {
            // Setup shadow map sampling with hardware depth compare
            it->_texture->DefineSampler(TextureFilterMode::COMPARE_BILINEAR, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP, 1);
        }
    }
}

bool Renderer::PrepareView(Scene* scene, Camera* camera, const TVector<RenderPassDesc>& passes)
{
	if (!_graphics)
		Initialize();

    if (!CollectObjects(scene, camera))
        return false;

    CollectLightInteractions();
    CollectBatches(passes);
    return true;
}

bool Renderer::CollectObjects(Scene* scene, Camera* camera)
{
    PROFILE(CollectObjects);

    // Acquire Graphics subsystem now, which needs to be initialized with a screen mode
    _geometries.Clear();
    _lights.Clear();
    _instanceTransforms.Clear();
    _lightLists.Clear();
    _lightPasses.Clear();
    for (auto it = _batchQueues.Begin(); it != _batchQueues.End(); ++it)
        it->_second.Clear();
    for (auto it = _shadowMaps.Begin(); it != _shadowMaps.End(); ++it)
        it->Clear();
    _usedShadowViews = 0;

    _scenes = scene;
    _camera = camera;
    _octree = _scenes ? _scenes->FindChild<Octree>() : nullptr;
    if (!_scenes || !_camera || !_octree)
        return false;

    // Increment frame number. Never use 0, as that is the default for objects that have never been rendered
    ++_frameNumber;
    if (!_frameNumber)
        ++_frameNumber;

    // Reinsert moved objects to the octree
    _octree->Update();

    _frustum = _camera->GetWorldFrustum();
    _viewLayoutMask = _camera->GetViewMask();
    _octree->FindNodes(_frustum, this, &Renderer::CollectGeometriesAndLights);

    return true;
}

void Renderer::CollectLightInteractions()
{
    PROFILE(CollectLightInteractions);

    {
        // Sort lights by increasing distance. Directional lights will have distance 0 which ensure they have the first
        // opportunity to allocate shadow maps
        PROFILE(SortLights);
        Sort(_lights.Begin(), _lights.End(), CompareLights);
    }

    for (auto it = _lights.Begin(), end = _lights.End(); it != end; ++it)
    {
        Light* light = *it;
        unsigned lightMask = light->GetLightMask();

        _litGeometries.Clear();
        bool hasReceivers = false;

        // Create a light list that contains only this light. It will be used for nodes that have no light interactions so far
        unsigned long long key = (unsigned long long)light;
        LightList* lightList = &_lightLists[key];
        lightList->_lights.Push(light);
        lightList->_key = key;
        lightList->_useCount = 0;
        
        switch (light->GetLightType())
        {
        case LightType::DIRECTIONAL:
            for (auto gIt = _geometries.Begin(), gEnd = _geometries.End(); gIt != gEnd; ++gIt)
            {
                GeometryNode* node = *gIt;
                if (node->GetLayerMask() & lightMask)
                {
                    AddLightToNode(node, light, lightList);
                    hasReceivers = true;
                }
            }
            break;

        case LightType::POINT:
            _octree->FindNodes(reinterpret_cast<TVector<OctreeNode*>&>(_litGeometries), light->GetWorldSphere(), NF_ENABLED |
                NF_GEOMETRY, lightMask);
            for (auto gIt = _litGeometries.Begin(), gEnd = _litGeometries.End(); gIt != gEnd; ++gIt)
            {
                GeometryNode* node = *gIt;
                // Add light only to nodes which are actually inside the frustum this frame
                if (node->LastFrameNumber() == _frameNumber)
                {
                    AddLightToNode(node, light, lightList);
                    hasReceivers = true;
                }
            }
            break;

        case LightType::SPOT:
            _octree->FindNodes(reinterpret_cast<TVector<OctreeNode*>&>(_litGeometries), light->GetWorldFrustum(), NF_ENABLED |
                NF_GEOMETRY, lightMask);
            for (auto gIt = _litGeometries.Begin(), gEnd = _litGeometries.End(); gIt != gEnd; ++gIt)
            {
                GeometryNode* node = *gIt;
                if (node->LastFrameNumber() == _frameNumber)
                {
                    AddLightToNode(node, light, lightList);
                    hasReceivers = true;
                }
            }
            break;
        }

        if (!light->CastShadows() || !hasReceivers)
        {
            light->SetShadowMap(nullptr);
            continue;
        }

        // Try to allocate shadow map rectangle. Retry with smaller _size two times if fails
        RectI shadowRect;
        Vector2I request = light->GetTotalShadowMapSize();
        size_t retries = 3;
        size_t index = 0;

        while (retries--)
        {
            for (index = 0; index < _shadowMaps.Size(); ++index)
            {
                ShadowMap& shadowMap = _shadowMaps[index];
                int x, y;
                if (shadowMap._allocator.Allocate(request._x, request._y, x, y))
                {
                    light->SetShadowMap(_shadowMaps[index]._texture, RectI(x, y, x + request._x, y + request._y));
                    break;
                }
            }

            if (index < _shadowMaps.Size())
                break;
            else
            {
                request._x /= 2;
                request._y /= 2;
            }
        }

        // If no room in any shadow map, render unshadowed
        if (index >= _shadowMaps.Size())
        {
            light->SetShadowMap(nullptr);
            continue;
        }

        // Setup shadow cameras & find shadow casters
        size_t startIndex = _usedShadowViews;
        light->SetupShadowViews(_camera, _shadowViews, _usedShadowViews);
        bool hasShadowBatches = false;

        for (size_t i = startIndex; i < _usedShadowViews; ++i)
        {
            ShadowView* view = _shadowViews[i].Get();
            Frustum shadowFrustum = view->_shadowCamera.GetWorldFrustum();
            RenderQueue& shadowQueue = view->_shadowQueue;
            shadowQueue._sort = RenderCommandSortMode::STATE;
            shadowQueue._lit = false;
            shadowQueue._baseIndex = Material::PassIndex("shadow");
            shadowQueue._additiveIndex = 0;

            switch (light->GetLightType())
            {
            case LightType::DIRECTIONAL:
                // Directional light needs a new frustum query for each split, as the shadow cameras are typically far outside
                // the main view
                _litGeometries.Clear();
                _octree->FindNodes(reinterpret_cast<TVector<OctreeNode*>&>(_litGeometries),
                    shadowFrustum, NF_ENABLED | NF_GEOMETRY | NF_CASTSHADOWS, light->GetLightMask());
                CollectShadowBatches(_litGeometries, shadowQueue, shadowFrustum, false, false);
                break;

            case LightType::POINT:
                // Check which lit geometries are shadow casters and inside each shadow frustum. First check whether the
                // shadow frustum is inside the view at all
                /// \todo Could use a frustum-frustum test for more accuracy
                if (_frustum.IsInsideFast(BoundingBoxF(shadowFrustum)))
                    CollectShadowBatches(_litGeometries, shadowQueue, shadowFrustum, true, true);
                break;

            case LightType::SPOT:
                // For spot light only need to check which lit geometries are shadow casters
                CollectShadowBatches(_litGeometries, shadowQueue, shadowFrustum, true, false);
                break;
            }

            shadowQueue.Sort(_instanceTransforms);

            // Mark shadow map for rendering only if it has a view with some batches
            if (shadowQueue._batches.Size())
            {
                _shadowMaps[index]._shadowViews.Push(view);
                _shadowMaps[index]._used = true;
                hasShadowBatches = true;
            }
        }

        if (!hasShadowBatches)
        {
            // Light did not have any shadow batches: convert to unshadowed and reuse the views
            light->SetShadowMap(nullptr);
            _usedShadowViews = startIndex;
        }
    }

    {
        PROFILE(BuildLightPasses);

        for (auto it = _lightLists.Begin(), end = _lightLists.End(); it != end; ++it)
        {
            LightList& list = it->_second;
            if (!list._useCount)
                continue;

            // Sort lights according to the light pointer to prevent camera angle from changing the light list order and
            // causing extra shader variations to be compiled
            Sort(list._lights.Begin(), list._lights.End());

            size_t lightsLeft = list._lights.Size();
            static TVector<bool> lightDone;
            static TVector<Light*> currentPass;
            lightDone.Resize(lightsLeft);
            for (size_t i = 0; i < lightsLeft; ++i)
                lightDone[i] = false;
            
            size_t index = 0;
            while (lightsLeft)
            {
                // Find lights to the current pass, while obeying rules for shadow coord allocations (shadowed directional & spot
                // lights can not share a pass)
                currentPass.Clear();
                size_t startIndex = index;
                size_t shadowCoordsLeft = MAX_LIGHTS_PER_PASS;
                while (lightsLeft && currentPass.Size() < MAX_LIGHTS_PER_PASS)
                {
                    if (!lightDone[index])
                    {
                        Light* light = list._lights[index];
                        size_t shadowCoords = light->GetNumShadowCoords();
                        if (shadowCoords <= shadowCoordsLeft)
                        {
                            lightDone[index] = true;
                            currentPass.Push(light);
                            shadowCoordsLeft -= shadowCoords;
                            --lightsLeft;
                        }
                    }

                    index = (index + 1) % list._lights.Size();
                    if (index == startIndex)
                        break;
                }

                unsigned long long passKey = 0;
                for (size_t i = 0; i < currentPass.Size(); ++i)
                    passKey += (unsigned long long)currentPass[i] << (i * 16);
                if (list._lightPasses.IsEmpty())
                    ++passKey; // First pass includes ambient light

                THashMap<unsigned long long, LightPass>::Iterator lpIt = _lightPasses.Find(passKey);
                if (lpIt != _lightPasses.End())
                    list._lightPasses.Push(&lpIt->_second);
                else
                {
                    LightPass* newLightPass = &_lightPasses[passKey];
                    newLightPass->_vsBits = 0;
                    newLightPass->_psBits = list._lightPasses.IsEmpty() ? LPS_AMBIENT : 0;
                    for (size_t i = 0; i < MAX_LIGHTS_PER_PASS; ++i)
                        newLightPass->_shadowMaps[i] = nullptr;

                    size_t numShadowCoords = 0;
                    for (size_t i = 0; i < currentPass.Size(); ++i)
                    {
                        Light* light = currentPass[i];
                        newLightPass->_psBits |= (light->GetLightType() + 1) << (i * 3 + 4);

                        float cutoff = cosf(light->GetFov() * 0.5f * M_DEGTORAD);
                        newLightPass->_lightPositions[i] = Vector4F(light->GetWorldPosition(), 1.0f);
                        newLightPass->_lightDirections[i] = Vector4F(-light->GetWorldDirection(), 0.0f);
                        newLightPass->_lightAttenuations[i] = Vector4F(1.0f / Max(light->GetRange(), M_EPSILON), cutoff, 1.0f /
                            (1.0f - cutoff), 0.0f);
                        newLightPass->_lightColors[i] = light->GetColor();

                        if (light->GetShadowMap())
                        {
                            // Enable shadowed shader variation, setup shadow parameters
                            newLightPass->_psBits |= 4 << (i * 3 + 4);
                            newLightPass->_shadowMaps[i] = light->GetShadowMap();

                            const TVector<Matrix4x4F>& shadowMatrices = light->GetShadowMatrices();
                            for (size_t j = 0; j < shadowMatrices.Size() && numShadowCoords < MAX_LIGHTS_PER_PASS; ++j)
                                newLightPass->_shadowMatrices[numShadowCoords++] = shadowMatrices[j];

                            newLightPass->_shadowParameters[i] = light->GetShadowParameters();

                            if (light->GetLightType() == LightType::DIRECTIONAL)
                            {
                                float fadeStart = light->GetShadowFadeStart() * light->GetMaxShadowDistance() / _camera->GetFarClip();
                                float fadeRange = light->GetMaxShadowDistance() / _camera->GetFarClip() - fadeStart;
                                newLightPass->_dirShadowSplits = light->GetShadowSplits() / _camera->GetFarClip();
                                newLightPass->_dirShadowFade = Vector4F(fadeStart / fadeRange, 1.0f / fadeRange, 0.0f, 0.0f);
                            }
                            else if (light->GetLightType() == LightType::POINT)
                                newLightPass->_pointShadowParameters[i] = light->GetPointShadowParameters();
                        }

                        newLightPass->_vsBits |= numShadowCoords << 2;
                        newLightPass->_psBits |= numShadowCoords << 1;
                    }

                    list._lightPasses.Push(newLightPass);
                }
            }
        }
    }
}

void Renderer::CollectBatches(const TVector<RenderPassDesc>& passes)
{
    PROFILE(CollectBatches);

    // Setup batch queues for each requested pass
    static TVector<RenderQueue*> currentQueues;
    currentQueues.Resize(passes.Size());
    for (size_t i = 0; i < passes.Size(); ++i)
    {
        const RenderPassDesc& srcPass = passes[i];
        unsigned char baseIndex = Material::PassIndex(srcPass._name);
        RenderQueue* batchQueue = &_batchQueues[baseIndex];
        currentQueues[i] = batchQueue;
        batchQueue->_sort = srcPass._sort;
        batchQueue->_lit = srcPass._lit;
        batchQueue->_baseIndex = baseIndex;
        batchQueue->_additiveIndex = srcPass._lit ? Material::PassIndex(srcPass._name + "add") : 0;
    }

    // Loop through geometry nodes
    for (auto gIt = _geometries.Begin(), gEnd = _geometries.End(); gIt != gEnd; ++gIt)
    {
        GeometryNode* node = *gIt;
        LightList* lightList = node->GetLightList();

        Batch newBatch;
        newBatch._type = node->GetGeometryType();
        newBatch._worldMatrix = &node->GetWorldTransform();

        // Loop through node's geometries
        for (auto bIt = node->GetBatches().Begin(), bEnd = node->GetBatches().End(); bIt != bEnd; ++bIt)
        {
            newBatch._geometry = bIt->_geometry.Get();
            Material* material = bIt->_material.Get();
            assert(material);

            // Loop through requested queues
            for (auto qIt = currentQueues.Begin(); qIt != currentQueues.End(); ++qIt)
            {
                RenderQueue& batchQueue = **qIt;
                newBatch._pass = material->GetPass(batchQueue._baseIndex);
                // Material may not have the requested pass at all, skip further processing as fast as possible in that case
                if (!newBatch._pass)
                    continue;

                newBatch._lights = batchQueue._lit ? lightList ? lightList->_lightPasses[0] : &_ambientLightPass : nullptr;
                if (batchQueue._sort < RenderCommandSortMode::BACK_TO_FRONT)
                    newBatch.CalculateSortKey();
                else
                    newBatch._distance = node->Distance();

                batchQueue._batches.Push(newBatch);

                // Create additive light batches if necessary
                if (batchQueue._lit && lightList && lightList->_lightPasses.Size() > 1)
                {
                    newBatch._pass = material->GetPass(batchQueue._additiveIndex);
                    if (!newBatch._pass)
                        continue;

                    for (size_t i = 1; i < lightList->_lightPasses.Size(); ++i)
                    {
                        newBatch._lights = lightList->_lightPasses[i];
                        if (batchQueue._sort != RenderCommandSortMode::BACK_TO_FRONT)
                        {
                            newBatch.CalculateSortKey();
                            batchQueue._additiveBatches.Push(newBatch);
                        }
                        else
                        {
                            // In back-to-front mode base and additive batches must be mixed. Manipulate distance to make
                            // the additive batches render later
                            newBatch._distance = node->Distance() * 0.99999f;
                            batchQueue._batches.Push(newBatch);
                        }
                    }
                }
            }
        }
    }

    size_t oldSize = _instanceTransforms.Size();

    for (auto qIt = currentQueues.Begin(); qIt != currentQueues.End(); ++qIt)
    {
        RenderQueue& batchQueue = **qIt;
        batchQueue.Sort(_instanceTransforms);
    }

    // Check if more instances where added
    if (_instanceTransforms.Size() != oldSize)
        _instanceTransformsDirty = true;
}

void Renderer::CollectBatches(const RenderPassDesc& pass)
{
    static TVector<RenderPassDesc> passDescs(1);
    passDescs[0] = pass;
    CollectBatches(passDescs);
}

void Renderer::RenderShadowMaps()
{
    PROFILE(RenderShadowMaps);

    // Make sure the shadow maps are not bound on any unit
    _graphics->ResetTextures();

    for (auto it = _shadowMaps.Begin(); it != _shadowMaps.End(); ++it)
    {
        if (!it->_used)
            continue;

        _graphics->SetRenderTarget(nullptr, it->_texture);
        _graphics->Clear(CLEAR_DEPTH, Color::BLACK, 1.0f);

        for (auto vIt = it->_shadowViews.Begin(); vIt < it->_shadowViews.End(); ++vIt)
        {
            ShadowView* view = *vIt;
            Light* light = view->_light;
            _graphics->SetViewport(view->_viewport);
            RenderBatches(view->_shadowQueue._batches, &view->_shadowCamera, true, true, light->GetDepthBias(), light->GetSlopeScaledDepthBias());
        }
    }
}

void Renderer::RenderBatches(const TVector<RenderPassDesc>& passes)
{
    PROFILE(RenderBatches);

    for (size_t i = 0; i < passes.Size(); ++i)
    {
        unsigned char passIndex = Material::PassIndex(passes[i]._name);
        RenderQueue& batchQueue = _batchQueues[passIndex];
        RenderBatches(batchQueue._batches, _camera, i == 0);

		// Just remove additive batches to facilitate debugging
        /*RenderBatches(batchQueue._additiveBatches, _camera, false);*/
    }
}

void Renderer::RenderBatches(const FString& pass)
{
    PROFILE(RenderBatches);

    unsigned char passIndex = Material::PassIndex(pass);
    RenderQueue& batchQueue = _batchQueues[passIndex];
    RenderBatches(batchQueue._batches, _camera);

	// Just remove additive batches to facilitate debugging
    /*RenderBatches(batchQueue._additiveBatches, _camera, false);*/
}

void Renderer::Initialize()
{
	_graphics = GModuleManager::Get().GraphicsModule();
    assert(_graphics && _graphics->IsInitialized());

    TVector<Constant> constants;

    _vsFrameConstantBuffer = new ConstantBuffer();
    constants.Push(Constant(ElementType::MATRIX3X4, "viewMatrix"));
    constants.Push(Constant(ElementType::MATRIX4, "projectionMatrix"));
    constants.Push(Constant(ElementType::MATRIX4, "viewProjMatrix"));
    constants.Push(Constant(ElementType::VECTOR4, "depthParameters"));
    _vsFrameConstantBuffer->Define(ResourceUsage::DEFAULT, constants);

    _psFrameConstantBuffer = new ConstantBuffer();
    constants.Clear();
    constants.Push(Constant(ElementType::VECTOR3, "ambientColor"));
	constants.Push(Constant(ElementType::VECTOR3, "viewPosition"));
    _psFrameConstantBuffer->Define(ResourceUsage::DEFAULT, constants);

    _vsObjectConstantBuffer = new ConstantBuffer();
    constants.Clear();
    constants.Push(Constant(ElementType::MATRIX3X4, "worldMatrix"));
    _vsObjectConstantBuffer->Define(ResourceUsage::DEFAULT, constants);

    _vsLightConstantBuffer = new ConstantBuffer();
    constants.Clear();
    constants.Push(Constant(ElementType::MATRIX4, "shadowMatrices", MAX_LIGHTS_PER_PASS));
    _vsLightConstantBuffer->Define(ResourceUsage::DEFAULT, constants);

    _psLightConstantBuffer = new ConstantBuffer();
    constants.Clear();
    constants.Push(Constant(ElementType::VECTOR4, "lightPositions", MAX_LIGHTS_PER_PASS));
    constants.Push(Constant(ElementType::VECTOR4, "lightDirections", MAX_LIGHTS_PER_PASS));
    constants.Push(Constant(ElementType::VECTOR4, "lightColors", MAX_LIGHTS_PER_PASS));
    constants.Push(Constant(ElementType::VECTOR4, "lightAttenuations", MAX_LIGHTS_PER_PASS));
    constants.Push(Constant(ElementType::VECTOR4, "shadowParameters", MAX_LIGHTS_PER_PASS));
    constants.Push(Constant(ElementType::VECTOR4, "pointShadowParameters", MAX_LIGHTS_PER_PASS));
    constants.Push(Constant(ElementType::VECTOR4, "dirShadowSplits"));
    constants.Push(Constant(ElementType::VECTOR4, "dirShadowFade"));
    _psLightConstantBuffer->Define(ResourceUsage::DEFAULT, constants);

    // Instance vertex buffer contains texcoords 4-6 which define the instances' world matrices
    _instanceVertexBuffer = new VertexBuffer();
    _instanceVertexElements.Push(VertexElement(ElementType::VECTOR4, ElementSemantic::TEXCOORD, INSTANCE_TEXCOORD, true));
    _instanceVertexElements.Push(VertexElement(ElementType::VECTOR4, ElementSemantic::TEXCOORD, INSTANCE_TEXCOORD + 1, true));
    _instanceVertexElements.Push(VertexElement(ElementType::VECTOR4, ElementSemantic::TEXCOORD, INSTANCE_TEXCOORD + 2, true));

    // Setup ambient light only -pass
    _ambientLightPass._vsBits = 0;
    _ambientLightPass._psBits = LPS_AMBIENT;

    // Setup point light face selection textures
    _faceSelectionTexture1 = new Texture();
    _faceSelectionTexture2 = new Texture();
    DefineFaceSelectionTextures();
}

void Renderer::DefineFaceSelectionTextures()
{
    PROFILE(DefineFaceSelectionTextures);

    const float faceSelectionData1[] = { 
        1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f
    };

    const float faceSelectionData2[] = {
        -0.5f, 0.5f, 0.5f, 1.5f,
        0.5f, 0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 1.5f, 1.5f,
        -0.5f, -0.5f, 1.5f, 0.5f,
        0.5f, 0.5f, 2.5f, 1.5f,
        -0.5f, 0.5f, 2.5f, 0.5f
    };

    TVector<ImageLevel> faces1;
    TVector<ImageLevel> faces2;
    for (size_t i = 0; i < MAX_CUBE_FACES; ++i)
    {
        ImageLevel level;
        level._rowSize = 4 * sizeof(float);
        level._data = (unsigned char*)&faceSelectionData1[4 * i];
        faces1.Push(level);
        level._data = (unsigned char*)&faceSelectionData2[4 * i];
        faces2.Push(level);
    }

    _faceSelectionTexture1->Define(TextureType::TEX_CUBE, ResourceUsage::DEFAULT, Vector2I(1, 1), ImageFormat::RGBA32F, 1, &faces1[0]);
    _faceSelectionTexture1->DefineSampler(TextureFilterMode::FILTER_POINT, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP);
    _faceSelectionTexture1->SetDataLost(false);

    _faceSelectionTexture2->Define(TextureType::TEX_CUBE, ResourceUsage::DEFAULT, Vector2I(1, 1), ImageFormat::RGBA32F, 1, &faces2[0]);
    _faceSelectionTexture2->DefineSampler(TextureFilterMode::FILTER_POINT, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP);
    _faceSelectionTexture2->SetDataLost(false);
}

void Renderer::CollectGeometriesAndLights(TVector<OctreeNode*>::ConstIterator begin, TVector<OctreeNode*>::ConstIterator end,
    bool inside)
{
    if (inside)
    {
        for (auto it = begin; it != end; ++it)
        {
            OctreeNode* node = *it;
            unsigned short flags = node->Flags();
            if ((flags & NF_ENABLED) && (flags & (NF_GEOMETRY | NF_LIGHT)) && (node->GetLayerMask() & _viewLayoutMask))
            {
                if (flags & NF_GEOMETRY)
                {
                    GeometryNode* geometry = static_cast<GeometryNode*>(node);
                    geometry->OnPrepareRender(_frameNumber, _camera);
                    _geometries.Push(geometry);
                }
                else
                {
                    Light* light = static_cast<Light*>(node);
                    light->OnPrepareRender(_frameNumber, _camera);
                    _lights.Push(light);
                }
            }
        }
    }
    else
    {
        for (auto it = begin; it != end; ++it)
        {
            OctreeNode* node = *it;
            unsigned short flags = node->Flags();
            if ((flags & NF_ENABLED) && (flags & (NF_GEOMETRY | NF_LIGHT)) && (node->GetLayerMask() & _viewLayoutMask) &&
                _frustum.IsInsideFast(node->WorldBoundingBox()))
            {
                if (flags & NF_GEOMETRY)
                {
                    GeometryNode* geometry = static_cast<GeometryNode*>(node);
                    geometry->OnPrepareRender(_frameNumber, _camera);
                    _geometries.Push(geometry);
                }
                else
                {
                    Light* light = static_cast<Light*>(node);
                    light->OnPrepareRender(_frameNumber, _camera);
                    _lights.Push(light);
                }
            }
        }
    }
}

void Renderer::AddLightToNode(GeometryNode* node, Light* light, LightList* lightList)
{
    LightList* oldList = node->GetLightList();

    if (!oldList)
    {
        // First light assigned on this frame
        node->SetLightList(lightList);
        ++lightList->_useCount;
    }
    else
    {
        // Create new light list based on the node's existing one
        --oldList->_useCount;
        unsigned long long newListKey = oldList->_key;
        newListKey += (unsigned long long)light << ((oldList->_lights.Size() & 3) * 16);
        THashMap<unsigned long long, LightList>::Iterator it = _lightLists.Find(newListKey);
        if (it != _lightLists.End())
        {
            LightList* newList = &it->_second;
            node->SetLightList(newList);
            ++newList->_useCount;
        }
        else
        {
            LightList* newList = &_lightLists[newListKey];
            newList->_key = newListKey;
            newList->_lights = oldList->_lights;
            newList->_lights.Push(light);
            newList->_useCount = 1;
            node->SetLightList(newList);
        }
    }
}

void Renderer::CollectShadowBatches(const TVector<GeometryNode*>& nodes, RenderQueue& batchQueue, const Frustum& frustum,
    bool checkShadowCaster, bool checkFrustum)
{
    Batch newBatch;
    newBatch._lights = nullptr;
    
    for (auto gIt = nodes.Begin(), gEnd = nodes.End(); gIt != gEnd; ++gIt)
    {
        GeometryNode* node = *gIt;
        if (checkShadowCaster && !(node->Flags() & NF_CASTSHADOWS))
            continue;
        if (checkFrustum && !frustum.IsInsideFast(node->WorldBoundingBox()))
            continue;

        // Node was possibly not in the main view. Update geometry first in that case
        if (node->LastFrameNumber() != _frameNumber)
            node->OnPrepareRender(_frameNumber, _camera);

        newBatch._type = node->GetGeometryType();
        newBatch._worldMatrix = &node->GetWorldTransform();

        // Loop through node's geometries
        for (auto bIt = node->GetBatches().Begin(), bEnd = node->GetBatches().End(); bIt != bEnd; ++bIt)
        {
            newBatch._geometry = bIt->_geometry.Get();
            Material* material = bIt->_material.Get();
            assert(material);

            newBatch._pass = material->GetPass(batchQueue._baseIndex);
            // Material may not have the requested pass at all, skip further processing as fast as possible in that case
            if (!newBatch._pass)
                continue;

            newBatch.CalculateSortKey();
            batchQueue._batches.Push(newBatch);
        }
    }
}

void Renderer::RenderBatches(const TVector<Batch>& batches, Camera* camera, bool setPerFrameConstants, bool overrideDepthBias,
	int depthBias, float slopeScaledDepthBias)
{
    if (_faceSelectionTexture1->IsDataLost() || _faceSelectionTexture2->IsDataLost())
        DefineFaceSelectionTextures();

    // Bind point light shadow face selection textures
    _graphics->SetTexture(MAX_MATERIAL_TEXTURE_UNITS + MAX_LIGHTS_PER_PASS, _faceSelectionTexture1);
    _graphics->SetTexture(MAX_MATERIAL_TEXTURE_UNITS + MAX_LIGHTS_PER_PASS + 1, _faceSelectionTexture2);

    // If rendering to a texture on OpenGL, flip the camera vertically to ensure similar texture coordinate addressing
    #ifdef AUTO_OPENGL
    bool flipVertical = camera->GetFlipVertical();
    if (_graphics->RenderTarget(0) || _graphics->DepthStencil())
        camera->SetFlipVertical(!flipVertical);
    #endif

    if (setPerFrameConstants)
    {
        // Set per-frame values to the frame constant buffers
        Matrix3x4F viewMatrix = camera->GetViewMatrix();
        Matrix4x4F projectionMatrix = camera->GetProjectionMatrix();
        Matrix4x4F viewProjMatrix = projectionMatrix * viewMatrix;
        Vector4F depthParameters(Vector4F::ZERO);
        depthParameters._x = camera->GetNearClip();
        depthParameters._y = camera->GetFarClip();
        if (camera->IsOrthographic())
        {
            #ifdef AUTO_OPENGL
            depthParameters._z = 0.5f;
            depthParameters._w = 0.5f;
            #else
            depthParameters._z = 1.0f;
            #endif
        }
        else
            depthParameters._w = 1.0f / _camera->GetFarClip();

        _vsFrameConstantBuffer->SetConstant(VS_FRAME_VIEW_MATRIX, viewMatrix);
        _vsFrameConstantBuffer->SetConstant(VS_FRAME_PROJECTION_MATRIX, projectionMatrix);
        _vsFrameConstantBuffer->SetConstant(VS_FRAME_VIEWPROJ_MATRIX, viewProjMatrix);
        _vsFrameConstantBuffer->SetConstant(VS_FRAME_DEPTH_PARAMETERS, depthParameters);
        _vsFrameConstantBuffer->Apply();

        /// \todo Add also fog settings
        _psFrameConstantBuffer->SetConstant(PS_FRAME_AMBIENT_COLOR, camera->GetAmbientColor());
		_psFrameConstantBuffer->SetConstant(PS_FRAME_VIEW_POSITION, camera->GetPosition());
        _psFrameConstantBuffer->Apply();

        _graphics->SetConstantBuffer(ShaderStage::VS, RendererConstantBuffer::FRAME, _vsFrameConstantBuffer);
        _graphics->SetConstantBuffer(ShaderStage::PS, RendererConstantBuffer::FRAME, _psFrameConstantBuffer);
    }

    if (_instanceTransformsDirty && _instanceTransforms.Size())
    {
        if (_instanceVertexBuffer->GetNumVertices() < _instanceTransforms.Size())
            _instanceVertexBuffer->Define(ResourceUsage::DYNAMIC, NextPowerOfTwo(_instanceTransforms.Size()), _instanceVertexElements, false);
        _instanceVertexBuffer->SetData(0, _instanceTransforms.Size(), &_instanceTransforms[0]);
        _graphics->SetVertexBuffer(1, _instanceVertexBuffer);
        _instanceTransformsDirty = false;
    }
    
    {
        Pass* lastPass = nullptr;
        Material* lastMaterial = nullptr;
        LightPass* lastLights = nullptr;

	
        for (auto it = batches.Begin(); it != batches.End();)
        {
            const Batch& batch = *it;
            bool instanced = batch._type == GeometryType::INSTANCED;

            Pass* pass = batch._pass;
            if (!pass->_shadersLoaded)
                LoadPassShaders(pass);

            // Check that pass is legal
            if (pass->_shaders[ShaderStage::VS].Get() && pass->_shaders[ShaderStage::PS].Get())
            {
                // Get the shader variations
                LightPass* lights = batch._lights;
				
                ShaderVariation* vs = FindShaderVariation(ShaderStage::VS, pass, (unsigned short)batch._type | (lights ? lights->_vsBits : 0));
                ShaderVariation* ps = FindShaderVariation(ShaderStage::PS, pass, lights ? lights->_psBits : 0);

				_graphics->SetShaders(vs, ps);

                Geometry* geometry = batch._geometry;
                assert(geometry);

                // Apply pass render state
                if (pass != lastPass)
                {
                    _graphics->SetColorState(pass->_blendMode, pass->_alphaToCoverage, pass->_colorWriteMask);
                    
                    if (!overrideDepthBias)
                        _graphics->SetDepthState(pass->_depthFunc, pass->_depthWrite, pass->_depthClip);
                    else
                        _graphics->SetDepthState(pass->_depthFunc, pass->_depthWrite, pass->_depthClip, depthBias, slopeScaledDepthBias);
                    
                    if (!camera->GetUseReverseCulling())
                        _graphics->SetRasterizerState(pass->_cullMode, pass->_fillMode);
                    else
                        _graphics->SetRasterizerState(cullModeFlip[pass->_cullMode], pass->_fillMode);

                    lastPass = pass;
                }

                // Apply material render state
                Material* material = pass->Parent();
                if (material != lastMaterial)
                {
                    for (size_t i = 0; i < MAX_MATERIAL_TEXTURE_UNITS; ++i)
                    {
                        if (material->_textures[i])
                            _graphics->SetTexture(i, material->_textures[i]);
                    }
					_graphics->SetConstantBuffer(ShaderStage::VS, RendererConstantBuffer::MATERIAL, material->_constantBuffers[ShaderStage::VS].Get());
                    _graphics->SetConstantBuffer(ShaderStage::PS, RendererConstantBuffer::MATERIAL, material->_constantBuffers[ShaderStage::PS].Get());

                    lastMaterial = material;
                }

                // Apply object render state
                if (geometry->_constantBuffers[ShaderStage::VS])
                    _graphics->SetConstantBuffer(ShaderStage::VS, RendererConstantBuffer::OBJECT, geometry->_constantBuffers[ShaderStage::VS].Get());
				else if (!instanced)
                {
                    _vsObjectConstantBuffer->SetConstant(VS_OBJECT_WORLD_MATRIX, *batch._worldMatrix);
                    _vsObjectConstantBuffer->Apply();
                    _graphics->SetConstantBuffer(ShaderStage::VS, RendererConstantBuffer::OBJECT, _vsObjectConstantBuffer.Get());
                }
				_graphics->SetConstantBuffer(ShaderStage::PS, RendererConstantBuffer::OBJECT, geometry->_constantBuffers[ShaderStage::PS].Get());
               
                // Apply light constant buffers and shadow maps
                if (lights && lights != lastLights)
                {
                    // If light queue is ambient only, no need to update the constants
                    if (lights->_psBits > LPS_AMBIENT)
                    {
                        if (lights->_vsBits & LVS_NUMSHADOWCOORDS)
                        {
                            _vsLightConstantBuffer->SetData(lights->_shadowMatrices);
                            _graphics->SetConstantBuffer(ShaderStage::VS, RendererConstantBuffer::LIGHTS, _vsLightConstantBuffer.Get());
                        }
                        
                        _psLightConstantBuffer->SetData(lights->_lightPositions);
                        _graphics->SetConstantBuffer(ShaderStage::PS, RendererConstantBuffer::LIGHTS, _psLightConstantBuffer.Get());

                        for (size_t i = 0; i < MAX_LIGHTS_PER_PASS; ++i)
                            _graphics->SetTexture(MAX_MATERIAL_TEXTURE_UNITS + i, lights->_shadowMaps[i]);
                    }

                    lastLights = lights;
                }
                
                // Set vertex / index buffers and draw
                if (instanced)
                    geometry->DrawInstanced(_graphics, batch._instanceStart, batch._instanceCount);
                else
                    geometry->Draw(_graphics);
            }

            // Advance. If instanced, skip over the batches that were converted
            it += instanced ? batch._instanceCount : 1;
        }
    }

    // Restore original camera vertical flipping state now
    #ifdef AUTO_OPENGL
    camera->SetFlipVertical(flipVertical);
    #endif
}

void Renderer::LoadPassShaders(Pass* pass)
{
    PROFILE(LoadPassShaders);

	ResourceCache* cache = GModuleManager::Get().CacheModule();
    // Use different extensions for GLSL & HLSL shaders
    #ifdef AUTO_OPENGL
    pass->_shaders[ShaderStage::VS] = cache->LoadResource<Shader>(pass->GetShaderName(ShaderStage::VS) + ".vert");
    pass->_shaders[ShaderStage::PS] = cache->LoadResource<Shader>(pass->GetShaderName(ShaderStage::PS) + ".frag");
    #else
	pass->_shaders[ShaderStage::VS] = cache->LoadResource<Shader>(pass->GetShaderName(ShaderStage::VS) + ".vs");
	pass->_shaders[ShaderStage::PS] = cache->LoadResource<Shader>(pass->GetShaderName(ShaderStage::PS) + ".ps");
    #endif

    pass->_shadersLoaded = true;
}

ShaderVariation* Renderer::FindShaderVariation(ShaderStage::Type stage, Pass* pass, unsigned short bits)
{
    /// \todo Evaluate whether the hash lookup is worth the memory save vs using just straightforward vectors
    THashMap<unsigned short, TWeakPtr<ShaderVariation> >& variations = pass->_shaderVariations[stage];
    THashMap<unsigned short, TWeakPtr<ShaderVariation> >::Iterator it = variations.Find(bits);

    if (it != variations.End())
        return it->_second.Get();
    else
    {
        if (stage == ShaderStage::VS)
        {
            FString vsString = pass->GetCombinedShaderDefines(stage) + " " + geometryDefines[bits & LVS_GEOMETRY];
            if (bits & LVS_NUMSHADOWCOORDS)
                vsString += " " + lightDefines[1] + "=" + FString((bits & LVS_NUMSHADOWCOORDS) >> 2);
            it = variations.Insert(MakePair(bits, TWeakPtr<ShaderVariation>(pass->_shaders[stage]->CreateVariation(vsString.Trimmed()))));
            return it->_second.Get();
        }
        else
        {
            FString psString = pass->GetCombinedShaderDefines(stage);
            if (bits & LPS_AMBIENT)
                psString += " " + lightDefines[0];
			if (bits & LPS_NUMSHADOWCOORDS)
				psString += " " + lightDefines[1] + "=" + FString((bits & LPS_NUMSHADOWCOORDS) >> 1);
				

            for (size_t i = 0; i < MAX_LIGHTS_PER_PASS; ++i)
            {
                unsigned short lightBits = (bits >> (i * 3 + 4)) & 7;
                if (lightBits)
                    psString += " " + lightDefines[(lightBits & 3) + 1] + FString((int)i);
                if (lightBits & 4)
                    psString += " " + lightDefines[5] + FString((int)i);
            }

            it = variations.Insert(MakePair(bits, TWeakPtr<ShaderVariation>(pass->_shaders[stage]->CreateVariation(psString.Trimmed()))));
            return it->_second.Get();
        }
    }
}

void RegisterRendererLibrary()
{
    static bool registered = false;
    if (registered)
        return;
    registered = true;

    // Scene node base attributes are needed
    RegisterSceneLibrary();
    Octree::RegisterObject();
    Camera::RegisterObject();
    OctreeNode::RegisterObject();
    GeometryNode::RegisterObject();
    StaticModel::RegisterObject();
    Light::RegisterObject();
    Material::RegisterObject();
	IBLMaterial::RegisterObject();
    Model::RegisterObject();
	SkyBox::RegisterObject();
	Image::RegisterObject();
	Font::RegisterObject();
}

}
