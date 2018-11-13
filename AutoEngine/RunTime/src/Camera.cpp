#include "Camera.h"
#include "Graphics.h"
#include "BaseSpace.h"
#include "Renderer.h"

namespace Auto3D {

Camera::Camera(Ambient* ambient)
	:Super(ambient)
	, _front(glm::vec3(0.0f, 0.0f, -1.0f))
	, _movementSpeed(2.5f)
	, _mouseSensitivity(0.1f)
	, _zoom(45.0f)
	, _isFirstMouse(true)
	, _near(0.1f)
	, _far(100.0f)
	, _isEnable(true)
	, _viewRect(Rectf(0.0f, 0.0f, 1.0f, 1.0f))
	, _worldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, _yaw(-90.0f)
	, _pitch(0.0f)
	, _projectionMode(ProjectionMode::kPerspective)
{
	_position = Vector3(0.0f, 0.0f, 0.0f).ToGLM();
	updateCameraVectors();
	GetSubSystem<Renderer>()->AddCamera(this);
	
}

Camera::~Camera()
{
}

//void Camera::RegisterObject(Ambient* ambient)
//{
//	ambient->RegisterFactory<Camera>(SCENE_ATTACH);
//}

void Camera::Reset()
{

}
void Camera::AllowOffScreen(bool enable)
{
	_isAllowOffScreen = enable;
	if (!_isAllowOffScreen || _offScreen)
		return;
	_offScreen = new OffScreen(_ambient);
}
void Camera::AllowMSAA(bool enable, int pointNum)
{
	if (!_offScreen)
	{
		_isAllowOffScreen = true;
		_offScreen = new OffScreen(_ambient);
	}
	_offScreen->AllowMSAA(enable, pointNum);
}

void Camera::AllowLateEffect(bool enable)
{
	if (!_offScreen)
	{
		_isAllowOffScreen = true;
		_offScreen = new OffScreen(_ambient);
	}
		
	_offScreen->AllowLateEffect(enable);
}

void Camera::AllowHDR(bool enable)
{
	if (!_offScreen)
	{
		_isAllowOffScreen = true;
		_offScreen = new OffScreen(_ambient);
	}

	_offScreen->AllowHDR(enable);
}
void Camera::SetLateEffect(PostProcessingMode mode)
{
	if (_offScreen && _isAllowOffScreen)
		_offScreen->SetEffect(mode);
}
void Camera::SetLateEffect(const Shader& shader)
{
	if (_offScreen && _isAllowOffScreen)
		_offScreen->SetEffect(shader);
}

OffScreen * Camera::GetOffScreen()
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

glm::mat4& Camera::GetViewMatrix()
{
	_viewMatrix = glm::lookAt(_position, _position + _front, _up);
	return _viewMatrix;
}

glm::mat4& Camera::GetProjectionMatrix()
{
	RectInt rect = GetSubSystem<Graphics>()->GetWindowRectInt();
	if (_projectionMode == ProjectionMode::kPerspective)
	{
		_projectionMatrix = glm::perspective(_zoom,
			((float)rect.width * (float)_viewRect.width) /
			((float)rect.height * (float)_viewRect.height),
			_near, _far);
		return _projectionMatrix;
	}
	else if (_projectionMode == ProjectionMode::kOrthographic)
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

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = _movementSpeed * deltaTime;
	if (direction == CameraMovement::kForward)
		_position += _front * velocity;
	if (direction == CameraMovement::kBackward)
		_position -= _front * velocity;
	if (direction == CameraMovement::kLeft)
		_position -= _right * velocity;
	if (direction == CameraMovement::kRight)
		_position += _right * velocity;
}


void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= _mouseSensitivity;
	yoffset *= _mouseSensitivity;

	_yaw += xoffset;
	_pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (_pitch > 89.0f)
			_pitch = 89.0f;
		if (_pitch < -89.0f)
			_pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	updateCameraVectors();
}


void Camera::ProcessMouseScroll(float yoffset)
{
	if (_zoom >= 1.0f && _zoom <= 45.0f)
		_zoom -= yoffset;
	if (_zoom <= 1.0f)
		_zoom = 1.0f;
	if (_zoom >= 45.0f)
		_zoom = 45.0f;
}



void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}


}