#include "../Debug/Log.h"
#include "../Math/Ray.h"
#include "Light.h"
#include "Octree.h"
#include "Renderer.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

static const LightType DEFAULT_LIGHTTYPE = LIGHT_POINT;
static const Color DEFAULT_COLOR = Color(1.0f, 1.0f, 1.0f, 0.5f);
static const float DEFAULT_RANGE = 10.0f;
static const float DEFAULT_SPOT_FOV = 30.0f;
static const int DEFAULT_SHADOWMAP_SIZE = 512;
static const Vector4 DEFAULT_SHADOW_SPLITS = Vector4(10.0f, 50.0f, 150.0f, 0.0f);
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

Light::Light() :
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

Light::~Light()
{
}

void Light::RegisterObject()
{
    RegisterFactory<Light>();

    CopyBaseAttributes<Light, OctreeNode>();
    RegisterAttribute("lightType", &Light::LightTypeAttr, &Light::SetLightTypeAttr, (int)DEFAULT_LIGHTTYPE, lightTypeNames);
    RegisterRefAttribute("color", &Light::GetColor, &Light::SetColor, DEFAULT_COLOR);
    RegisterAttribute("range", &Light::Range, &Light::SetRange, DEFAULT_RANGE);
    RegisterAttribute("fov", &Light::Fov, &Light::SetFov, DEFAULT_SPOT_FOV);
    RegisterAttribute("lightMask", &Light::LightMask, &Light::SetLightMask, M_MAX_UNSIGNED);
    RegisterAttribute("shadowMapSize", &Light::ShadowMapSize, &Light::SetShadowMapSize, DEFAULT_SHADOWMAP_SIZE);
    RegisterRefAttribute("shadowSplits", &Light::ShadowSplits, &Light::SetShadowSplits, DEFAULT_SHADOW_SPLITS);
    RegisterAttribute("shadowFadeStart", &Light::ShadowFadeStart, &Light::SetShadowFadeStart, DEFAULT_FADE_START);
    RegisterAttribute("depthBias", &Light::DepthBias, &Light::SetDepthBias, DEFAULT_DEPTH_BIAS);
    RegisterAttribute("slopeScaledDepthBias", &Light::SlopeScaledDepthBias, &Light::SetSlopeScaledDepthBias, DEFAULT_SLOPE_SCALED_DEPTH_BIAS);
}


void Light::OnPrepareRender(unsigned frameNumber, Camera* camera)
{
    _lastFrameNumber = frameNumber;
    
    switch (_lightType)
    {
    case LIGHT_DIRECTIONAL:
        _distance = 0.0f;
        break;

    case LIGHT_POINT:
        _distance = WorldFrustum().Distance(camera->WorldPosition());
        break;

    case LIGHT_SPOT:
        _distance = WorldSphere().Distance(camera->WorldPosition());
        break;
    }
}

void Light::OnRaycast(Vector<RaycastResult>& dest, const Ray& ray, float maxDistance)
{
    if (_lightType == LIGHT_SPOT)
    {
        float distance = ray.HitDistance(WorldFrustum());
        if (distance <= maxDistance)
        {
            RaycastResult res;
            res._position = ray._origin + distance * ray._direction;
            res._normal = -ray._direction;
            res._distance = distance;
            res._node = this;
            res._subObject = 0;
            dest.Push(res);
        }
    }
    else if (_lightType == LIGHT_POINT)
    {
        float distance = ray.HitDistance(WorldSphere());
        if (distance <= maxDistance)
        {
            RaycastResult res;
            res._position = ray._origin + distance * ray._direction;
            res._normal = -ray._direction;
            res._distance = distance;
            res._node = this;
            res._subObject = 0;
            dest.Push(res);
        }
    }
}

void Light::SetLightType(LightType type)
{
    if (type != _lightType)
    {
        _lightType = type;
        // Bounding box will change
        OctreeNode::OnTransformChanged();
    }
}

void Light::SetColor(const Color& color)
{
    _color = color;
}

