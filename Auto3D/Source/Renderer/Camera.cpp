#include "../Math/Matrix3x4.h"
#include "Camera.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

static const float DEFAULT_NEARCLIP = 0.1f;
static const float DEFAULT_FARCLIP = 1000.0f;
static const float DEFAULT_FOV = 45.0f;
static const float DEFAULT_ORTHOSIZE = 20.0f;
static const Color DEFAULT_AMBIENT_COLOR(0.25f, 0.25f, 0.25f, 1.0f);

static const Matrix4x4F flipMatrix(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
    );

Camera::Camera() :
    _viewMatrix(Matrix3x4F::IDENTITY),
    _viewMatrixDirty(false),
    _orthographic(false),
    _flipVertical(false),
    _nearClip(DEFAULT_NEARCLIP),
    _farClip(DEFAULT_FARCLIP),
    _fov(DEFAULT_FOV),
    _orthoSize(DEFAULT_ORTHOSIZE),
    _aspectRatio(1.0f),
    _zoom(1.0f),
    _lodBias(1.0f),
    _viewMask(M_MAX_UNSIGNED),
    _ambientColor(DEFAULT_AMBIENT_COLOR),
    _projectionOffset(Vector2F::ZERO),
    _reflectionPlane(Plane::UP),
    _clipPlane(Plane::UP),
    _useReflection(false),
    _useClipping(false)
{
    _reflectionMatrix = _reflectionPlane.ReflectionMatrix();
}

void Camera::RegisterObject()
{
    RegisterFactory<Camera>();
    CopyBaseAttributes<Camera, SpatialNode>();

    RegisterAttribute("nearClip", &Camera::GetNearClip, &Camera::SetNearClip, DEFAULT_NEARCLIP);
    RegisterAttribute("farClip", &Camera::GetFarClip, &Camera::SetFarClip, DEFAULT_FARCLIP);
    RegisterAttribute("fov", &Camera::GetFov, &Camera::SetFov, DEFAULT_FOV);
    RegisterAttribute("aspectRatio", &Camera::GetAspectRatio, &Camera::SetAspectRatio, 1.0f);
    RegisterAttribute("orthographic", &Camera::IsOrthographic, &Camera::SetOrthographic, false);
    RegisterAttribute("orthoSize", &Camera::GetOrthoSize, &Camera::SetOrthoSize, DEFAULT_ORTHOSIZE);
    RegisterAttribute("zoom", &Camera::GetZoom, &Camera::SetZoom, 1.0f);
    RegisterAttribute("lodBias", &Camera::GetLodBias, &Camera::SetLodBias, 1.0f);
    RegisterAttribute("viewMask", &Camera::GetViewMask, &Camera::SetViewMask, M_MAX_UNSIGNED);
    RegisterRefAttribute("ambientColor", &Camera::GetAmbientColor, &Camera::SetAmbientColor, DEFAULT_AMBIENT_COLOR);
    RegisterRefAttribute("projectionOffset", &Camera::GetProjectionOffset, &Camera::SetProjectionOffset, Vector2F::ZERO);
    RegisterMixedRefAttribute("reflectionPlane", &Camera::ReflectionPlaneAttr, &Camera::SetReflectionPlaneAttr, Vector4F(0.0f, 1.0f, 0.0f, 0.0f));
    RegisterMixedRefAttribute("clipPlane", &Camera::ClipPlaneAttr, &Camera::SetClipPlaneAttr, Vector4F(0.0f, 1.0f, 0.0f, 0.0f));
    RegisterAttribute("useReflection", &Camera::GetUseReflection, &Camera::SetUseReflection, false);
    RegisterAttribute("useClipping", &Camera::GetUseClipping, &Camera::SetUseClipping, false);
}

void Camera::SetNearClip(float nearClip)
{
    _nearClip = Max(nearClip, M_EPSILON);
}

void Camera::SetFarClip(float farClip)
{
    _farClip = Max(farClip, M_EPSILON);
}

