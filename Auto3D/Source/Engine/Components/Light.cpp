#include "Debug/Log.h"
#include "Math/Ray.h"
#include "Light.h"
#include "Scene/Octree.h"
#include "Renderer/ForwardShadingRenderer.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

static const ELightType::Type DEFAULT_LIGHTTYPE = ELightType::POINT;
static const FColor DEFAULT_COLOR = FColor(1.0f, 1.0f, 1.0f, 0.5f);
static const float DEFAULT_RANGE = 10.0f;
static const float DEFAULT_SPOT_FOV = 30.0f;
static const int DEFAULT_SHADOWMAP_SIZE = 512;
static const TVector4F DEFAULT_SHADOW_SPLITS = TVector4F(10.0f, 50.0f, 150.0f, 0.0f);
static const float DEFAULT_FADE_START = 0.9f;
static const int DEFAULT_DEPTH_BIAS = 5;
static const float DEFAULT_SLOPE_SCALED_DEPTH_BIAS = 0.5f;

static const char* lightTypeNames[] =
{
    "directional",
    "point",
    "spot",
    0
};

ALight::ALight() :
    _lightType(DEFAULT_LIGHTTYPE),
    _color(DEFAULT_COLOR),
    _range(DEFAULT_RANGE),
    _fov(DEFAULT_SPOT_FOV),
    _lightMask(M_MAX_UNSIGNED),
    _shadowMapSize(DEFAULT_SHADOWMAP_SIZE),
    _shadowSplits(DEFAULT_SHADOW_SPLITS),
    _shadowFadeStart(DEFAULT_FADE_START),
    _depthBias(DEFAULT_DEPTH_BIAS),
    _slopeScaledDepthBias(DEFAULT_SLOPE_SCALED_DEPTH_BIAS),
    _shadowMap(nullptr)
{
    SetFlag(NF_LIGHT, true);
}

ALight::~ALight()
{
}

void ALight::OnPrepareRender(unsigned frameNumber, ACamera* camera)
{
    _lastFrameNumber = frameNumber;
    
    switch (_lightType)
    {
    case ELightType::DIRECTIONAL:
        _distance = 0.0f;
        break;

    case ELightType::POINT:
        _distance = GetWorldFrustum().Distance(camera->GetWorldPosition());
        break;

    case ELightType::SPOT:
        _distance = GetWorldSphere().Distance(camera->GetWorldPosition());
        break;
    }
}

void ALight::OnRaycast(TVector<FRaycastResult>& dest, const FRay& ray, float maxDistance)
{
    if (_lightType == ELightType::SPOT)
    {
        float distance = ray.HitDistance(GetWorldFrustum());
        if (distance <= maxDistance)
        {
            FRaycastResult res;
            res._position = ray._origin + distance * ray._direction;
            res._normal = -ray._direction;
            res._distance = distance;
            res._node = this;
            res._subObject = 0;
            dest.Push(res);
        }
    }
    else if (_lightType == ELightType::POINT)
    {
        float distance = ray.HitDistance(GetWorldSphere());
        if (distance <= maxDistance)
        {
            FRaycastResult res;
            res._position = ray._origin + distance * ray._direction;
            res._normal = -ray._direction;
            res._distance = distance;
            res._node = this;
            res._subObject = 0;
            dest.Push(res);
        }
    }
}

void ALight::SetLightType(ELightType::Type type)
{
    if (type != _lightType)
    {
        _lightType = type;
        // Bounding box will change
        AOctreeNode::OnTransformChanged();
    }
}

void ALight::SetColor(const FColor& color)
{
    _color = color;
}

void ALight::SetRange(float range)
{
    range = Max(range, 0.0f);
    if (range != _range)
    {
        _range = range;
        // Bounding box will change
        AOctreeNode::OnTransformChanged();
    }
}

void ALight::SetFov(float fov)
{
    fov = Clamp(fov, 0.0f, 180.0f);
    if (fov != _fov)
    {
        _fov = fov;
        // Bounding box will change
        AOctreeNode::OnTransformChanged();
    }
}

void ALight::SetLightMask(unsigned lightMask)
{
    _lightMask = lightMask;
}

void ALight::SetShadowMapSize(int _size)
{
    if (_size < 1)
        _size = 1;

    _shadowMapSize = NextPowerOfTwo(_size);
}

