#ifndef FPS_CAMERA_H
#define FPS_CAMERA_H

#include "Auto.h"
#include "Camera.h"
AUTO_BEGIN

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class FPSCamera : public Camera
{
	REGISTER_DERIVED_ABSTRACT_CLASS(FPSCamera, Camera);
	DECLARE_OBJECT_SERIALIZE(FPSCamera);
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Eular Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

private:
	bool firstMouse;
public:
	// Constructor with vectors
	FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),float yaw = YAW, float pitch = PITCH);

	// Constructor with scalar values
	FPSCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset);

	virtual void Update() {}
	virtual void Start() {}
private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors();
};

AUTO_END
#endif //!FPS_CAMERA_H