void Camera::SetFov(float fov)
{
    _fov = Clamp(fov, 0.0f, 180.0f);
}

void Camera::SetOrthoSize(float orthoSize)
{
    _orthoSize = orthoSize;
    _aspectRatio = 1.0f;
}

void Camera::SetOrthoSize(const Vector2F& orthoSize)
{
    _orthoSize = orthoSize._y;
    _aspectRatio = orthoSize._x / orthoSize._y;
}

void Camera::SetAspectRatio(float aspectRatio)
{
    _aspectRatio = Max(aspectRatio, M_EPSILON);
}

void Camera::SetZoom(float zoom)
{
    _zoom = Max(zoom, M_EPSILON);
}

void Camera::SetLodBias(float bias)
{
    _lodBias = Max(bias, M_EPSILON);
}

void Camera::SetViewMask(unsigned mask)
{
    _viewMask = mask;
}

void Camera::SetOrthographic(bool enable)
{
    _orthographic = enable;
}

void Camera::SetAmbientColor(const Color& color)
{
    _ambientColor = color;
}

void Camera::SetProjectionOffset(const Vector2F& offset)
{
    _projectionOffset = offset;
}

void Camera::SetUseReflection(bool enable)
{
    _useReflection = enable;
    _viewMatrixDirty = true;
}

void Camera::SetReflectionPlane(const Plane& plane)
{
    _reflectionPlane = plane;
    _reflectionMatrix = plane.ReflectionMatrix();
    _viewMatrixDirty = true;
}

void Camera::SetUseClipping(bool enable)
{
    _useClipping = enable;
}

void Camera::SetClipPlane(const Plane& plane)
{
    _clipPlane = plane;
}


void Camera::SetFlipVertical(bool enable)
{
    _flipVertical = enable;
}

float Camera::GetNearClip() const
{
    // Orthographic camera has always near clip at 0 to avoid trouble with shader depth parameters,
    // and unlike in perspective mode there should be no depth buffer precision issue
    return _orthographic ? 0.0f : _nearClip;
}

Frustum Camera::GetWorldFrustum() const
{
    Frustum ret;
    Matrix3x4F worldTransform = EffectiveWorldTransform();

    if (!_orthographic)
        ret.Define(_fov, _aspectRatio, _zoom, GetNearClip(), _farClip, worldTransform);
    else
        ret.DefineOrtho(_orthoSize, _aspectRatio, _zoom, GetNearClip(), _farClip, worldTransform);

    return ret;
}

Frustum Camera::WorldSplitFrustum(float nearClip, float farClip) const
{
    Frustum ret;
    Matrix3x4F worldTransform = EffectiveWorldTransform();

    nearClip = Max(nearClip, GetNearClip());
    farClip = Min(farClip, _farClip);
    if (farClip < nearClip)
        farClip = nearClip;

    if (!_orthographic)
        ret.Define(_fov, _aspectRatio, _zoom, nearClip, farClip, worldTransform);
    else
        ret.DefineOrtho(_orthoSize, _aspectRatio, _zoom, nearClip, farClip, worldTransform);

    return ret;
}

Frustum Camera::GetViewSpaceFrustum() const
{
    Frustum ret;

    if (!_orthographic)
        ret.Define(_fov, _aspectRatio, _zoom, GetNearClip(), _farClip);
    else
        ret.DefineOrtho(_orthoSize, _aspectRatio, _zoom, GetNearClip(), _farClip);

    return ret;
}

Frustum Camera::ViewSpaceSplitFrustum(float nearClip, float farClip) const
{
    Frustum ret;

    nearClip = Max(nearClip, GetNearClip());
    farClip = Min(farClip, _farClip);
    if (farClip < nearClip)
        farClip = nearClip;

    if (!_orthographic)
        ret.Define(_fov, _aspectRatio, _zoom, nearClip, farClip);
    else
        ret.DefineOrtho(_orthoSize, _aspectRatio, _zoom, nearClip, farClip);

    return ret;
}

