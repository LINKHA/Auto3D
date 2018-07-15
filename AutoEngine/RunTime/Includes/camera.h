#pragma once
#include "Auto.h"
#include "GameObject.h"
#include "Math/Rect.h"
#include "Math/Matrix4x4.h"
#include "Math/Color.h"

USING_MATH
AUTO_BEGIN
class RenderLoop;

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

class Camera : public Component
{
	REGISTER_DERIVED_CLASS(Camera, Component);
	DECLARE_OBJECT_SERIALIZE(Camera);
public:
	// Camera Attributes
	
private:
	bool firstMouse;
public:
	Camera(Vector3 position = Vector3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw = -90.0f, float pitch = 0.0f);
	virtual void Reset();
	void Render();
	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void ProcessMouseScroll(float yoffset);
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	float GetDepth() const { return m_Depth; }
	void SetDepth(float depth) { m_Depth = depth; }

	Color GetBackgroundColor() const{ return m_BackGroundColor; }
	void SetBackgroundColor(const Color& color) { m_BackGroundColor = color; }

	SortMode GetSortMode() const { return m_SortMode; }
	void SetSortMode(SortMode m) { m_SortMode = m; }

	bool GetEnable()const { return m_Enable; }
	void SetEnable(bool e) { m_Enable = e; }

	void SetViewRect(float x, float y, float w, float h) { m_ViewRect = Rectf(x, y, w, h); }
	void SetViewRect(const Rectf& rectf) { m_ViewRect = rectf; }
	Rectf& GetViewRect() { return m_ViewRect; }

	void SetNear(float snear) { m_Near = snear; }
	float GetNear() { return m_Near; }

	void SetFar(float sfar) { m_Far = sfar; }
	float GetFar() { return m_Far; }

	void SetZoom(float zoom) { m_Zoom = zoom; }
	float GetZoom() { return m_Zoom; }

	glm::vec3 GetPosition() { return m_Position; }

	void SetSpeed(float speed) { m_MovementSpeed = speed; }
	
	void SetSensitivity(float sen) { m_MouseSensitivity = sen; }
private:
	void updateCameraVectors();
private:
	glm::vec3 m_Position;
	float m_Zoom;
	Rectf m_ViewRect;
	float m_Near;
	float m_Far;
	float m_MovementSpeed;
	float m_MouseSensitivity;

	//Variables used in internal calculations
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;
	float m_Yaw;
	float m_Pitch;
	//////////////////////////////////////////////////////////////////////////
protected:
	RenderLoop*			m_RenderLoop;
	float				m_Depth;
	Color				m_BackGroundColor;
	SortMode			m_SortMode;
	bool				m_Enable;
	bool				m_IsRendering;
};
AUTO_END
