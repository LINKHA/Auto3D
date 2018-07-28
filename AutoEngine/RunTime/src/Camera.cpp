#include "Camera.h"
#include "RenderLoop.h"
#include "Graphics.h"
AUTO_BEGIN

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
	, _sortMode(kSortPerspective)
{
	_renderLoop = CreateRenderLoop(ambient,*this);
	_position = Vector3(0.0f, 0.0f, 0.0f).ToGLM();
	updateCameraVectors();
}

Camera::~Camera()
{
	DeleteRenderLoop(_renderLoop);
}
void Camera::Reset()
{

}

void Camera::Render()
{
	_renderLoop->RunLoop();
}

glm::mat4& Camera::GetViewMatrix()
{
	_viewMatrix = glm::lookAt(_position, _position + _front, _up);
	return _viewMatrix;
}
glm::mat4& Camera::GetProjectionMatrix()
{
	RectInt rect = GetSubSystem<Graphics>()->GetWindowRectInt();
	if (_sortMode == kSortPerspective)
	{
		_projectionMatrix = glm::perspective(_zoom,
			((float)rect.width * (float)_viewRect.width) /
			((float)rect.height * (float)_viewRect.height),
			_near, _far);
		return _projectionMatrix;
	}
	else if (_sortMode == kSortOrthographic)
	{
		float t = ((float)rect.width * (float)_viewRect.width) /
			((float)rect.height * (float)_viewRect.height);
		_projectionMatrix = glm::ortho(-t, t, -1.0f, 1.0f, _near, _far);
		return _projectionMatrix;
	}
	else
		ErrorString("Fail to set projection matrix");
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = _movementSpeed * deltaTime;
	if (direction == FORWARD) 
		_position += _front * velocity;
	if (direction == BACKWARD)
		_position -= _front * velocity;
	if (direction == LEFT)
		_position -= _right * velocity;
	if (direction == RIGHT)
		_position += _right * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
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

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
	if (_zoom >= 1.0f && _zoom <= 45.0f)
		_zoom -= yoffset;
	if (_zoom <= 1.0f)
		_zoom = 1.0f;
	if (_zoom >= 45.0f)
		_zoom = 45.0f;
}
///Private
// Calculates the front vector from the Camera's (updated) Eular Angles
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


AUTO_END