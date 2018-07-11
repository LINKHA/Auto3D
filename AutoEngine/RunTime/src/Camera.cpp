#include "Camera.h"
#include "RenderLoop.h"
#include "RenderManager.h"
#include "GLWindow.h"
AUTO_BEGIN

Camera::Camera(Vector3 position, glm::vec3 up, float yaw, float pitch)
	: m_Front(glm::vec3(0.0f, 0.0f, -1.0f))
	, m_MovementSpeed(2.5f)
	, m_MouseSensitivity(0.1f)
	, m_Zoom(45.0f)
	, firstMouse(true)
	, m_Near(0.1f)
	, m_Far(100.0f)
	, m_Enable(true)
	, m_ViewRect(Rectf(0.0f,0.0f,1.0f,1.0f))
	, m_WorldUp(up)
	, m_Yaw(yaw)
	, m_Pitch(pitch)
	, m_SortMode(kSortPerspective)
{
	m_RenderLoop = CreateRenderLoop(*this);
	m_Position = position.ToGLM();
	updateCameraVectors();
}
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: m_Front(glm::vec3(0.0f, 0.0f, -1.0f))
	, m_MovementSpeed(2.5f)
	, m_MouseSensitivity(0.1f)
	, m_Zoom(45.0f)
	, firstMouse(true)
	, m_Near(0.1f)
	, m_Far(100.0f)
	, m_Enable(true)
	, m_ViewRect(Rectf(0.0f, 0.0f, 1.0f, 1.0f))
	, m_WorldUp(glm::vec3(upX, upY, upZ))
	, m_Yaw(yaw)
	, m_Pitch(pitch)
	, m_SortMode(kSortPerspective)
{
	m_RenderLoop = CreateRenderLoop(*this);
	m_Position = glm::vec3(posX, posY, posZ);
	updateCameraVectors();
}
Camera::~Camera()
{
	DeleteRenderLoop(m_RenderLoop);
}
void Camera::Reset()
{

}

void Camera::Render()
{
	m_RenderLoop->RunLoop();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}
glm::mat4 Camera::GetProjectionMatrix()
{
	RectInt rect = INSTANCE(GLWindow).GetWindowRectInt();
	if (m_SortMode == kSortPerspective)
	{
		glm::mat4 projectionMat = glm::perspective(m_Zoom,
			((float)rect.width * (float)m_ViewRect.width) /
			((float)rect.height * (float)m_ViewRect.height),
			m_Near, m_Far);
		return projectionMat;
	}
	else if (m_SortMode == kSortOrthographic)
	{
		float t = ((float)rect.width * (float)m_ViewRect.width) /
			((float)rect.height * (float)m_ViewRect.height);
		glm::mat4 projectionMat = glm::ortho(-t, t, -1.0f, 1.0f, m_Near, m_Far);
		return projectionMat;
	}
	else
		ErrorString("Fail to set projection matrix");
}
// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = m_MovementSpeed * deltaTime;
	if (direction == FORWARD)
		m_Position += m_Front * velocity;
	if (direction == BACKWARD)
		m_Position -= m_Front * velocity;
	if (direction == LEFT)
		m_Position -= m_Right * velocity;
	if (direction == RIGHT)
		m_Position += m_Right * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= m_MouseSensitivity;
	yoffset *= m_MouseSensitivity;

	m_Yaw += xoffset;
	m_Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
	if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
		m_Zoom -= yoffset;
	if (m_Zoom <= 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom >= 45.0f)
		m_Zoom = 45.0f;
}
///Private
// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}


AUTO_END