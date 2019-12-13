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

static const ECullMode::Type cullModeFlip[] =
{
    ECullMode::NONE,
    ECullMode::NONE,
    ECullMode::BACK,
    ECullMode::FRONT
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

inline bool CompareLights(ALight* lhs, ALight* rhs)
{
    return lhs->Distance() < rhs->Distance();
}

FRendererModule::FRendererModule() :
	_camera(nullptr),
	_octree(nullptr),
	_scenes(nullptr),
	_usedShadowViews(0),
	_viewLayoutMask(NULL),
    _frameNumber(0),
    _instanceTransformsDirty(false)
{
}

FRendererModule::~FRendererModule()
{
}

void FRendererModule::Render(AScene* scene, ACamera* camera)
{
	PROFILE(RenderScene);
	TVector<FRenderPassDesc> passes;
	passes.Push(FRenderPassDesc("opaque", ERenderCommandSortMode::FRONT_TO_BACK, true));
	passes.Push(FRenderPassDesc("alpha", ERenderCommandSortMode::BACK_TO_FRONT, true));

	PrepareView(scene, camera, passes);

	RenderShadowMaps();

	_graphics->ResetRenderTargets();
	_graphics->ResetViewport();
	_graphics->Clear(CLEAR_COLOR | CLEAR_DEPTH | CLEAR_STENCIL, FColor::BLACK);

	RenderBatches(passes);

}
void FRendererModule::SetupShadowMaps(size_t num, int size, EImageFormat::Type format)
{
    if (size < 1)
        size = 1;
    size = NextPowerOfTwo(size);

    _shadowMaps.Resize(num);
    for (auto it = _shadowMaps.Begin(); it != _shadowMaps.End(); ++it)
    {
        if (it->_texture->Define(ETextureType::TEX_2D, EResourceUsage::RENDERTARGET, TVector2I(size, size), format, 1))
        {
            // Setup shadow map sampling with hardware depth compare
            it->_texture->DefineSampler(ETextureFilterMode::COMPARE_BILINEAR, ETextureAddressMode::CLAMP, ETextureAddressMode::CLAMP, ETextureAddressMode::CLAMP, 1);
        }
    }
}

bool FRendererModule::PrepareView(AScene* scene, ACamera* camera, const TVector<FRenderPassDesc>& passes)
{
	if (!_graphics)
		Initialize();

    if (!CollectObjects(scene, camera))
        return false;

    CollectLightInteractions();
    CollectBatches(passes);
    return true;
}

bool FRendererModule::CollectObjects(AScene* scene, ACamera* camera)
{
    PROFILE(CollectObjects);

    // Acquire AGraphics subsystem now, which needs to be initialized with a screen mode
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
    _octree = _scenes ? _scenes->FindChild<AOctree>() : nullptr;
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
    _octree->FindNodes(_frustum, this, &FRendererModule::CollectGeometriesAndLights);

    return true;
}

void FRendererModule::CollectLightInteractions()
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
        ALight* light = *it;
        unsigned lightMask = light->GetLightMask();

        _litGeometries.Clear();
        bool hasReceivers = false;

        // Create a light list that contains only this light. It will be used for nodes that have no light interactions so far
        unsigned long long key = (unsigned long long)light;
        FLightList* lightList = &_lightLists[key];
        lightList->_lights.Push(light);
        lightList->_key = key;
        lightList->_useCount = 0;
        
        switch (light->GetLightType())
        {
        case ELightType::DIRECTIONAL:
            for (auto gIt = _geometries.Begin(), gEnd = _geometries.End(); gIt != gEnd; ++gIt)
            {
                AGeometryNode* node = *gIt;
                if (node->GetLayerMask() & lightMask)
                {
                    AddLightToNode(node, light, lightList);
                    hasReceivers = true;
                }
            }
            break;

        case ELightType::POINT:
            _octree->FindNodes(reinterpret_cast<TVector<AOctreeNode*>&>(_litGeometries), light->GetWorldSphere(), NF_ENABLED |
                NF_GEOMETRY, lightMask);
            for (auto gIt = _litGeometries.Begin(), gEnd = _litGeometries.End(); gIt != gEnd; ++gIt)
            {
                AGeometryNode* node = *gIt;
                // Add light only to nodes which are actually inside the frustum this frame
                if (node->LastFrameNumber() == _frameNumber)
                {
                    AddLightToNode(node, light, lightList);
                    hasReceivers = true;
                }
            }
            break;

        case ELightType::SPOT:
            _octree->FindNodes(reinterpret_cast<TVector<AOctreeNode*>&>(_litGeometries), light->GetWorldFrustum(), NF_ENABLED |
                NF_GEOMETRY, lightMask);
            for (auto gIt = _litGeometries.Begin(), gEnd = _litGeometries.End(); gIt != gEnd; ++gIt)
            {
                AGeometryNode* node = *gIt;
                if (node->LastFrameNumber() == _frameNumber)
                {
                    AddLightToNode(node, light, lightList);
                    hasReceivers = true;
                }
            }
            break;
        }

        if (!light->GetCastShadows() || !hasReceivers)
        {
            light->SetShadowMap(nullptr);
            continue;
        }

        // Try to allocate shadow map rectangle. Retry with smaller _size two times if fails
        TRectI shadowRect;
        TVector2I request = light->GetTotalShadowMapSize();
        size_t retries = 3;
        size_t index = 0;

        while (retries--)
        {
            for (index = 0; index < _shadowMaps.Size(); ++index)
            {
                FShadowMap& shadowMap = _shadowMaps[index];
                int x, y;
                if (shadowMap._allocator.Allocate(request._x, request._y, x, y))
                {
                    light->SetShadowMap(_shadowMaps[index]._texture, TRectI(x, y, x + request._x, y + request._y));
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
            FShadowView* view = _shadowViews[i].Get();
            FFrustum shadowFrustum = view->_shadowCamera.GetWorldFrustum();
            FRenderQueue& shadowQueue = view->_shadowQueue;
            shadowQueue._sort = ERenderCommandSortMode::STATE;
            shadowQueue._lit = false;
            shadowQueue._baseIndex = AMaterial::PassIndex("shadow");
            shadowQueue._additiveIndex = 0;

            switch (light->GetLightType())
            {
            case ELightType::DIRECTIONAL:
                // Directional light needs a new frustum query for each split, as the shadow cameras are typically far outside
                // the main view
                _litGeometries.Clear();
                _octree->FindNodes(reinterpret_cast<TVector<AOctreeNode*>&>(_litGeometries),
                    shadowFrustum, NF_ENABLED | NF_GEOMETRY | NF_CASTSHADOWS, light->GetLightMask());
                CollectShadowBatches(_litGeometries, shadowQueue, shadowFrustum, false, false);
                break;

            case ELightType::POINT:
                // Check which lit geometries are shadow casters and inside each shadow frustum. First check whether the
                // shadow frustum is inside the view at all
                /// \todo Could use a frustum-frustum test for more accuracy
                if (_frustum.IsInsideFast(TBoundingBoxF(shadowFrustum)))
                    CollectShadowBatches(_litGeometries, shadowQueue, shadowFrustum, true, true);
                break;

            case ELightType::SPOT:
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
            // ALight did not have any shadow batches: convert to unshadowed and reuse the views
            light->SetShadowMap(nullptr);
            _usedShadowViews = startIndex;
        }
    }

    {
        PROFILE(BuildLightPasses);

        for (auto it = _lightLists.Begin(), end = _lightLists.End(); it != end; ++it)
        {
            FLightList& list = it->_second;
            if (!list._useCount)
                continue;

            // Sort lights according to the light pointer to prevent camera angle from changing the light list order and
            // causing extra shader variations to be compiled
            Sort(list._lights.Begin(), list._lights.End());

            size_t lightsLeft = list._lights.Size();
            static TVector<bool> lightDone;
            static TVector<ALight*> currentPass;
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
                        ALight* light = list._lights[index];
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

                THashMap<unsigned long long, FLightPass>::Iterator lpIt = _lightPasses.Find(passKey);
                if (lpIt != _lightPasses.End())
                    list._lightPasses.Push(&lpIt->_second);
                else
                {
                    FLightPass* newLightPass = &_lightPasses[passKey];
                    newLightPass->_vsBits = 0;
                    newLightPass->_psBits = list._lightPasses.IsEmpty() ? LPS_AMBIENT : 0;
                    for (size_t i = 0; i < MAX_LIGHTS_PER_PASS; ++i)
                        newLightPass->_shadowMaps[i] = nullptr;

                    size_t numShadowCoords = 0;
                    for (size_t i = 0; i < currentPass.Size(); ++i)
                    {
                        ALight* light = currentPass[i];
                        newLightPass->_psBits |= (light->GetLightType() + 1) << (i * 3 + 4);

                        float cutoff = cosf(light->GetFov() * 0.5f * M_DEGTORAD);
                        newLightPass->_lightPositions[i] = TVector4F(light->GetWorldPosition(), 1.0f);
                        newLightPass->_lightDirections[i] = TVector4F(-light->GetWorldDirection(), 0.0f);
                        newLightPass->_lightAttenuations[i] = TVector4F(1.0f / Max(light->GetRange(), M_EPSILON), cutoff, 1.0f /
                            (1.0f - cutoff), 0.0f);
                        newLightPass->_lightColors[i] = light->GetColor();

                        if (light->GetShadowMap())
                        {
                            // Enable shadowed shader variation, setup shadow parameters
                            newLightPass->_psBits |= 4 << (i * 3 + 4);
                            newLightPass->_shadowMaps[i] = light->GetShadowMap();

                            const TVector<TMatrix4x4F>& shadowMatrices = light->GetShadowMatrices();
                            for (size_t j = 0; j < shadowMatrices.Size() && numShadowCoords < MAX_LIGHTS_PER_PASS; ++j)
                                newLightPass->_shadowMatrices[numShadowCoords++] = shadowMatrices[j];

                            newLightPass->_shadowParameters[i] = light->GetShadowParameters();

                            if (light->GetLightType() == ELightType::DIRECTIONAL)
                            {
                                float fadeStart = light->GetShadowFadeStart() * light->GetMaxShadowDistance() / _camera->GetFarClip();
                                float fadeRange = light->GetMaxShadowDistance() / _camera->GetFarClip() - fadeStart;
                                newLightPass->_dirShadowSplits = light->GetShadowSplits() / _camera->GetFarClip();
                                newLightPass->_dirShadowFade = TVector4F(fadeStart / fadeRange, 1.0f / fadeRange, 0.0f, 0.0f);
                            }
                            else if (light->GetLightType() == ELightType::POINT)
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

void FRendererModule::CollectBatches(const TVector<FRenderPassDesc>& passes)
{
    PROFILE(CollectBatches);

    // Setup batch queues for each requested pass
    static TVector<FRenderQueue*> currentQueues;
    currentQueues.Resize(passes.Size());
    for (size_t i = 0; i < passes.Size(); ++i)
    {
        const FRenderPassDesc& srcPass = passes[i];
        unsigned char baseIndex = AMaterial::PassIndex(srcPass._name);
        FRenderQueue* batchQueue = &_batchQueues[baseIndex];
        currentQueues[i] = batchQueue;
        batchQueue->_sort = srcPass._sort;
        batchQueue->_lit = srcPass._lit;
        batchQueue->_baseIndex = baseIndex;
        batchQueue->_additiveIndex = srcPass._lit ? AMaterial::PassIndex(srcPass._name + "add") : 0;
    }

    // Loop through geometry nodes
    for (auto gIt = _geometries.Begin(), gEnd = _geometries.End(); gIt != gEnd; ++gIt)
    {
        AGeometryNode* node = *gIt;
        FLightList* lightList = node->GetLightList();

        FBatch newBatch;
        newBatch._type = node->GetGeometryType();
        newBatch._worldMatrix = &node->GetWorldTransform();

        // Loop through node's geometries
        for (auto bIt = node->GetBatches().Begin(), bEnd = node->GetBatches().End(); bIt != bEnd; ++bIt)
        {
            newBatch._geometry = bIt->_geometry.Get();
            AMaterial* material = bIt->_material.Get();
            assert(material);

            // Loop through requested queues
            for (auto qIt = currentQueues.Begin(); qIt != currentQueues.End(); ++qIt)
            {
                FRenderQueue& batchQueue = **qIt;
                newBatch._pass = material->GetPass(batchQueue._baseIndex);
                // AMaterial may not have the requested pass at all, skip further processing as fast as possible in that case
                if (!newBatch._pass)
                    continue;

                newBatch._lights = batchQueue._lit ? lightList ? lightList->_lightPasses[0] : &_ambientLightPass : nullptr;
                if (batchQueue._sort < ERenderCommandSortMode::BACK_TO_FRONT)
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
                        if (batchQueue._sort != ERenderCommandSortMode::BACK_TO_FRONT)
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
        FRenderQueue& batchQueue = **qIt;
        batchQueue.Sort(_instanceTransforms);
    }

    // Check if more instances where added
    if (_instanceTransforms.Size() != oldSize)
        _instanceTransformsDirty = true;
}

void FRendererModule::CollectBatches(const FRenderPassDesc& pass)
{
    static TVector<FRenderPassDesc> passDescs(1);
    passDescs[0] = pass;
    CollectBatches(passDescs);
}

void FRendererModule::RenderShadowMaps()
{
    PROFILE(RenderShadowMaps);

    // Make sure the shadow maps are not bound on any unit
    _graphics->ResetTextures();

    for (auto it = _shadowMaps.Begin(); it != _shadowMaps.End(); ++it)
    {
        if (!it->_used)
            continue;

        _graphics->SetRenderTarget(nullptr, it->_texture);
        _graphics->Clear(CLEAR_DEPTH, FColor::BLACK, 1.0f);

        for (auto vIt = it->_shadowViews.Begin(); vIt < it->_shadowViews.End(); ++vIt)
        {
            FShadowView* view = *vIt;
            ALight* light = view->_light;
            _graphics->SetViewport(view->_viewport);
            RenderBatches(view->_shadowQueue._batches, &view->_shadowCamera, true, true, light->GetDepthBias(), light->GetSlopeScaledDepthBias());
        }
    }
}

void FRendererModule::RenderBatches(const TVector<FRenderPassDesc>& passes)
{
    PROFILE(RenderBatches);

    for (size_t i = 0; i < passes.Size(); ++i)
    {
        unsigned char passIndex = AMaterial::PassIndex(passes[i]._name);
        FRenderQueue& batchQueue = _batchQueues[passIndex];
        RenderBatches(batchQueue._batches, _camera, i == 0);

		// Just remove additive batches to facilitate debugging
        /*RenderBatches(batchQueue._additiveBatches, _camera, false);*/
    }
}

void FRendererModule::RenderBatches(const FString& pass)
{
    PROFILE(RenderBatches);

    unsigned char passIndex = AMaterial::PassIndex(pass);
    FRenderQueue& batchQueue = _batchQueues[passIndex];
    RenderBatches(batchQueue._batches, _camera);

	// Just remove additive batches to facilitate debugging
    /*RenderBatches(batchQueue._additiveBatches, _camera, false);*/
}

void FRendererModule::Initialize()
{
	_graphics = GModuleManager::Get().GraphicsModule();
    assert(_graphics && _graphics->IsInitialized());

    TVector<FConstant> constants;

    _vsFrameConstantBuffer = new FConstantBuffer();
    constants.Push(FConstant(EElementType::MATRIX3X4, "viewMatrix"));
    constants.Push(FConstant(EElementType::MATRIX4, "projectionMatrix"));
    constants.Push(FConstant(EElementType::MATRIX4, "viewProjMatrix"));
    constants.Push(FConstant(EElementType::VECTOR4, "depthParameters"));
    _vsFrameConstantBuffer->Define(EResourceUsage::DEFAULT, constants);

    _psFrameConstantBuffer = new FConstantBuffer();
    constants.Clear();
    constants.Push(FConstant(EElementType::VECTOR3, "ambientColor"));
	constants.Push(FConstant(EElementType::VECTOR3, "viewPosition"));
    _psFrameConstantBuffer->Define(EResourceUsage::DEFAULT, constants);

    _vsObjectConstantBuffer = new FConstantBuffer();
    constants.Clear();
    constants.Push(FConstant(EElementType::MATRIX3X4, "worldMatrix"));
    _vsObjectConstantBuffer->Define(EResourceUsage::DEFAULT, constants);

    _vsLightConstantBuffer = new FConstantBuffer();
    constants.Clear();
    constants.Push(FConstant(EElementType::MATRIX4, "shadowMatrices", MAX_LIGHTS_PER_PASS));
    _vsLightConstantBuffer->Define(EResourceUsage::DEFAULT, constants);

    _psLightConstantBuffer = new FConstantBuffer();
    constants.Clear();
    constants.Push(FConstant(EElementType::VECTOR4, "lightPositions", MAX_LIGHTS_PER_PASS));
    constants.Push(FConstant(EElementType::VECTOR4, "lightDirections", MAX_LIGHTS_PER_PASS));
    constants.Push(FConstant(EElementType::VECTOR4, "lightColors", MAX_LIGHTS_PER_PASS));
    constants.Push(FConstant(EElementType::VECTOR4, "lightAttenuations", MAX_LIGHTS_PER_PASS));
    constants.Push(FConstant(EElementType::VECTOR4, "shadowParameters", MAX_LIGHTS_PER_PASS));
    constants.Push(FConstant(EElementType::VECTOR4, "pointShadowParameters", MAX_LIGHTS_PER_PASS));
    constants.Push(FConstant(EElementType::VECTOR4, "dirShadowSplits"));
    constants.Push(FConstant(EElementType::VECTOR4, "dirShadowFade"));
    _psLightConstantBuffer->Define(EResourceUsage::DEFAULT, constants);

    // Instance vertex buffer contains texcoords 4-6 which define the instances' world matrices
    _instanceVertexBuffer = new FVertexBuffer();
    _instanceVertexElements.Push(FVertexElement(EElementType::VECTOR4, EElementSemantic::TEXCOORD, INSTANCE_TEXCOORD, true));
    _instanceVertexElements.Push(FVertexElement(EElementType::VECTOR4, EElementSemantic::TEXCOORD, INSTANCE_TEXCOORD + 1, true));
    _instanceVertexElements.Push(FVertexElement(EElementType::VECTOR4, EElementSemantic::TEXCOORD, INSTANCE_TEXCOORD + 2, true));

    // Setup ambient light only -pass
    _ambientLightPass._vsBits = 0;
    _ambientLightPass._psBits = LPS_AMBIENT;

    // Setup point light face selection textures
    _faceSelectionTexture1 = new ATexture();
    _faceSelectionTexture2 = new ATexture();
    DefineFaceSelectionTextures();
}

void FRendererModule::DefineFaceSelectionTextures()
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

    TVector<FImageLevel> faces1;
    TVector<FImageLevel> faces2;
    for (size_t i = 0; i < MAX_CUBE_FACES; ++i)
    {
        FImageLevel level;
        level._rowSize = 4 * sizeof(float);
        level._data = (unsigned char*)&faceSelectionData1[4 * i];
        faces1.Push(level);
        level._data = (unsigned char*)&faceSelectionData2[4 * i];
        faces2.Push(level);
    }

    _faceSelectionTexture1->Define(ETextureType::TEX_CUBE, EResourceUsage::DEFAULT, TVector2I(1, 1), EImageFormat::RGBA32F, 1, &faces1[0]);
    _faceSelectionTexture1->DefineSampler(ETextureFilterMode::FILTER_POINT, ETextureAddressMode::CLAMP, ETextureAddressMode::CLAMP, ETextureAddressMode::CLAMP);
    _faceSelectionTexture1->SetDataLost(false);

    _faceSelectionTexture2->Define(ETextureType::TEX_CUBE, EResourceUsage::DEFAULT, TVector2I(1, 1), EImageFormat::RGBA32F, 1, &faces2[0]);
    _faceSelectionTexture2->DefineSampler(ETextureFilterMode::FILTER_POINT, ETextureAddressMode::CLAMP, ETextureAddressMode::CLAMP, ETextureAddressMode::CLAMP);
    _faceSelectionTexture2->SetDataLost(false);
}

void FRendererModule::CollectGeometriesAndLights(TVector<AOctreeNode*>::ConstIterator begin, TVector<AOctreeNode*>::ConstIterator end,
    bool inside)
{
    if (inside)
    {
        for (auto it = begin; it != end; ++it)
        {
            AOctreeNode* node = *it;
            unsigned short flags = node->Flags();
            if ((flags & NF_ENABLED) && (flags & (NF_GEOMETRY | NF_LIGHT)) && (node->GetLayerMask() & _viewLayoutMask))
            {
                if (flags & NF_GEOMETRY)
                {
                    AGeometryNode* geometry = static_cast<AGeometryNode*>(node);
                    geometry->OnPrepareRender(_frameNumber, _camera);
                    _geometries.Push(geometry);
                }
                else
                {
                    ALight* light = static_cast<ALight*>(node);
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
            AOctreeNode* node = *it;
            unsigned short flags = node->Flags();
            if ((flags & NF_ENABLED) && (flags & (NF_GEOMETRY | NF_LIGHT)) && (node->GetLayerMask() & _viewLayoutMask) &&
                _frustum.IsInsideFast(node->WorldBoundingBox()))
            {
                if (flags & NF_GEOMETRY)
                {
                    AGeometryNode* geometry = static_cast<AGeometryNode*>(node);
                    geometry->OnPrepareRender(_frameNumber, _camera);
                    _geometries.Push(geometry);
                }
                else
                {
                    ALight* light = static_cast<ALight*>(node);
                    light->OnPrepareRender(_frameNumber, _camera);
                    _lights.Push(light);
                }
            }
        }
    }
}

void FRendererModule::AddLightToNode(AGeometryNode* node, ALight* light, FLightList* lightList)
{
    FLightList* oldList = node->GetLightList();

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
        THashMap<unsigned long long, FLightList>::Iterator it = _lightLists.Find(newListKey);
        if (it != _lightLists.End())
        {
            FLightList* newList = &it->_second;
            node->SetLightList(newList);
            ++newList->_useCount;
        }
        else
        {
            FLightList* newList = &_lightLists[newListKey];
            newList->_key = newListKey;
            newList->_lights = oldList->_lights;
            newList->_lights.Push(light);
            newList->_useCount = 1;
            node->SetLightList(newList);
        }
    }
}

void FRendererModule::CollectShadowBatches(const TVector<AGeometryNode*>& nodes, FRenderQueue& batchQueue, const FFrustum& frustum,
    bool checkShadowCaster, bool checkFrustum)
{
    FBatch newBatch;
    newBatch._lights = nullptr;
    
    for (auto gIt = nodes.Begin(), gEnd = nodes.End(); gIt != gEnd; ++gIt)
    {
        AGeometryNode* node = *gIt;
        if (checkShadowCaster && !(node->Flags() & NF_CASTSHADOWS))
            continue;
        if (checkFrustum && !frustum.IsInsideFast(node->WorldBoundingBox()))
            continue;

        // ANode was possibly not in the main view. Update geometry first in that case
        if (node->LastFrameNumber() != _frameNumber)
            node->OnPrepareRender(_frameNumber, _camera);

        newBatch._type = node->GetGeometryType();
        newBatch._worldMatrix = &node->GetWorldTransform();

        // Loop through node's geometries
        for (auto bIt = node->GetBatches().Begin(), bEnd = node->GetBatches().End(); bIt != bEnd; ++bIt)
        {
            newBatch._geometry = bIt->_geometry.Get();
            AMaterial* material = bIt->_material.Get();
            assert(material);

            newBatch._pass = material->GetPass(batchQueue._baseIndex);
            // AMaterial may not have the requested pass at all, skip further processing as fast as possible in that case
            if (!newBatch._pass)
                continue;

            newBatch.CalculateSortKey();
            batchQueue._batches.Push(newBatch);
        }
    }
}

void FRendererModule::RenderBatches(const TVector<FBatch>& batches, ACamera* camera, bool setPerFrameConstants, bool overrideDepthBias,
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
        TMatrix3x4F viewMatrix = camera->GetViewMatrix();
        TMatrix4x4F projectionMatrix = camera->GetProjectionMatrix();
        TMatrix4x4F viewProjMatrix = projectionMatrix * viewMatrix;
        TVector4F depthParameters(TVector4F::ZERO);
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

        _graphics->SetConstantBuffer(EShaderStage::VS, ERendererConstantBuffer::FRAME, _vsFrameConstantBuffer);
        _graphics->SetConstantBuffer(EShaderStage::PS, ERendererConstantBuffer::FRAME, _psFrameConstantBuffer);
    }

    if (_instanceTransformsDirty && _instanceTransforms.Size())
    {
        if (_instanceVertexBuffer->GetNumVertices() < _instanceTransforms.Size())
            _instanceVertexBuffer->Define(EResourceUsage::DYNAMIC, NextPowerOfTwo(_instanceTransforms.Size()), _instanceVertexElements, false);
        _instanceVertexBuffer->SetData(0, _instanceTransforms.Size(), &_instanceTransforms[0]);
        _graphics->SetVertexBuffer(1, _instanceVertexBuffer);
        _instanceTransformsDirty = false;
    }
    
    {
        FPass* lastPass = nullptr;
        AMaterial* lastMaterial = nullptr;
        FLightPass* lastLights = nullptr;

	
        for (auto it = batches.Begin(); it != batches.End();)
        {
            const FBatch& batch = *it;
            bool instanced = batch._type == EGeometryType::INSTANCED;

            FPass* pass = batch._pass;
            if (!pass->_shadersLoaded)
                LoadPassShaders(pass);

            // Check that pass is legal
            if (pass->_shaders[EShaderStage::VS].Get() && pass->_shaders[EShaderStage::PS].Get())
            {
                // Get the shader variations
                FLightPass* lights = batch._lights;
				
                FShaderVariation* vs = FindShaderVariation(EShaderStage::VS, pass, (unsigned short)batch._type | (lights ? lights->_vsBits : 0));
                FShaderVariation* ps = FindShaderVariation(EShaderStage::PS, pass, lights ? lights->_psBits : 0);

				_graphics->SetShaders(vs, ps);

                FGeometry* geometry = batch._geometry;
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
                AMaterial* material = pass->Parent();
                if (material != lastMaterial)
                {
                    for (size_t i = 0; i < MAX_MATERIAL_TEXTURE_UNITS; ++i)
                    {
                        if (material->_textures[i])
                            _graphics->SetTexture(i, material->_textures[i]);
                    }
					_graphics->SetConstantBuffer(EShaderStage::VS, ERendererConstantBuffer::MATERIAL, material->_constantBuffers[EShaderStage::VS].Get());
                    _graphics->SetConstantBuffer(EShaderStage::PS, ERendererConstantBuffer::MATERIAL, material->_constantBuffers[EShaderStage::PS].Get());

                    lastMaterial = material;
                }

                // Apply object render state
                if (geometry->_constantBuffers[EShaderStage::VS])
                    _graphics->SetConstantBuffer(EShaderStage::VS, ERendererConstantBuffer::OBJECT, geometry->_constantBuffers[EShaderStage::VS].Get());
				else if (!instanced)
                {
                    _vsObjectConstantBuffer->SetConstant(VS_OBJECT_WORLD_MATRIX, *batch._worldMatrix);
                    _vsObjectConstantBuffer->Apply();
                    _graphics->SetConstantBuffer(EShaderStage::VS, ERendererConstantBuffer::OBJECT, _vsObjectConstantBuffer.Get());
                }
				_graphics->SetConstantBuffer(EShaderStage::PS, ERendererConstantBuffer::OBJECT, geometry->_constantBuffers[EShaderStage::PS].Get());
               
                // Apply light constant buffers and shadow maps
                if (lights && lights != lastLights)
                {
                    // If light queue is ambient only, no need to update the constants
                    if (lights->_psBits > LPS_AMBIENT)
                    {
                        if (lights->_vsBits & LVS_NUMSHADOWCOORDS)
                        {
                            _vsLightConstantBuffer->SetData(lights->_shadowMatrices);
                            _graphics->SetConstantBuffer(EShaderStage::VS, ERendererConstantBuffer::LIGHTS, _vsLightConstantBuffer.Get());
                        }
                        
                        _psLightConstantBuffer->SetData(lights->_lightPositions);
                        _graphics->SetConstantBuffer(EShaderStage::PS, ERendererConstantBuffer::LIGHTS, _psLightConstantBuffer.Get());

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

void FRendererModule::LoadPassShaders(FPass* pass)
{
    PROFILE(LoadPassShaders);

	FResourceModule* cache = GModuleManager::Get().CacheModule();
    // Use different extensions for GLSL & HLSL shaders
    #ifdef AUTO_OPENGL
    pass->_shaders[EShaderStage::VS] = cache->LoadResource<AShader>(pass->GetShaderName(EShaderStage::VS) + ".vert");
    pass->_shaders[EShaderStage::PS] = cache->LoadResource<AShader>(pass->GetShaderName(EShaderStage::PS) + ".frag");
    #else
	pass->_shaders[EShaderStage::VS] = cache->LoadResource<AShader>(pass->GetShaderName(EShaderStage::VS) + ".vs");
	pass->_shaders[EShaderStage::PS] = cache->LoadResource<AShader>(pass->GetShaderName(EShaderStage::PS) + ".ps");
    #endif

    pass->_shadersLoaded = true;
}

FShaderVariation* FRendererModule::FindShaderVariation(EShaderStage::Type stage, FPass* pass, unsigned short bits)
{
    /// \todo Evaluate whether the hash lookup is worth the memory save vs using just straightforward vectors
    THashMap<unsigned short, TWeakPtr<FShaderVariation> >& variations = pass->_shaderVariations[stage];
    THashMap<unsigned short, TWeakPtr<FShaderVariation> >::Iterator it = variations.Find(bits);

    if (it != variations.End())
        return it->_second.Get();
    else
    {
        if (stage == EShaderStage::VS)
        {
            FString vsString = pass->GetCombinedShaderDefines(stage) + " " + geometryDefines[bits & LVS_GEOMETRY];
            if (bits & LVS_NUMSHADOWCOORDS)
                vsString += " " + lightDefines[1] + "=" + FString((bits & LVS_NUMSHADOWCOORDS) >> 2);
            it = variations.Insert(MakePair(bits, TWeakPtr<FShaderVariation>(pass->_shaders[stage]->CreateVariation(vsString.Trimmed()))));
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

            it = variations.Insert(MakePair(bits, TWeakPtr<FShaderVariation>(pass->_shaders[stage]->CreateVariation(psString.Trimmed()))));
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

    // AScene node base attributes are needed
    RegisterSceneLibrary();
    AOctree::RegisterObject();
    ACamera::RegisterObject();
    AOctreeNode::RegisterObject();
    AGeometryNode::RegisterObject();
    AStaticModel::RegisterObject();
    ALight::RegisterObject();
    AMaterial::RegisterObject();
	AIBLMaterial::RegisterObject();
    AModel::RegisterObject();
	ASkyBox::RegisterObject();
	AImage::RegisterObject();
	AFont::RegisterObject();
}

}