void ALight::SetShadowSplits(const TVector4F& splits)
{
    _shadowSplits = splits;
}

void ALight::SetShadowFadeStart(float start)
{
    _shadowFadeStart = Clamp(start, 0.0f, 1.0f);
}

void ALight::SetDepthBias(int bias)
{
    _depthBias = Max(bias, 0);
}

void ALight::SetSlopeScaledDepthBias(float bias)
{
    _slopeScaledDepthBias = Max(bias, 0.0f);
}

TVector2I ALight::GetTotalShadowMapSize() const
{
    if (_lightType == ELightType::DIRECTIONAL)
    {
        int splits = GetNumShadowSplits();
        if (splits == 1)
            return TVector2I(_shadowMapSize, _shadowMapSize);
        else if (splits == 2)
            return TVector2I(_shadowMapSize * 2, _shadowMapSize);
        else
            return TVector2I(_shadowMapSize * 2, _shadowMapSize * 2);
    }
    else if (_lightType == ELightType::POINT)
        return TVector2I(_shadowMapSize * 3, _shadowMapSize * 2);
    else
        return TVector2I(_shadowMapSize, _shadowMapSize);
}

int ALight::GetNumShadowSplits() const
{
    if (_shadowSplits._y <= 0.0f)
        return 1;
    else if (_shadowSplits._z <= 0.0f)
        return 2;
    else if (_shadowSplits._w <= 0.0f)
        return 3;
    else
        return 4;
}

float ALight::GetShadowSplit(size_t index) const
{
    if (index == 0)
        return _shadowSplits._x;
    else if (index == 1)
        return _shadowSplits._y;
    else if (index == 2)
        return _shadowSplits._z;
    else
        return _shadowSplits._w;
}

float ALight::GetMaxShadowDistance() const
{
    if (_lightType != ELightType::DIRECTIONAL)
        return _range;
    else
    {
        if (_shadowSplits._y <= 0.0f)
            return _shadowSplits._x;
        else if (_shadowSplits._z <= 0.0f)
            return _shadowSplits._y;
        else if (_shadowSplits._w <= 0.0f)
            return _shadowSplits._z;
        else
            return _shadowSplits._w;
    }
}

size_t ALight::GetNumShadowViews() const
{
    if (!GetCastShadows())
        return 0;
    else if (_lightType == ELightType::DIRECTIONAL)
        return GetNumShadowSplits();
    else if (_lightType == ELightType::POINT)
        return 6;
    else
        return 1;
}

size_t ALight::GetNumShadowCoords() const
{
    if (!GetCastShadows() || _lightType == ELightType::POINT)
        return 0;
    // Directional light always uses up all the light coordinates and can not share the pass with shadowed spot lights
    else if (_lightType == ELightType::DIRECTIONAL)
        return MAX_LIGHTS_PER_PASS;
    else
        return 1;
}

FFrustum ALight::GetWorldFrustum() const
{
    const TMatrix3x4F& transform = GetWorldTransform();
    TMatrix3x4F frustumTransform(transform.Translation(), transform.Rotation(), 1.0f);
    FFrustum ret;
    ret.Define(_fov, 1.0f, 1.0f, 0.0f, _range, frustumTransform);
    return ret;
}

FSphere ALight::GetWorldSphere() const
{
    return FSphere(GetWorldPosition(), _range);
}

void ALight::SetShadowMap(ATexture* shadowMap, const TRectI& shadowRect)
{
    _shadowMap = shadowMap;
    _shadowRect = shadowRect;
}