const Matrix3x4F& Camera::GetViewMatrix() const
{
    if (_viewMatrixDirty)
    {
        _viewMatrix = EffectiveWorldTransform().Inverse();
        _viewMatrixDirty = false;
    }

    return _viewMatrix;
}

Matrix4x4F Camera::GetProjectionMatrix(bool apiSpecific) const
{
    Matrix4x4F ret(Matrix4x4F::ZERO);

    bool openGLFormat = apiSpecific;

    // Whether to construct matrix using OpenGL or Direct3D clip space convention
    #ifndef AUTO_OPENGL
    openGLFormat = false;
    #endif

    if (!_orthographic)
    {
        float h = (1.0f / tanf(_fov * M_DEGTORAD * 0.5f)) * _zoom;
        float w = h / _aspectRatio;
        float q, r;

        if (openGLFormat)
        {
            q = (_farClip + _nearClip) / (_farClip - _nearClip);
            r = -2.0f * _farClip * _nearClip / (_farClip - _nearClip);
        }
        else
        {
            q = _farClip / (_farClip - _nearClip);
            r = -q * _nearClip;
        }

        ret._m00 = w;
        ret._m02 = _projectionOffset._x * 2.0f;
        ret._m11 = h;
        ret._m12 = _projectionOffset._y * 2.0f;
        ret._m22 = q;
        ret._m23 = r;
        ret._m32 = 1.0f;
    }
    else
    {
        // Disregard near clip, because it does not affect depth precision as with perspective projection
        float h = (1.0f / (_orthoSize * 0.5f)) * _zoom;
        float w = h / _aspectRatio;
        float q, r;

        if (openGLFormat)
        {
            q = 2.0f / _farClip;
            r = -1.0f;
        }
        else
        {
            q = 1.0f / _farClip;
            r = 0.0f;
        }

        ret._m00 = w;
        ret._m03 = _projectionOffset._x * 2.0f;
        ret._m11 = h;
        ret._m13 = _projectionOffset._y * 2.0f;
        ret._m22 = q;
        ret._m23 = r;
        ret._m33 = 1.0f;
    }

    if (_flipVertical)
        ret = flipMatrix * ret;

    return ret;
}

void Camera::FrustumSize(Vector3F& near, Vector3F& far) const
{
    near._z = GetNearClip();
    far._z = _farClip;

    if (!_orthographic)
    {
        float halfViewSize = tanf(_fov * M_DEGTORAD * 0.5f) / _zoom;
        near._y = near._z * halfViewSize;
        near._x = near._y * _aspectRatio;
        far._y = far._z * halfViewSize;
        far._x = far._y * _aspectRatio;
    }
    else
    {
        float halfViewSize = _orthoSize * 0.5f / _zoom;
        near._y = far._y = halfViewSize;
        near._x = far._x = near._y * _aspectRatio;
    }

    if (_flipVertical)
    {
        near._y = -near._y;
        far._y = -far._y;
    }
}

float Camera::GetHalfViewSize() const
{
    if (!_orthographic)
        return tanf(_fov * M_DEGTORAD * 0.5f) / _zoom;
    else
        return _orthoSize * 0.5f / _zoom;
}

Ray Camera::ScreenRay(float x, float y) const
{
    Ray ret;

    // If projection is invalid, just return a ray pointing forward
    if (!IsProjectionValid())
    {
        ret._origin = GetWorldPosition();
        ret._direction = GetWorldDirection();
        return ret;
    }

    Matrix4x4F viewProjInverse = (GetProjectionMatrix(false) * GetViewMatrix()).Inverse();

    // The parameters range from 0.0 to 1.0. Expand to normalized device coordinates (-1.0 to 1.0) & flip Y axis
    x = 2.0f * x - 1.0f;
    y = 1.0f - 2.0f * y;
    Vector3F near(x, y, 0.0f);
    Vector3F far(x, y, 1.0f);

    ret._origin = viewProjInverse * near;
    ret._direction = ((viewProjInverse * far) - ret._origin).Normalized();
    return ret;
}