void Light::SetRange(float range)
{
    range = Max(range, 0.0f);
    if (range != _range)
    {
        _range = range;
        // Bounding box will change
        OctreeNode::OnTransformChanged();
    }
}

void Light::SetFov(float fov)
{
    fov = Clamp(fov, 0.0f, 180.0f);
    if (fov != _fov)
    {
        _fov = fov;
        // Bounding box will change
        OctreeNode::OnTransformChanged();
    }
}

void Light::SetLightMask(unsigned lightMask)
{
    _lightMask = lightMask;
}

void Light::SetShadowMapSize(int size)
{
    if (size < 1)
        size = 1;

    _shadowMapSize = NextPowerOfTwo(size);
}

void Light::SetShadowSplits(const Vector4& splits)
{
    _shadowSplits = splits;
}

void Light::SetShadowFadeStart(float start)
{
    _shadowFadeStart = Clamp(start, 0.0f, 1.0f);
}

void Light::SetDepthBias(int bias)
{
    _depthBias = Max(bias, 0);
}

void Light::SetSlopeScaledDepthBias(float bias)
{
    _slopeScaledDepthBias = Max(bias, 0.0f);
}

IntVector2 Light::TotalShadowMapSize() const
{
    if (_lightType == LIGHT_DIRECTIONAL)
    {
        int splits = NumShadowSplits();
        if (splits == 1)
            return IntVector2(_shadowMapSize, _shadowMapSize);
        else if (splits == 2)
            return IntVector2(_shadowMapSize * 2, _shadowMapSize);
        else
            return IntVector2(_shadowMapSize * 2, _shadowMapSize * 2);
    }
    else if (_lightType == LIGHT_POINT)
        return IntVector2(_shadowMapSize * 3, _shadowMapSize * 2);
    else
        return IntVector2(_shadowMapSize, _shadowMapSize);
}

int Light::NumShadowSplits() const
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

float Light::ShadowSplit(size_t index) const
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

