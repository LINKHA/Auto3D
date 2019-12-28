#pragma once

#include "Math/Color.h"
#include "Math/Frustum.h"
#include "Math/Plane.h"
#include "Math/Ray.h"
#include "Engine/Components/Component.h"


namespace Auto3D
{

/// Billboard camera facing modes.
namespace EFaceCameraMode
{
	enum Type
	{
		NONE = 0,
		ROTATE_XYZ,
		ROTATE_Y,
		LOOKAT_XYZ,
		LOOKAT_Y
	};
};

class ATexture;

/// ACamera scene node.
class AUTO_API ACamera : public AComponent
{
	DECLARE_CLASS(ACamera, AComponent)
public:
    /// Construct.
    ACamera();
	/// Destructor
	~ACamera() = default;

    /// Set near clip distance.
    void SetNearClip(float nearClip);
    /// Set far clip distance.
    void SetFarClip(float farClip);
    /// Set vertical field of view in degrees.
    void SetFov(float fov);
    /// Set orthographic mode view uniform _size.
    void SetOrthoSize(float orthoSize);
    /// Set orthographic mode view non-uniform _size.
    void SetOrthoSize(const TVector2F& orthoSize);
    /// Set aspect ratio.
    void SetAspectRatio(float aspectRatio);
    /// Set zoom.
    void SetZoom(float zoom);
    /// Set LOD bias. Values higher than 1 uses higher quality LOD (acts if distance is smaller.)
    void SetLodBias(float bias);
	/// Set view layer mask. Will be checked against scene objects' layers to see what to render.
	void SetLayoutMask(unsigned mask);
	/// Set view layer mask. Will be checked against scene objects' layers to see what to render.
    void SetLayoutMaskIndex(unsigned maskIndex);
	/// Set view layer mask from name. Will be checked against scene objects' layers to see what to render.
	void SetLayoutMaskName(const FString& name);
	/// Set view layer out with mask.
	void SetLayoutMaskOutIndex(unsigned maskIndex);
	/// Set view layer out with mask.
	void SetLayoutMaskOutName(const FString& name);
	/// Set view layer mask all layout.
	void SetLayoutMaskAll();
    /// Set orthographic projection mode.
    void SetOrthographic(bool enable);
    /// Set ambient light color to use when rendering with this camera.
    void SetAmbientColor(const FColor& color);
    /// Set projection offset. It needs to be calculated as (offset in pixels) / (viewport dimensions.)
    void SetProjectionOffset(const TVector2F& offset);
    /// Set reflection mode.
    void SetUseReflection(bool enable);
    /// Set reflection plane in world space for reflection mode.
    void SetReflectionPlane(const FPlane& plane);
    /// Set whether to use a custom clip plane.
    void SetUseClipping(bool enable);
    /// Set custom clipping plane in world space.
    void SetClipPlane(const FPlane& plane);
    /// Set vertical flipping mode.
    void SetFlipVertical(bool enable);
    /// Return far clip distance.
    float GetFarClip() const { return _farClip; }
    /// Return near clip distance.
    float GetNearClip() const;
    /// Return vertical field of view in degrees.
    float GetFov() const { return _fov; }
    /// Return orthographic mode _size.
    float GetOrthoSize() const { return _orthoSize; }
    /// Return aspect ratio.
    float GetAspectRatio() const { return _aspectRatio; }
    /// Return zoom.
    float GetZoom() const { return _zoom; }
    /// Return LOD bias.
    float GetLodBias() const { return _lodBias; }
    /// Return view layer mask.
    unsigned GetViewMask() const { return _viewLayoutMask; }
    /// Return whether is orthographic.
    bool IsOrthographic() const { return _orthographic; }
    /// Return ambient light color.
    const FColor& GetAmbientColor() const { return _ambientColor; }
    /// Return projection offset.
    const TVector2F& GetProjectionOffset() const { return _projectionOffset; }
    /// Return whether is using reflection.
    bool GetUseReflection() const { return _useReflection; }
    /// Return the reflection plane.
    const FPlane& GetReflectionPlane() const { return _reflectionPlane; }
    /// Return whether is using a custom clipping plane.
    bool GetUseClipping() const { return _useClipping; }
    /// Return the custom clipping plane.
    const FPlane& GetClipPlane() const { return _clipPlane; }
    /// Return vertical flipping mode.
    bool GetFlipVertical() const { return _flipVertical; }
    /// Return whether to reverse culling; affected by vertical flipping and reflection.
    bool GetUseReverseCulling() const { return _flipVertical ^ _useReflection; }
    /// Return frustum in world space.
    FFrustum GetWorldFrustum() const;
    /// Return world space frustum split by custom near and far clip distances.
    FFrustum WorldSplitFrustum(float nearClip, float farClip) const;
    /// Return frustum in view space.
    FFrustum GetViewSpaceFrustum() const;
    /// Return split frustum in view space.
    FFrustum ViewSpaceSplitFrustum(float nearClip, float farClip) const;
    /// Return view matrix.
    const TMatrix3x4F& GetViewMatrix() const;
    /// Return either API-specific or API-independent (D3D convention) projection matrix.
    TMatrix4x4F GetProjectionMatrix(bool apiSpecific = true) const;
    /// Return frustum near and far sizes.
    void FrustumSize(TVector3F& near, TVector3F& far) const;
    /// Return half view _size.
    float GetHalfViewSize() const;
    /// Return ray corresponding to normalized screen coordinates (0.0 - 1.0).
    FRay ScreenRay(float x, float y) const;
    // Convert a world space point to normalized screen coordinates (0.0 - 1.0).
    TVector2F WorldToScreenPoint(const TVector3F& worldPos) const;
    // Convert normalized screen coordinates (0.0 - 1.0) and depth to a world space point.
    TVector3F ScreenToWorldPoint(const TVector3F& screenPos) const;
    /// Return distance to _position. In orthographic mode uses only Z coordinate.
    float Distance(const TVector3F& worldPos) const;
    /// Return a scene node's LOD scaled distance.
    float LodDistance(float distance, float scale, float bias) const;
    /// Return a world rotation for facing a camera on certain axes based on the existing world rotation.
    FQuaternion FaceCameraRotation(const TVector3F& position, const FQuaternion& rotation, EFaceCameraMode::Type mode);
    /// Get effective world transform for matrix and frustum calculations including reflection but excluding node scaling.
    TMatrix3x4F EffectiveWorldTransform() const;
    /// Return if projection parameters are _valid for rendering and raycasting.
    bool IsProjectionValid() const;
public:
	/// Set reflection plane as vector. Used in serialization.
	void SetReflectionPlaneAttr(TVector4F value);
	/// Return reflection plane as vector. Used in serialization.
	TVector4F ReflectionPlaneAttr() const;
	/// Set clipping plane attribute as vector. Used in serialization.
	void SetClipPlaneAttr(TVector4F value);
	/// Return clipping plane attribute as vector. Used in serialization.
	TVector4F ClipPlaneAttr() const;

protected:
    /// Handle the transform matrix changing.
    void OnTransformChanged() override;

private:
   
    /// Cached view matrix.
    mutable TMatrix3x4F _viewMatrix;
    /// View matrix dirty flag.
    mutable bool _viewMatrixDirty;
    /// Orthographic mode flag.
    bool _orthographic;
    /// Flip vertical flag.
    bool _flipVertical;
    /// Near clip distance.
    float _nearClip;
    /// Far clip distance.
    float _farClip;
    /// Field of view.
    float _fov;
    /// Orthographic view size.
    float _orthoSize;
    /// Aspect ratio.
    float _aspectRatio;
    /// Zoom.
    float _zoom;
    /// LOD bias.
    float _lodBias;
    /// View layer mask.
    unsigned _viewLayoutMask;
    /// Ambient light color.
    FColor _ambientColor;
    /// Projection offset.
    TVector2F _projectionOffset;
    /// Reflection plane.
    FPlane _reflectionPlane;
    /// Clipping plane.
    FPlane _clipPlane;
    /// Reflection matrix calculated from the plane.
    TMatrix3x4F _reflectionMatrix;
    /// Reflection mode enabled flag.
    bool _useReflection;
    /// Use custom clip plane flag.
    bool _useClipping;
};

}
