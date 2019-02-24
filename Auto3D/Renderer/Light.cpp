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
    lightType(DEFAULT_LIGHTTYPE),
    color(DEFAULT_COLOR),
    range(DEFAULT_RANGE),
    fov(DEFAULT_SPOT_FOV),
    lightMask(M_MAX_UNSIGNED),
    shadowMapSize(DEFAULT_SHADOWMAP_SIZE),
    shadowSplits(DEFAULT_SHADOW_SPLITS),
    shadowFadeStart(DEFAULT_FADE_START),
    depthBias(DEFAULT_DEPTH_BIAS),
    slopeScaledDepthBias(DEFAULT_SLOPE_SCALED_DEPTH_BIAS),
    shadowMap(nullptr)
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
    lastFrameNumber = frameNumber;
    
    switch (lightType)
    {
    case LIGHT_DIRECTIONAL:
        distance = 0.0f;
        break;

    case LIGHT_POINT:
        distance = WorldFrustum().Distance(camera->WorldPosition());
        break;

    case LIGHT_SPOT:
        distance = WorldSphere().Distance(camera->WorldPosition());
        break;
    }
}

void Light::OnRaycast(Vector<RaycastResult>& dest, const Ray& ray, float maxDistance)
{
    if (lightType == LIGHT_SPOT)
    {
        float distance = ray.HitDistance(WorldFrustum());
        if (distance <= maxDistance)
        {
            RaycastResult res;
            res.position = ray.origin + distance * ray.direction;
            res.normal = -ray.direction;
            res.distance = distance;
            res.node = this;
            res.subObject = 0;
            dest.Push(res);
        }
    }
    else if (lightType == LIGHT_POINT)
    {
        float distance = ray.HitDistance(WorldSphere());
        if (distance <= maxDistance)
        {
            RaycastResult res;
            res.position = ray.origin + distance * ray.direction;
            res.normal = -ray.direction;
            res.distance = distance;
            res.node = this;
            res.subObject = 0;
            dest.Push(res);
        }
    }
}

void Light::SetLightType(LightType type)
{
    if (type != lightType)
    {
        lightType = type;
        // Bounding box will change
        OctreeNode::OnTransformChanged();
    }
}

void Light::SetColor(const Color& color_)
{
    color = color_;
}

void Light::SetRange(float range_)
{
    range_ = Max(range_, 0.0f);
    if (range_ != range)
    {
        range = range_;
        // Bounding box will change
        OctreeNode::OnTransformChanged();
    }
}

void Light::SetFov(float fov_)
{
    fov_ = Clamp(fov_, 0.0f, 180.0f);
    if (fov_ != fov)
    {
        fov = fov_;
        // Bounding box will change
        OctreeNode::OnTransformChanged();
    }
}

void Light::SetLightMask(unsigned lightMask_)
{
    lightMask = lightMask_;
}

void Light::SetShadowMapSize(int size)
{
    if (size < 1)
        size = 1;

    shadowMapSize = NextPowerOfTwo(size);
}

void Light::SetShadowSplits(const Vector4& splits)
{
    shadowSplits = splits;
}

void Light::SetShadowFadeStart(float start)
{
    shadowFadeStart = Clamp(start, 0.0f, 1.0f);
}

void Light::SetDepthBias(int bias)
{
    depthBias = Max(bias, 0);
}

void Light::SetSlopeScaledDepthBias(float bias)
{
    slopeScaledDepthBias = Max(bias, 0.0f);
}

IntVector2 Light::TotalShadowMapSize() const
{
    if (lightType == LIGHT_DIRECTIONAL)
    {
        int splits = NumShadowSplits();
        if (splits == 1)
            return IntVector2(shadowMapSize, shadowMapSize);
        else if (splits == 2)
            return IntVector2(shadowMapSize * 2, shadowMapSize);
        else
            return IntVector2(shadowMapSize * 2, shadowMapSize * 2);
    }
    else if (lightType == LIGHT_POINT)
        return IntVector2(shadowMapSize * 3, shadowMapSize * 2);
    else
        return IntVector2(shadowMapSize, shadowMapSize);
}

int Light::NumShadowSplits() const
{
    if (shadowSplits.y <= 0.0f)
        return 1;
    else if (shadowSplits.z <= 0.0f)
        return 2;
    else if (shadowSplits.w <= 0.0f)
        return 3;
    else
        return 4;
}

