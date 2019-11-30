#pragma once

#include "Math/Color.h"
#include "Math/Frustum.h"
#include "Math/Rect.h"
#include "Math/Vector2.h"
#include "Math/Sphere.h"
#include "Scene/OctreeNode.h"

namespace Auto3D
{

class ATexture;
struct ShadowView;

/// %ALight types.
namespace ELightType
{
	enum Type
	{
		DIRECTIONAL = 0,
		POINT,
		SPOT
	};
};


/// Dynamic light scene node.
class AUTO_API ALight : public AOctreeNode
{
	REGISTER_OBJECT_CLASS(ALight, AOctreeNode)
    
public:
    /// Construct.
    ALight();
    /// Destruct.
    virtual ~ALight();

    /// Register factory and attributes.
    static void RegisterObject();

    /// Prepare object for rendering. Reset framenumber and calculate distance from camera. Called by Renderer.
    void OnPrepareRender(unsigned frameNumber, ACamera* camera) override;
    /// Perform ray test on self and add possible hit to the result vector.
    void OnRaycast(TVector<RaycastResult>& dest, const Ray& ray, float maxDistance) override;

    /// Set light type.
    void SetLightType(ELightType::Type type);
    /// Set color. Alpha component contains specular intensity.
    void SetColor(const Color& color);
    /// Set range.
    void SetRange(float range);
    /// Set spotlight field of view.
    void SetFov(float fov);
    /// Set light layer mask. Will be checked against scene objects' layers to see what objects to illuminate.
    void SetLightMask(unsigned mask);
    /// Set shadow map face resolution in pixels.
    void SetShadowMapSize(int _size);
    /// Set directional light shadow split distances. Fill unused splits with zero.
    void SetShadowSplits(const Vector4F& splits);
    /// Set directional light shadow fade start depth, where 1 represents shadow max distance.
    void SetShadowFadeStart(float start);
    /// Set constant depth bias for shadows.
    void SetDepthBias(int bias);
    /// Set slope-scaled depth bias for shadows.
    void SetSlopeScaledDepthBias(float bias);

    /// Return light type.
    ELightType::Type GetLightType() const { return _lightType; }
    /// Return color.
    const Color& GetColor() const { return _color; }
    /// Return range.
    float GetRange() const { return _range; }
    /// Return spotlight field of view.
    float GetFov() const { return _fov; }
    /// Return light layer mask.
    unsigned GetLightMask() const { return _lightMask; }
    /// Return shadow map face resolution in pixels.
    int GetShadowMapSize() const { return _shadowMapSize; }
    /// Return directional light shadow split distances.
    const Vector4F& GetShadowSplits() const { return _shadowSplits; }
    /// Return directional light shadow fade start depth.
    float GetShadowFadeStart() const { return _shadowFadeStart; }
    /// Return number of directional light shadow splits.
    int GetNumShadowSplits() const;
    /// Return shadow split distance by index.
    float GetShadowSplit(size_t index) const;
    /// Return shadow maximum distance.
    float GetMaxShadowDistance() const;
    /// Return constant depth bias.
    int GetDepthBias() const { return _depthBias; }
    /// Return slope-scaled depth bias.
    float GetSlopeScaledDepthBias() const { return _slopeScaledDepthBias; }
    /// Return total requested shadow map _size, accounting for multiple faces / splits for directional and point lights.
    Vector2I GetTotalShadowMapSize() const;
    /// Return number of required shadow views / cameras.
    size_t GetNumShadowViews() const;
    /// Return number of required shadow coordinates in the vertex shader.
    size_t GetNumShadowCoords() const;
    /// Return spotlight world space frustum.
    Frustum GetWorldFrustum() const;
    /// Return point light world space sphere.
    Sphere GetWorldSphere() const;

    /// Set shadow map and viewport within it. Called by Renderer.
    void SetShadowMap(ATexture* shadowMap, const RectI& shadowRect = RectI::ZERO);
    /// Setup shadow cameras and viewports. Called by Renderer.
    void SetupShadowViews(ACamera* mainCamera, TVector<TAutoPtr<ShadowView> >& shadowViews, size_t& useIndex);
    /// Return shadow map.
    ATexture* GetShadowMap() const { return _shadowMap; }
    /// Return actual shadow map rectangle. May be smaller than the requested total shadow map _size.
    const RectI& GetShadowRect() const { return _shadowRect; }
    /// Return shadow mapping matrices.
    const TVector<Matrix4x4F>& GetShadowMatrices() const { return _shadowMatrices; }
    /// Return shadow map offset and depth parameters.
    const Vector4F& GetShadowParameters() const { return _shadowParameters; }
    /// Return point light shadow extra parameters.
    const Vector4F& GetPointShadowParameters() const { return _pointShadowParameters; }

protected:
    /// Recalculate the world space bounding box.
    virtual void OnWorldBoundingBoxUpdate() const override;

private:
    /// Set light type as int. Used in serialization.
    void SetLightTypeAttr(int lightType);
    /// Return light type as int. Used in serialization.
    int LightTypeAttr() const;
    
    /// ALight type.
    ELightType::Type _lightType;
    /// ALight color.
    Color _color;
    /// Range.
    float _range;
    /// Spotlight field of view.
    float _fov;
    /// ALight layer mask.
    unsigned _lightMask;
    /// Shadow map resolution in pixels.
    int _shadowMapSize;
    /// Directional shadow splits.
    Vector4F _shadowSplits;
    /// Directional shadow fade start.
    float _shadowFadeStart;
    /// FConstant depth bias.
    int _depthBias;
    /// Slope-scaled depth bias.
    float _slopeScaledDepthBias;
    /// Current shadow map texture.
    ATexture* _shadowMap;
    /// Rectangle within the shadow map.
    RectI _shadowRect;
    /// Shadow mapping matrices.
    TVector<Matrix4x4F> _shadowMatrices;
    /// Shadow mapping parameters.
    Vector4F _shadowParameters;
    /// Shadow mapping extra parameters for point lights.
    Vector4F _pointShadowParameters;
};

}
