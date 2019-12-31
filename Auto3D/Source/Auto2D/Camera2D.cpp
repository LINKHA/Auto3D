#include "Camera2D.h"
#include "Scene2D.h"

#include "Debug/Log.h"

namespace Auto3D
{
static const float DEFAULT_NEARCLIP = 0.1f;
static const float DEFAULT_FARCLIP = 1000.0f;
static const float DEFAULT_FOV = 45.0f;
static const float DEFAULT_ORTHOSIZE = 20.0f;
static const FColor DEFAULT_AMBIENT_COLOR(0.25f, 0.25f, 0.25f, 1.0f);

static const TMatrix4x4F flipMatrix(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, -1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);

ACamera2D::ACamera2D() :
	_viewMatrix(TMatrix3x4F::IDENTITY),
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
	_viewLayoutMask(M_MAX_UNSIGNED),
	_ambientColor(DEFAULT_AMBIENT_COLOR),
	_projectionOffset(TVector2F::ZERO),
	_reflectionPlane(FPlane::UP),
	_clipPlane(FPlane::UP),
	_useReflection(false),
	_useClipping(false)
{
	_reflectionMatrix = _reflectionPlane.ReflectionMatrix();
}

ACamera2D::~ACamera2D()
{

}

void ACamera2D::SetNearClip(float nearClip)
{
	_nearClip = Max(nearClip, M_EPSILON);
}

void ACamera2D::SetFarClip(float farClip)
{
	_farClip = Max(farClip, M_EPSILON);
}

void ACamera2D::SetFov(float fov)
{
	_fov = Clamp(fov, 0.0f, 180.0f);
}

void ACamera2D::SetOrthoSize(float orthoSize)
{
	_orthoSize = orthoSize;
	_aspectRatio = 1.0f;
}

void ACamera2D::SetOrthoSize(const TVector2F& orthoSize)
{
	_orthoSize = orthoSize._y;
	_aspectRatio = orthoSize._x / orthoSize._y;
}

void ACamera2D::SetAspectRatio(float aspectRatio)
{
	_aspectRatio = Max(aspectRatio, M_EPSILON);
}

void ACamera2D::SetZoom(float zoom)
{
	_zoom = Max(zoom, M_EPSILON);
}

void ACamera2D::SetLodBias(float bias)
{
	_lodBias = Max(bias, M_EPSILON);
}

void ACamera2D::SetLayoutMask(unsigned mask)
{
	_viewLayoutMask = mask;
}

void ACamera2D::SetLayoutMaskIndex(unsigned maskIndex)
{
	_viewLayoutMask |= 1 << maskIndex;
}

void ACamera2D::SetLayoutMaskName(const FString& name)
{
	AScene2D* scene = ParentScene2D();
	if (!scene)
		return;

	const THashMap<FString, unsigned char>& layous = scene->Layers();

	auto it = layous.Find(name);
	if (it != layous.End())
		SetLayoutMaskIndex(it->_second);
	else
		ErrorString("Layer" + name + " not defined in the scene");
}

void ACamera2D::SetLayoutMaskOutIndex(unsigned maskIndex)
{
	int tempViewLayoutMask = _viewLayoutMask;

	tempViewLayoutMask &= ~(1 << maskIndex);
	_viewLayoutMask &= tempViewLayoutMask;
}

void ACamera2D::SetLayoutMaskOutName(const FString& name)
{
	AScene2D* scene = ParentScene2D();
	if (!scene)
		return;

	const THashMap<FString, unsigned char>& layous = scene->Layers();

	auto it = layous.Find(name);
	if (it != layous.End())
		SetLayoutMaskOutIndex(it->_second);
	else
		ErrorString("Layer" + name + " not defined in the scene");
}

void ACamera2D::SetLayoutMaskAll()
{
	_viewLayoutMask = M_MAX_UNSIGNED;
}

void ACamera2D::SetOrthographic(bool enable)
{
	_orthographic = enable;
}

void ACamera2D::SetAmbientColor(const FColor& color)
{
	_ambientColor = color;
}

void ACamera2D::SetProjectionOffset(const TVector2F& offset)
{
	_projectionOffset = offset;
}

void ACamera2D::SetUseReflection(bool enable)
{
	_useReflection = enable;
	_viewMatrixDirty = true;
}

void ACamera2D::SetReflectionPlane(const FPlane& plane)
{
	_reflectionPlane = plane;
	_reflectionMatrix = plane.ReflectionMatrix();
	_viewMatrixDirty = true;
}

void ACamera2D::SetUseClipping(bool enable)
{
	_useClipping = enable;
}

void ACamera2D::SetClipPlane(const FPlane& plane)
{
	_clipPlane = plane;
}


void ACamera2D::SetFlipVertical(bool enable)
{
	_flipVertical = enable;
}

float ACamera2D::GetNearClip() const
{
	// Orthographic camera has always near clip at 0 to avoid trouble with shader depth parameters,
	// and unlike in perspective mode there should be no depth buffer precision issue
	return _orthographic ? 0.0f : _nearClip;
}

FFrustum ACamera2D::GetWorldFrustum() const
{
	FFrustum ret;
	TMatrix3x4F worldTransform = EffectiveWorldTransform();

	if (!_orthographic)
		ret.Define(_fov, _aspectRatio, _zoom, GetNearClip(), _farClip, worldTransform);
	else
		ret.DefineOrtho(_orthoSize, _aspectRatio, _zoom, GetNearClip(), _farClip, worldTransform);

	return ret;
}

FFrustum ACamera2D::WorldSplitFrustum(float nearClip, float farClip) const
{
	FFrustum ret;
	TMatrix3x4F worldTransform = EffectiveWorldTransform();

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

FFrustum ACamera2D::GetViewSpaceFrustum() const
{
	FFrustum ret;

	if (!_orthographic)
		ret.Define(_fov, _aspectRatio, _zoom, GetNearClip(), _farClip);
	else
		ret.DefineOrtho(_orthoSize, _aspectRatio, _zoom, GetNearClip(), _farClip);

	return ret;
}

FFrustum ACamera2D::ViewSpaceSplitFrustum(float nearClip, float farClip) const
{
	FFrustum ret;

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

const TMatrix3x4F& ACamera2D::GetViewMatrix() const
{
	if (_viewMatrixDirty)
	{
		_viewMatrix = EffectiveWorldTransform().Inverse();
		_viewMatrixDirty = false;
	}

	return _viewMatrix;
}

TMatrix4x4F ACamera2D::GetProjectionMatrix(bool apiSpecific) const
{
	TMatrix4x4F ret(TMatrix4x4F::ZERO);

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

void ACamera2D::FrustumSize(TVector3F& near, TVector3F& far) const
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

float ACamera2D::GetHalfViewSize() const
{
	if (!_orthographic)
		return tanf(_fov * M_DEGTORAD * 0.5f) / _zoom;
	else
		return _orthoSize * 0.5f / _zoom;
}

FRay ACamera2D::ScreenRay(float x, float y) const
{
	FRay ret;

	// If projection is invalid, just return a ray pointing forward
	if (!IsProjectionValid())
	{
		ret._origin = GetWorldPosition();
		ret._direction = GetWorldDirection();
		return ret;
	}

	TMatrix4x4F viewProjInverse = (GetProjectionMatrix(false) * GetViewMatrix()).Inverse();

	// The parameters range from 0.0 to 1.0. Expand to normalized device coordinates (-1.0 to 1.0) & flip Y axis
	x = 2.0f * x - 1.0f;
	y = 1.0f - 2.0f * y;
	TVector3F near(x, y, 0.0f);
	TVector3F far(x, y, 1.0f);

	ret._origin = viewProjInverse * near;
	ret._direction = ((viewProjInverse * far) - ret._origin).Normalized();
	return ret;
}

TVector2F ACamera2D::WorldToScreenPoint(const TVector3F& worldPos) const
{
	TVector3F eyeSpacePos = GetViewMatrix() * worldPos;
	TVector2F ret;

	if (eyeSpacePos._z > 0.0f)
	{
		TVector3F screenSpacePos = GetProjectionMatrix(false) * eyeSpacePos;
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

TVector3F ACamera2D::ScreenToWorldPoint(const TVector3F& screenPos) const
{
	FRay ray = ScreenRay(screenPos._x, screenPos._y);
	return ray._origin + ray._direction * screenPos._z;
}

float ACamera2D::Distance(const TVector3F& worldPos) const
{
	if (!_orthographic)
		return (worldPos - GetWorldPosition()).Length();
	else
		return Abs((GetViewMatrix() * worldPos)._z);
}

float ACamera2D::LodDistance(float distance, float scale, float bias) const
{
	float d = Max(_lodBias * bias * scale * _zoom, M_EPSILON);
	if (!_orthographic)
		return distance / d;
	else
		return _orthoSize / d;
}

FQuaternion ACamera2D::FaceCameraRotation(const TVector3F& position, const FQuaternion& rotation, EFace2DCameraMode::Type mode)
{
	switch (mode)
	{
	default:
		return rotation;

	case EFace2DCameraMode::ROTATE_XYZ:
		return GetWorldRotation();

	case EFace2DCameraMode::ROTATE_Y:
	{
		TVector3F euler = rotation.EulerAngles();
		euler._y = GetWorldRotation().EulerAngles()._y;
		return FQuaternion(euler._x, euler._y, euler._z);
	}

	case EFace2DCameraMode::LOOKAT_XYZ:
	{
		FQuaternion lookAt;
		lookAt.FromLookRotation(position - GetWorldPosition());
		return lookAt;
	}

	case EFace2DCameraMode::LOOKAT_Y:
	{
		// Make the Y-only lookat happen on an XZ plane to make sure there are no unwanted transitions
		// or singularities
		TVector3F lookAtVec(position - GetWorldPosition());
		lookAtVec._y = 0.0f;

		FQuaternion lookAt;
		lookAt.FromLookRotation(lookAtVec);

		TVector3F euler = rotation.EulerAngles();
		euler._y = lookAt.EulerAngles()._y;
		return FQuaternion(euler._x, euler._y, euler._z);
	}
	}
}

TMatrix3x4F ACamera2D::EffectiveWorldTransform() const
{
	TMatrix3x4F worldTransform(GetWorldPosition(), GetWorldRotation(), 1.0f);
	return _useReflection ? _reflectionMatrix * worldTransform : worldTransform;
}

bool ACamera2D::IsProjectionValid() const
{
	return _farClip > GetNearClip();
}

void ACamera2D::OnTransformChanged()
{
	ATransform2D::OnTransformChanged();

	_viewMatrixDirty = true;
}

void ACamera2D::SetReflectionPlaneAttr(const TVector4F& value)
{
	SetReflectionPlane(FPlane(value));
}

void ACamera2D::SetClipPlaneAttr(const TVector4F& value)
{
	SetClipPlane(FPlane(value));
}

const TVector4F& ACamera2D::ReflectionPlaneAttr() const
{
	return _reflectionPlane.ToVector4();
}

const TVector4F& ACamera2D::ClipPlaneAttr() const
{
	return _clipPlane.ToVector4();
}


}