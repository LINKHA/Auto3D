#include "Camera.h"
#include "Graphics.h"
#include "BaseSpace.h"
#include "Renderer.h"
#include "NewDef.h"

namespace Auto3D {

Camera::Camera(SharedPtr<Ambient> ambient)
	:Super(ambient)
	, _front(0.0f, 0.0f, -1.0f)
	, _zoom(45.0f)
	, _isFirstMouse(true)
	, _near(0.1f)
	, _far(100.0f)
	, _isEnable(true)
	, _viewRect(Rectf(0.0f, 0.0f, 1.0f, 1.0f))
	, _worldUp(0.0f, 1.0f, 0.0f)
	, _yaw(0)
	, _pitch(0.0f)
	, _projectionMode(ProjectionMode::Perspective)
{
	updateCameraVectors();

}

Camera::~Camera()
{
}

void Camera::RegisterObject(SharedPtr<Ambient> ambient)
{
	ambient->RegisterFactory<Camera>(SCENE_ATTACH);
}

void Camera::Reset()
{

}
void Camera::AllowOffScreen(bool enable)
{
	_isAllowOffScreen = enable;
	if (!_isAllowOffScreen || _offScreen)
		return;
	_offScreen = MakeShared<OffScreen>(_ambient);
}
void Camera::AllowMSAA(bool enable, int pointNum)
{
	if (!_offScreen)
	{
		_isAllowOffScreen = true;
		_offScreen = MakeShared<OffScreen>(_ambient);
	}
	_offScreen->AllowMSAA(enable, pointNum);
}

void Camera::AllowLateEffect(bool enable)
{
	if (!_offScreen)
	{
		_isAllowOffScreen = true;
		_offScreen = MakeShared<OffScreen>(_ambient);
	}
		
	_offScreen->AllowLateEffect(enable);
}

void Camera::AllowHDR(bool enable)
{
	if (!_offScreen)
	{
		_isAllowOffScreen = true;
		_offScreen = MakeShared<OffScreen>(_ambient);
	}

	_offScreen->AllowHDR(enable);
}
void Camera::SetLateEffect(PostProcessingMode mode)
{
	if (_offScreen && _isAllowOffScreen)
		_offScreen->SetEffect(mode);
}
void Camera::SetLateEffect(ShaderVariation* shader)
{
	if (_offScreen && _isAllowOffScreen)
		_offScreen->SetEffect(shader);
}

SharedPtr<OffScreen> Camera::GetOffScreen()
{
	if (_isAllowOffScreen && _offScreen)
		return _offScreen;
	else
	{
		ErrorString("Fail to get camera off screen");
		return nullptr;
	}
}

void Camera::Start()
{
	if (_isAllowOffScreen)
		_offScreen->RenderReady();
}

void Camera::Init()
{
	_transform = GetNode()->GetComponent<Transform>();
	GetSubSystem<Renderer>()->AddCamera(SharedFromThis());
}

glm::mat4 Camera::GetViewMatrix()
{
	//_viewMatrix = glm::lookAt(_position, _position + _front, _up);
	_viewMatrix = glm::lookAt(_transform->GetPosition().ToGLM(), _transform->GetPosition().ToGLM() + _front.ToGLM(), _up.ToGLM());
	return _viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	RectInt rect = GetSubSystem<Graphics>()->GetWindowRectInt();
	if (_projectionMode == ProjectionMode::Perspective)
	{
		_projectionMatrix = glm::perspective(_zoom,
			((float)rect.width * (float)_viewRect.width) /
			((float)rect.height * (float)_viewRect.height),
			_near, _far);
		return _projectionMatrix;
	}
	else if (_projectionMode == ProjectionMode::Orthographic)
	{
		float t = ((float)rect.width * (float)_viewRect.width) /
			((float)rect.height * (float)_viewRect.height);
		_projectionMatrix = glm::ortho(-t, t, -1.0f, 1.0f, _near, _far);
		return _projectionMatrix;
	}
	else
		ErrorString("Fail to set projection matrix");
	return _projectionMatrix;
}

bool Camera::GetAllowMSAA()
{
	if (_offScreen)
		return _offScreen->GetAllowMSAA();
	else
		return false;
}

bool Camera::GetAllowLateEffect()
{
	if (_offScreen)
		return _offScreen->GetAllowLateEffect();
	else
		return false;
}

void Camera::AccPitchYaw(float yawOffset, float pitchOffset, bool constrainPitch)
{
	SetPitchYaw(_yaw + yawOffset, _pitch + pitchOffset, constrainPitch);
}

void Camera::SetPitchYaw(float yaw, float pitch, bool constrainPitch)
{
	_yaw = yaw;
	_pitch = pitch;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (_pitch > 89.0f)
			_pitch = 89.0f;
		if (_pitch < -89.0f)
			_pitch = -89.0f;
	}
	updateCameraVectors();
}

void Camera::AccYaw(float yawOffset)
{
	SetYaw(_yaw + yawOffset);
}

void Camera::SetYaw(float yaw)
{
	_yaw = yaw;
	updateCameraVectors();
}
void Camera::AccPitch(float pitchOffset, bool constrainPitch)
{
	SetPitch(_pitch + pitchOffset, constrainPitch);
}

void Camera::SetPitch(float pitch, bool constrainPitch)
{
	_pitch = pitch;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (_pitch > 89.0f)
			_pitch = 89.0f;
		if (_pitch < -89.0f)
			_pitch = -89.0f;
	}
	updateCameraVectors();
}

void Camera::AccZoom(float offset)
{
	SetZoom(_zoom - offset);
}

void Camera::SetZoom(float zoom)
{
	if (_zoom >= MATH_EPSILON && _zoom <= 180.0f)
		_zoom = zoom;
	if (_zoom <= MATH_EPSILON)
		_zoom = MATH_EPSILON;
	if (_zoom >= 180.0f)
		_zoom = 180.0f;
}


void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	Vector3 front;
	//_yaw - 90 Modified coordinate system
	front.x = cos(radians(_yaw - 90)) * cos(radians(_pitch));
	front.y = sin(radians(_pitch));
	front.z = sin(radians(_yaw - 90)) * cos(radians(_pitch));
	_front = Normalize(front);
	// Also re-calculate the Right and Up vector
	_right = Normalize(Cross(_front, _worldUp));
	_up = Normalize(Cross(_right, _front));
}


}