float Light::ShadowSplit(size_t index) const
{
    if (index == 0)
        return shadowSplits.x;
    else if (index == 1)
        return shadowSplits.y;
    else if (index == 2)
        return shadowSplits.z;
    else
        return shadowSplits.w;
}

float Light::MaxShadowDistance() const
{
    if (lightType != LIGHT_DIRECTIONAL)
        return range;
    else
    {
        if (shadowSplits.y <= 0.0f)
            return shadowSplits.x;
        else if (shadowSplits.z <= 0.0f)
            return shadowSplits.y;
        else if (shadowSplits.w <= 0.0f)
            return shadowSplits.z;
        else
            return shadowSplits.w;
    }
}

size_t Light::NumShadowViews() const
{
    if (!CastShadows())
        return 0;
    else if (lightType == LIGHT_DIRECTIONAL)
        return NumShadowSplits();
    else if (lightType == LIGHT_POINT)
        return 6;
    else
        return 1;
}

size_t Light::NumShadowCoords() const
{
    if (!CastShadows() || lightType == LIGHT_POINT)
        return 0;
    // Directional light always uses up all the light coordinates and can not share the pass with shadowed spot lights
    else if (lightType == LIGHT_DIRECTIONAL)
        return MAX_LIGHTS_PER_PASS;
    else
        return 1;
}

Frustum Light::WorldFrustum() const
{
    const Matrix3x4& transform = WorldTransform();
    Matrix3x4 frustumTransform(transform.Translation(), transform.Rotation(), 1.0f);
    Frustum ret;
    ret.Define(fov, 1.0f, 1.0f, 0.0f, range, frustumTransform);
    return ret;
}

Sphere Light::WorldSphere() const
{
    return Sphere(WorldPosition(), range);
}

void Light::SetShadowMap(Texture* shadowMap_, const IntRect& shadowRect_)
{
    shadowMap = shadowMap_;
    shadowRect = shadowRect_;
}