float Light::MaxShadowDistance() const
{
    if (_lightType != LIGHT_DIRECTIONAL)
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

size_t Light::NumShadowViews() const
{
    if (!CastShadows())
        return 0;
    else if (_lightType == LIGHT_DIRECTIONAL)
        return NumShadowSplits();
    else if (_lightType == LIGHT_POINT)
        return 6;
    else
        return 1;
}

size_t Light::NumShadowCoords() const
{
    if (!CastShadows() || _lightType == LIGHT_POINT)
        return 0;
    // Directional light always uses up all the light coordinates and can not share the pass with shadowed spot lights
    else if (_lightType == LIGHT_DIRECTIONAL)
        return MAX_LIGHTS_PER_PASS;
    else
        return 1;
}

Frustum Light::WorldFrustum() const
{
    const Matrix3x4& transform = WorldTransform();
    Matrix3x4 frustumTransform(transform.Translation(), transform.Rotation(), 1.0f);
    Frustum ret;
    ret.Define(_fov, 1.0f, 1.0f, 0.0f, _range, frustumTransform);
    return ret;
}

Sphere Light::WorldSphere() const
{
    return Sphere(WorldPosition(), _range);
}

void Light::SetShadowMap(Texture* shadowMap, const IntRect& shadowRect)
{
    _shadowMap = shadowMap;
    _shadowRect = shadowRect;
}

void Light::SetupShadowViews(Camera* mainCamera, Vector<AutoPtr<ShadowView> >& shadowViews, size_t& useIndex)
{
    size_t numViews = NumShadowViews();
    if (!numViews)
        return;

    if (shadowViews.Size() < useIndex + numViews)
        shadowViews.Resize(useIndex + numViews);

    int numVerticalSplits = (_lightType == LIGHT_POINT || (_lightType == LIGHT_DIRECTIONAL && NumShadowSplits() > 2)) ? 2 : 1;
    int actualShadowMapSize = _shadowRect.Height() / numVerticalSplits;

    for (size_t i = 0; i < numViews; ++i)
    {
        if (!shadowViews[useIndex + i])
            shadowViews[useIndex + i] = new ShadowView();

        ShadowView* view = shadowViews[useIndex + i].Get();
        view->Clear();
        view->_light = this;
        Camera& shadowCamera = view->_shadowCamera;

        switch (_lightType)
        {
        case LIGHT_DIRECTIONAL:
            {
                IntVector2 topLeft(_shadowRect._left, _shadowRect._top);
                if (i & 1)
                    topLeft._x += actualShadowMapSize;
                if (i & 2)
                    topLeft._y += actualShadowMapSize;
                view->_viewport = IntRect(topLeft._x, topLeft._y, topLeft._x + actualShadowMapSize, topLeft._y + actualShadowMapSize);

                float splitStart = Max(mainCamera->NearClip(), (i == 0) ? 0.0f : ShadowSplit(i - 1));
                float splitEnd = Min(mainCamera->FarClip(), ShadowSplit(i));
                float extrusionDistance = mainCamera->FarClip();
                
                // Calculate initial _position & rotation
                shadowCamera.SetTransform(mainCamera->WorldPosition() - extrusionDistance * WorldDirection(), WorldRotation());

                // Calculate main camera shadowed frustum in light's view space
                Frustum splitFrustum = mainCamera->WorldSplitFrustum(splitStart, splitEnd);
                const Matrix3x4& lightView = shadowCamera.ViewMatrix();
                Frustum lightViewFrustum = splitFrustum.Transformed(lightView);

                // Fit the frustum inside a bounding box
                BoundingBox shadowBox;
                shadowBox.Define(lightViewFrustum);

                // If shadow camera is far away from the frustum, can bring it closer for better depth precision
                /// \todo The minimum distance is somewhat arbitrary
                float minDistance = mainCamera->FarClip() * 0.25f;
                if (shadowBox._min._z > minDistance)
                {
                    float move = shadowBox._min._z - minDistance;
                    shadowCamera.Translate(Vector3(0.0f, 0.f, move));
                    shadowBox._min._z -= move,
                    shadowBox._max._z -= move;
                }

                shadowCamera.SetOrthographic(true);
                shadowCamera.SetFarClip(shadowBox._max._z);

                Vector3 center = shadowBox.Center();
                Vector3 size = shadowBox.Size();
                shadowCamera.SetOrthoSize(Vector2(size._x, size._y));
                shadowCamera.SetZoom(1.0f);

                // Center shadow camera to the view space bounding box
                Vector3 pos(shadowCamera.WorldPosition());
                Quaternion rot(shadowCamera.WorldRotation());
                Vector3 adjust(center._x, center._y, 0.0f);
                shadowCamera.Translate(rot * adjust, TS_WORLD);

                // Snap to whole texels
                {
                    Vector3 viewPos(rot.Inverse() * shadowCamera.WorldPosition());
                    float invSize = 1.0f / actualShadowMapSize;
                    Vector2 texelSize(size._x * invSize, size._y * invSize);
                    Vector3 snap(-fmodf(viewPos._x, texelSize._x), -fmodf(viewPos._y, texelSize._y), 0.0f);
                    shadowCamera.Translate(rot * snap, TS_WORLD);
                }
            }
            break;

        case LIGHT_POINT:
            {
                static const Quaternion pointLightFaceRotations[] = {
                    Quaternion(0.0f, 90.0f, 0.0f),
                    Quaternion(0.0f, -90.0f, 0.0f),
                    Quaternion(-90.0f, 0.0f, 0.0f),
                    Quaternion(90.0f, 0.0f, 0.0f),
                    Quaternion(0.0f, 0.0f, 0.0f),
                    Quaternion(0.0f, 180.0f, 0.0f)
                };

                IntVector2 topLeft(_shadowRect._left, _shadowRect._top);
                if (i & 1)
                    topLeft._y += actualShadowMapSize;
                topLeft._x += ((unsigned)i >> 1) * actualShadowMapSize;
                view->_viewport = IntRect(topLeft._x, topLeft._y, topLeft._x + actualShadowMapSize, topLeft._y + actualShadowMapSize);

                shadowCamera.SetTransform(WorldPosition(), pointLightFaceRotations[i]);
                shadowCamera.SetFov(90.0f);
                // Adjust zoom to avoid edge sampling artifacts (there is a matching adjustment in the shadow sampling)
                shadowCamera.SetZoom(0.99f);
                shadowCamera.SetFarClip(Range());
                shadowCamera.SetNearClip(Range() * 0.01f);
                shadowCamera.SetOrthographic(false);
                shadowCamera.SetAspectRatio(1.0f);
            }
            break;

        case LIGHT_SPOT:
            view->_viewport = _shadowRect;
            shadowCamera.SetTransform(WorldPosition(), WorldRotation());
            shadowCamera.SetFov(_fov);
            shadowCamera.SetZoom(1.0f);
            shadowCamera.SetFarClip(Range());
            shadowCamera.SetNearClip(Range() * 0.01f);
            shadowCamera.SetOrthographic(false);
            shadowCamera.SetAspectRatio(1.0f);
            break;
        }
    }

    // Setup shadow matrices now as camera positions have been finalized
    if (_lightType != LIGHT_POINT)
    {
        _shadowMatrices.Resize(numViews);
        
        for (size_t i = 0; i < numViews; ++i)
        {
            ShadowView* view = shadowViews[useIndex + i].Get();

            Camera& shadowCamera = view->_shadowCamera;
            float width = (float)_shadowMap->Width();
            float height = (float)_shadowMap->Height();
            Vector3 offset((float)view->_viewport._left / width, (float)view->_viewport._top / height, 0.0f);
            Vector3 scale(0.5f * (float)view->_viewport.Width() / width, 0.5f * (float)view->_viewport.Height() / height, 1.0f);

            offset._x += scale._x;
            offset._y += scale._y;
            scale._y = -scale._y;

            // OpenGL has different depth range
            #ifdef AUTO_OPENGL
            offset._z = 0.5f;
            scale._z = 0.5f;
            #endif
            
            Matrix4 texAdjust(Matrix4::IDENTITY);
            texAdjust.SetTranslation(offset);
            texAdjust.SetScale(scale);

            _shadowMatrices[i] = texAdjust * shadowCamera.ProjectionMatrix() * shadowCamera.ViewMatrix();
        }
    }
    else
    {
        // Point lights use an extra constant instead
        _shadowMatrices.Clear();

        Vector2 textureSize((float)_shadowMap->Width(), (float)_shadowMap->Height());
        _pointShadowParameters = Vector4(actualShadowMapSize / textureSize._x, actualShadowMapSize / textureSize._y,
            (float)_shadowRect._left / textureSize._x, (float)_shadowRect._top / textureSize._y);
    }

    // Calculate shadow mapping constants
    Camera& shadowCamera = shadowViews[useIndex]->_shadowCamera;
    float nearClip = shadowCamera.NearClip();
    float farClip = shadowCamera.FarClip();
    float q = farClip / (farClip - nearClip);
    float r = -q * nearClip;
    _shadowParameters = Vector4(0.5f / (float)_shadowMap->Width(), 0.5f / (float)_shadowMap->Height(), q, r);
    
    useIndex += numViews;
}

void Light::OnWorldBoundingBoxUpdate() const
{
    switch (_lightType)
    {
    case LIGHT_DIRECTIONAL:
        // Directional light always sets humongous bounding box not affected by transform
        _worldBoundingBox.Define(-M_MAX_FLOAT, M_MAX_FLOAT);
        break;
        
    case LIGHT_POINT:
        {
            const Vector3& center = WorldPosition();
            Vector3 edge(_range, _range, _range);
            _worldBoundingBox.Define(center - edge, center + edge);
        }
        break;
        
    case LIGHT_SPOT:
        _worldBoundingBox.Define(WorldFrustum());
        break;
    }

    SetFlag(NF_BOUNDING_BOX_DIRTY, false);
}

void Light::SetLightTypeAttr(int type)
{
    if (type <= LIGHT_SPOT)
        SetLightType((LightType)type);
}

int Light::LightTypeAttr() const
{
    return (int)_lightType;
}

}
