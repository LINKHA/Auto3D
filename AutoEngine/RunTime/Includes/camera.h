#pragma once
#include "Auto.h"
#include "GameObject.h"
#include "Math/Rect.h"
#include "Math/Matrix4x4.h"
#include "Math/Color.h"
#include "HDRSkyBox.h"
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
	Camera(Vector3 position = Vector3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw = -90.0f, float pitch = 0.0f);
	virtual void Reset();
	void Render();
	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void ProcessMouseScroll(float yoffset);
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	float GetDepth() const { return _depth; }
	void SetDepth(float depth) { _depth = depth; }

	Color GetBackgroundColor() const{ return _backGroundColor; }
	void SetBackgroundColor(const Color& color) { _backGroundColor = color; }

	SortMode GetSortMode() const { return _sortMode; }
	void SetSortMode(SortMode m) { _sortMode = m; }

	bool GetEnable()const { return _isEnable; }
	void SetEnable(bool e) { _isEnable = e; }

	void SetViewRect(float x, float y, float w, float h) { _viewRect = Rectf(x, y, w, h); }
	void SetViewRect(const Rectf& rectf) { _viewRect = rectf; }
	Rectf& GetViewRect() { return _viewRect; }

	void SetNear(float snear) { _near = snear; }
	float GetNear() { return _near; }

	void SetFar(float sfar) { _far = sfar; }
	float GetFar() { return _far; }

	void SetZoom(float zoom) { _zoom = zoom; }
	float GetZoom() { return _zoom; }

	glm::vec3 GetPosition() { return _position; }

	void SetSpeed(float speed) { _movementSpeed = speed; }
	
	void SetSensitivity(float sen) { _mouseSensitivity = sen; }
private:
	void updateCameraVectors();
private:
	glm::vec3 _position;
	float _zoom;
	Rectf _viewRect;
	float _near;
	float _far;
	float _movementSpeed;
	float _mouseSensitivity;

	//Variables used in internal calculations
	glm::vec3 _front;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _worldUp;
	float _yaw;
	float _pitch;
	
	//////////////////////////////////////////////////////////////////////////
protected:
	RenderLoop*			_renderLoop;
	float				_depth;
	Color				_backGroundColor;
	SortMode			_sortMode;
	bool				_isEnable;
	bool				_isRendering;
	bool				_isFirstMouse;
};
AUTO_END