void Light::SetupShadowViews(Camera* mainCamera, Vector<AutoPtr<ShadowView> >& shadowViews, size_t& useIndex)
{
    size_t numViews = NumShadowViews();
    if (!numViews)
        return;

    if (shadowViews.Size() < useIndex + numViews)
        shadowViews.Resize(useIndex + numViews);

    int numVerticalSplits = (lightType == LIGHT_POINT || (lightType == LIGHT_DIRECTIONAL && NumShadowSplits() > 2)) ? 2 : 1;
    int actualShadowMapSize = shadowRect.Height() / numVerticalSplits;

    for (size_t i = 0; i < numViews; ++i)
    {
        if (!shadowViews[useIndex + i])
            shadowViews[useIndex + i] = new ShadowView();

        ShadowView* view = shadowViews[useIndex + i].Get();
        view->Clear();
        view->light = this;
        Camera& shadowCamera = view->shadowCamera;

        switch (lightType)
        {
        case LIGHT_DIRECTIONAL:
            {
                IntVector2 topLeft(shadowRect.left, shadowRect.top);
                if (i & 1)
                    topLeft.x += actualShadowMapSize;
                if (i & 2)
                    topLeft.y += actualShadowMapSize;
                view->viewport = IntRect(topLeft.x, topLeft.y, topLeft.x + actualShadowMapSize, topLeft.y + actualShadowMapSize);

                float splitStart = Max(mainCamera->NearClip(), (i == 0) ? 0.0f : ShadowSplit(i - 1));
                float splitEnd = Min(mainCamera->FarClip(), ShadowSplit(i));
                float extrusionDistance = mainCamera->FarClip();
                
                // Calculate initial position & rotation
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
                if (shadowBox.min.z > minDistance)
                {
                    float move = shadowBox.min.z - minDistance;
                    shadowCamera.Translate(Vector3(0.0f, 0.f, move));
                    shadowBox.min.z -= move,
                    shadowBox.max.z -= move;
                }

                shadowCamera.SetOrthographic(true);
                shadowCamera.SetFarClip(shadowBox.max.z);

                Vector3 center = shadowBox.Center();
                Vector3 size = shadowBox.Size();
                shadowCamera.SetOrthoSize(Vector2(size.x, size.y));
                shadowCamera.SetZoom(1.0f);

                // Center shadow camera to the view space bounding box
                Vector3 pos(shadowCamera.WorldPosition());
                Quaternion rot(shadowCamera.WorldRotation());
                Vector3 adjust(center.x, center.y, 0.0f);
                shadowCamera.Translate(rot * adjust, TS_WORLD);

                // Snap to whole texels
                {
                    Vector3 viewPos(rot.Inverse() * shadowCamera.WorldPosition());
                    float invSize = 1.0f / actualShadowMapSize;
                    Vector2 texelSize(size.x * invSize, size.y * invSize);
                    Vector3 snap(-fmodf(viewPos.x, texelSize.x), -fmodf(viewPos.y, texelSize.y), 0.0f);
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

                IntVector2 topLeft(shadowRect.left, shadowRect.top);
                if (i & 1)
                    topLeft.y += actualShadowMapSize;
                topLeft.x += ((unsigned)i >> 1) * actualShadowMapSize;
                view->viewport = IntRect(topLeft.x, topLeft.y, topLeft.x + actualShadowMapSize, topLeft.y + actualShadowMapSize);

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
            view->viewport = shadowRect;
            shadowCamera.SetTransform(WorldPosition(), WorldRotation());
            shadowCamera.SetFov(fov);
            shadowCamera.SetZoom(1.0f);
            shadowCamera.SetFarClip(Range());
            shadowCamera.SetNearClip(Range() * 0.01f);
            shadowCamera.SetOrthographic(false);
            shadowCamera.SetAspectRatio(1.0f);
            break;
        }
    }

    // Setup shadow matrices now as camera positions have been finalized
    if (lightType != LIGHT_POINT)
    {
        shadowMatrices.Resize(numViews);
        
        for (size_t i = 0; i < numViews; ++i)
        {
            ShadowView* view = shadowViews[useIndex + i].Get();

            Camera& shadowCamera = view->shadowCamera;
            float width = (float)shadowMap->Width();
            float height = (float)shadowMap->Height();
            Vector3 offset((float)view->viewport.left / width, (float)view->viewport.top / height, 0.0f);
            Vector3 scale(0.5f * (float)view->viewport.Width() / width, 0.5f * (float)view->viewport.Height() / height, 1.0f);

            offset.x += scale.x;
            offset.y += scale.y;
            scale.y = -scale.y;

            // OpenGL has different depth range
            #ifdef AUTO_OPENGL
            offset.z = 0.5f;
            scale.z = 0.5f;
            #endif
            
            Matrix4 texAdjust(Matrix4::IDENTITY);
            texAdjust.SetTranslation(offset);
            texAdjust.SetScale(scale);

            shadowMatrices[i] = texAdjust * shadowCamera.ProjectionMatrix() * shadowCamera.ViewMatrix();
        }
    }
    else
    {
        // Point lights use an extra constant instead
        shadowMatrices.Clear();

        Vector2 textureSize((float)shadowMap->Width(), (float)shadowMap->Height());
        pointShadowParameters = Vector4(actualShadowMapSize / textureSize.x, actualShadowMapSize / textureSize.y,
            (float)shadowRect.left / textureSize.x, (float)shadowRect.top / textureSize.y);
    }

    // Calculate shadow mapping constants
    Camera& shadowCamera = shadowViews[useIndex]->shadowCamera;
    float nearClip = shadowCamera.NearClip();
    float farClip = shadowCamera.FarClip();
    float q = farClip / (farClip - nearClip);
    float r = -q * nearClip;
    shadowParameters = Vector4(0.5f / (float)shadowMap->Width(), 0.5f / (float)shadowMap->Height(), q, r);
    
    useIndex += numViews;
}

void Light::OnWorldBoundingBoxUpdate() const
{
    switch (lightType)
    {
    case LIGHT_DIRECTIONAL:
        // Directional light always sets humongous bounding box not affected by transform
        worldBoundingBox.Define(-M_MAX_FLOAT, M_MAX_FLOAT);
        break;
        
    case LIGHT_POINT:
        {
            const Vector3& center = WorldPosition();
            Vector3 edge(range, range, range);
            worldBoundingBox.Define(center - edge, center + edge);
        }
        break;
        
    case LIGHT_SPOT:
        worldBoundingBox.Define(WorldFrustum());
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
    return (int)lightType;
}

}
