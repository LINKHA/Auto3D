#include "Camera.h"
#include "RenderLoop.h"
#include "Graphics.h"
#include "MSAA.h"
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
	, _sortMode(kSortPerspective)
	, _msaa(nullptr)
	, _frameBuffersScreen(nullptr)
	, _isAllowMSAA(false)
	, _isAllowPostPrecess(false)
{
	_renderLoop = CreateRenderLoop(ambient,*this);
	_position = Vector3(0.0f, 0.0f, 0.0f).ToGLM();
	updateCameraVectors();
}

Camera::~Camera()
{
	SAFE_DELETE(_msaa);
	SAFE_DELETE(_frameBuffersScreen);
	DeleteRenderLoop(_renderLoop);
}
void Camera::Reset()
{

}

void Camera::Render()
{
	//Use MSAA
	if (_isAllowMSAA)
		_msaa->RenderStart();
	//Use Post precess
	if (_isAllowPostPrecess)
		_frameBuffersScreen->RenderStart();

	_renderLoop->RunLoop();

	if (_isAllowPostPrecess)
		_frameBuffersScreen->RenderEnd();
	if (_isAllowMSAA)
		_msaa->RenderEnd();
}

void Camera::AllowMSAA(bool enable, int pointNum)
{
	_isAllowMSAA = enable;
	if (!_isAllowMSAA || _msaa)
		return;
	_msaa = new MSAA(_ambient, pointNum);

}

void Camera::AllowPostProcess(bool enable)
{
	_isAllowPostPrecess = enable;
	if (!_isAllowPostPrecess || _frameBuffersScreen)
		return;
	_frameBuffersScreen = new FrameBuffersScreen(_ambient);
}

void Camera::SetPostProcess(BuffersMode mode)
{
	if (_frameBuffersScreen && _isAllowPostPrecess)
		_frameBuffersScreen->SetEffect(mode);
}

void Camera::SetPostPrecess(const Shader& shader)
{
	if (_frameBuffersScreen && _isAllowPostPrecess)
		_frameBuffersScreen->SetEffect(shader);
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
	return _projectionMatrix;
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