void ALight::SetupShadowViews(ACamera* mainCamera, TVector<TAutoPtr<FShadowView> >& shadowViews, size_t& useIndex)
{
    size_t numViews = GetNumShadowViews();
    if (!numViews)
        return;

    if (shadowViews.Size() < useIndex + numViews)
        shadowViews.Resize(useIndex + numViews);

    int numVerticalSplits = (_lightType == ELightType::POINT || (_lightType == ELightType::DIRECTIONAL && GetNumShadowSplits() > 2)) ? 2 : 1;
    int actualShadowMapSize = _shadowRect.Height() / numVerticalSplits;

    for (size_t i = 0; i < numViews; ++i)
    {
        if (!shadowViews[useIndex + i])
            shadowViews[useIndex + i] = new FShadowView();

        FShadowView* view = shadowViews[useIndex + i].Get();
        view->Clear();
        view->_light = this;
        ACamera& shadowCamera = view->_shadowCamera;

        switch (_lightType)
        {
        case ELightType::DIRECTIONAL:
            {
                TVector2I topLeft(_shadowRect.Left(), _shadowRect.Top());
                if (i & 1)
                    topLeft._x += actualShadowMapSize;
                if (i & 2)
                    topLeft._y += actualShadowMapSize;
                view->_viewport = TRectI(topLeft._x, topLeft._y, topLeft._x + actualShadowMapSize, topLeft._y + actualShadowMapSize);

                float splitStart = Max(mainCamera->GetNearClip(), (i == 0) ? 0.0f : GetShadowSplit(i - 1));
                float splitEnd = Min(mainCamera->GetFarClip(), GetShadowSplit(i));
                float extrusionDistance = mainCamera->GetFarClip();
                
                // Calculate initial _position & rotation
                shadowCamera.SetTransform(mainCamera->GetWorldPosition() - extrusionDistance * GetWorldDirection(), GetWorldRotation());

                // Calculate main camera shadowed frustum in light's view space
                FFrustum splitFrustum = mainCamera->WorldSplitFrustum(splitStart, splitEnd);
                const TMatrix3x4F& lightView = shadowCamera.GetViewMatrix();
                FFrustum lightViewFrustum = splitFrustum.Transformed(lightView);

                // Fit the frustum inside a bounding box
                TBoundingBoxF shadowBox;
                shadowBox.Define(lightViewFrustum);

                // If shadow camera is far away from the frustum, can bring it closer for better depth precision
                /// \todo The minimum distance is somewhat arbitrary
                float minDistance = mainCamera->GetFarClip() * 0.25f;
                if (shadowBox._min._z > minDistance)
                {
                    float move = shadowBox._min._z - minDistance;
                    shadowCamera.Translate(TVector3F(0.0f, 0.f, move));
                    shadowBox._min._z -= move,
                    shadowBox._max._z -= move;
                }

                shadowCamera.SetOrthographic(true);
                shadowCamera.SetFarClip(shadowBox._max._z);

                TVector3F center = shadowBox.Center();
                TVector3F _size = shadowBox.Size();
                shadowCamera.SetOrthoSize(TVector2F(_size._x, _size._y));
                shadowCamera.SetZoom(1.0f);

                // Center shadow camera to the view space bounding box
                TVector3F pos(shadowCamera.GetWorldPosition());
                FQuaternion rot(shadowCamera.GetWorldRotation());
                TVector3F adjust(center._x, center._y, 0.0f);
                shadowCamera.Translate(rot * adjust, ETransformSpace::WORLD);

                // Snap to whole texels
                {
                    TVector3F viewPos(rot.Inverse() * shadowCamera.GetWorldPosition());
                    float invSize = 1.0f / actualShadowMapSize;
                    TVector2F texelSize(_size._x * invSize, _size._y * invSize);
                    TVector3F snap(-fmodf(viewPos._x, texelSize._x), -fmodf(viewPos._y, texelSize._y), 0.0f);
                    shadowCamera.Translate(rot * snap, ETransformSpace::WORLD);
                }
            }
            break;

        case ELightType::POINT:
            {
                static const FQuaternion pointLightFaceRotations[] = {
                    FQuaternion(0.0f, 90.0f, 0.0f),
                    FQuaternion(0.0f, -90.0f, 0.0f),
                    FQuaternion(-90.0f, 0.0f, 0.0f),
                    FQuaternion(90.0f, 0.0f, 0.0f),
                    FQuaternion(0.0f, 0.0f, 0.0f),
                    FQuaternion(0.0f, 180.0f, 0.0f)
                };

                TVector2I topLeft(_shadowRect.Left(), _shadowRect.Top());
                if (i & 1)
                    topLeft._y += actualShadowMapSize;
                topLeft._x += ((unsigned)i >> 1) * actualShadowMapSize;
                view->_viewport = TRectI(topLeft._x, topLeft._y, topLeft._x + actualShadowMapSize, topLeft._y + actualShadowMapSize);

                shadowCamera.SetTransform(GetWorldPosition(), pointLightFaceRotations[i]);
                shadowCamera.SetFov(90.0f);
                // Adjust zoom to avoid edge sampling artifacts (there is a matching adjustment in the shadow sampling)
                shadowCamera.SetZoom(0.99f);
                shadowCamera.SetFarClip(GetRange());
                shadowCamera.SetNearClip(GetRange() * 0.01f);
                shadowCamera.SetOrthographic(false);
                shadowCamera.SetAspectRatio(1.0f);
            }
            break;

        case ELightType::SPOT:
            view->_viewport = _shadowRect;
            shadowCamera.SetTransform(GetWorldPosition(), GetWorldRotation());
            shadowCamera.SetFov(_fov);
            shadowCamera.SetZoom(1.0f);
            shadowCamera.SetFarClip(GetRange());
            shadowCamera.SetNearClip(GetRange() * 0.01f);
            shadowCamera.SetOrthographic(false);
            shadowCamera.SetAspectRatio(1.0f);
            break;
        }
    }

    // Setup shadow matrices now as camera positions have been finalized
    if (_lightType != ELightType::POINT)
    {
        _shadowMatrices.Resize(numViews);
        
        for (size_t i = 0; i < numViews; ++i)
        {
            FShadowView* view = shadowViews[useIndex + i].Get();

            ACamera& shadowCamera = view->_shadowCamera;
            float width = (float)_shadowMap->GetWidth();
            float height = (float)_shadowMap->GetHeight();
            TVector3F offset((float)view->_viewport.Left() / width, (float)view->_viewport.Top() / height, 0.0f);
            TVector3F scale(0.5f * (float)view->_viewport.Width() / width, 0.5f * (float)view->_viewport.Height() / height, 1.0f);

            offset._x += scale._x;
            offset._y += scale._y;
            scale._y = -scale._y;

            // OpenGL has different depth range
            #ifdef AUTO_OPENGL
            offset._z = 0.5f;
            scale._z = 0.5f;
            #endif
            
            TMatrix4x4F texAdjust(TMatrix4x4F::IDENTITY);
            texAdjust.SetTranslation(offset);
            texAdjust.SetScale(scale);

            _shadowMatrices[i] = texAdjust * shadowCamera.GetProjectionMatrix() * shadowCamera.GetViewMatrix();
        }
    }
    else
    {
        // Point lights use an extra constant instead
        _shadowMatrices.Clear();

        TVector2F textureSize((float)_shadowMap->GetWidth(), (float)_shadowMap->GetHeight());
        _pointShadowParameters = TVector4F(actualShadowMapSize / textureSize._x, actualShadowMapSize / textureSize._y,
            (float)_shadowRect.Left() / textureSize._x, (float)_shadowRect.Top() / textureSize._y);
    }

    // Calculate shadow mapping constants
    ACamera& shadowCamera = shadowViews[useIndex]->_shadowCamera;
    float nearClip = shadowCamera.GetNearClip();
    float farClip = shadowCamera.GetFarClip();
    float q = farClip / (farClip - nearClip);
    float r = -q * nearClip;
    _shadowParameters = TVector4F(0.5f / (float)_shadowMap->GetWidth(), 0.5f / (float)_shadowMap->GetHeight(), q, r);
    
    useIndex += numViews;
}

void ALight::OnWorldBoundingBoxUpdate() const
{
    switch (_lightType)
    {
    case ELightType::DIRECTIONAL:
        // Directional light always sets humongous bounding box not affected by transform
        _worldBoundingBox.Define(-M_MAX_FLOAT, M_MAX_FLOAT);
        break;
        
    case ELightType::POINT:
        {
            const TVector3F& center = GetWorldPosition();
            TVector3F edge(_range, _range, _range);
            _worldBoundingBox.Define(center - edge, center + edge);
        }
        break;
        
    case ELightType::SPOT:
        _worldBoundingBox.Define(GetWorldFrustum());
        break;
    }

    SetFlag(NF_BOUNDING_BOX_DIRTY, false);
}

void ALight::SetLightTypeAttr(int type)
{
    if (type <= ELightType::SPOT)
        SetLightType((ELightType::Type)type);
}

int ALight::GetLightTypeAttr() const
{
    return (int)_lightType;
}

}
