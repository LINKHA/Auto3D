#include "Camera.h"
#include "RenderLoop.h"
#include "RenderManager.h"
#include "GLWindow.h"
AUTO_BEGIN

Camera::Camera(Vector3 position, glm::vec3 up, float yaw, float pitch)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f))
	, MovementSpeed(2.5f)
	, MouseSensitivity(0.1f)
	, Zoom(45.0f)
	, firstMouse(true)
	, Near(0.1f)
	, Far(100.0f)
	, m_Enable(true)
	, ViewRect(Rectf(0.0f,0.0f,1.0f,1.0f))
	, WorldUp(up)
	, Yaw(yaw)
	, Pitch(pitch)
	, m_SortMode(kSortPerspective)
{
	m_RenderLoop = CreateRenderLoop(*this);
	Position = position.ToGLM();
	updateCameraVectors();
}
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f))
	, MovementSpeed(2.5f)
	, MouseSensitivity(0.1f)
	, Zoom(45.0f)
	, firstMouse(true)
	, Near(0.1f)
	, Far(100.0f)
	, m_Enable(true)
	, ViewRect(Rectf(0.0f, 0.0f, 1.0f, 1.0f))
	, WorldUp(glm::vec3(upX, upY, upZ))
	, Yaw(yaw)
	, Pitch(pitch)
	, m_SortMode(kSortPerspective)
{
	m_RenderLoop = CreateRenderLoop(*this);
	Position = glm::vec3(posX, posY, posZ);
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
	return glm::lookAt(Position, Position + Front, Up);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}
///Private
// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  
	Up = glm::normalize(glm::cross(Right, Front));
}


AUTO_END