Vector2F Camera::WorldToScreenPoint(const Vector3F& worldPos) const
{
    Vector3F eyeSpacePos = GetViewMatrix() * worldPos;
    Vector2F ret;

    if (eyeSpacePos._z > 0.0f)
    {
        Vector3F screenSpacePos = GetProjectionMatrix(false) * eyeSpacePos;
        ret._x = screenSpacePos._x;
        ret._y = screenSpacePos._y;
    }
    else
    {
        ret._x = (-eyeSpacePos._x > 0.0f) ? -1.0f : 1.0f;
        ret._y = (-eyeSpacePos._y > 0.0f) ? -1.0f : 1.0f;
    }

    ret._x = (ret._x * 0.5f) + 0.5f;
    ret._y = 1.0f - ((ret._y * 0.5f) + 0.5f);
    return ret;
}

Vector3F Camera::ScreenToWorldPoint(const Vector3F& screenPos) const
{
    Ray ray = ScreenRay(screenPos._x, screenPos._y);
    return ray._origin + ray._direction * screenPos._z;
}

float Camera::Distance(const Vector3F& worldPos) const
{
    if (!_orthographic)
        return (worldPos - GetWorldPosition()).Length();
    else
        return Abs((GetViewMatrix() * worldPos)._z);
}

float Camera::LodDistance(float distance, float scale, float bias) const
{
    float d = Max(_lodBias * bias * scale * _zoom, M_EPSILON);
    if (!_orthographic)
        return distance / d;
    else
        return _orthoSize / d;
}

Quaternion Camera::FaceCameraRotation(const Vector3F& position, const Quaternion& rotation, FaceCameraMode mode)
{
    switch (mode)
    {
    default:
        return rotation;

    case FaceCameraMode::ROTATE_XYZ:
        return GetWorldRotation();

    case FaceCameraMode::ROTATE_Y:
        {
            Vector3F euler = rotation.EulerAngles();
            euler._y = GetWorldRotation().EulerAngles()._y;
            return Quaternion(euler._x, euler._y, euler._z);
        }

    case FaceCameraMode::LOOKAT_XYZ:
        {
            Quaternion lookAt;
            lookAt.FromLookRotation(position - GetWorldPosition());
            return lookAt;
        }

    case FaceCameraMode::LOOKAT_Y:
        {
            // Make the Y-only lookat happen on an XZ plane to make sure there are no unwanted transitions
            // or singularities
            Vector3F lookAtVec(position - GetWorldPosition());
            lookAtVec._y = 0.0f;

            Quaternion lookAt;
            lookAt.FromLookRotation(lookAtVec);

            Vector3F euler = rotation.EulerAngles();
            euler._y = lookAt.EulerAngles()._y;
            return Quaternion(euler._x, euler._y, euler._z);
        }
    }
}

Matrix3x4F Camera::EffectiveWorldTransform() const
{
    Matrix3x4F worldTransform(GetWorldPosition(), GetWorldRotation(), 1.0f);
    return _useReflection ? _reflectionMatrix * worldTransform : worldTransform;
}

bool Camera::IsProjectionValid() const
{
    return _farClip > GetNearClip();
}

void Camera::OnTransformChanged()
{
    SpatialNode::OnTransformChanged();

    _viewMatrixDirty = true;
}

void Camera::SetReflectionPlaneAttr(const Vector4F& value)
{
    SetReflectionPlane(Plane(value));
}

void Camera::SetClipPlaneAttr(const Vector4F& value)
{
    SetClipPlane(Plane(value));
}

Vector4F Camera::ReflectionPlaneAttr() const
{
    return _reflectionPlane.ToVector4();
}

Vector4F Camera::ClipPlaneAttr() const
{
    return _clipPlane.ToVector4();
}

}
