#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Auto.h"
#include "Motion.h"
#include "Math/RectT.h"
#include "Math/Matrix4x4.h"

USING_MATH
AUTO_BEGIN
class RenderLoop;
// Default camera values
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.1f;
const float YAW = -90.0f;
const float ZOOM = 45.0f;
enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

enum SortMode
{
	kSortDefault = 0,
	kSortPerspective = 1,
	kSortOrthographic = 2,
};
class Camera : public Motion
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Camera, Motion);
	DECLARE_OBJECT_SERIALIZE(Camera);
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
	Camera(Vector3 position = Vector3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	virtual void Update() {}
	virtual void Start() {}
	virtual void Reset();

	float GetDepth() const { return m_Depth; }
	void SetDepth(float depth) { m_Depth = depth; }
	Color GetBackgroundColor() const { return m_BackGroundColor; }
	void SetBackgroundColor(const Color& color) { m_BackGroundColor = color; }
	SortMode GetSortMode() const { return m_SortMode; }
	void SetSortMode(SortMode m) { m_SortMode = m; }
	bool GetEnable() { return m_Enable; }
	void SetEnable(bool e) { m_Enable = e; }

	void Render();
	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void ProcessMouseScroll(float yoffset);
private:
	void updateCameraVectors();
protected:
	RenderLoop*			m_RenderLoop;
	float				m_Depth;
	Color				m_BackGroundColor;
	Rectf				m_Rect;
	SortMode			m_SortMode;
	bool				m_Enable;
	bool				m_IsRendering;
};
AUTO_END
#endif //!CAMERA_H_