#pragma once
#include "Auto.h"
#include "Rect.h"
#include "Matrix4x4.h"
#include "Color.h"
#include "HDRSkyBox.h"
#include "OffScreen.h"
#include "Transform.h"
namespace Auto3D {

enum class CameraMovement
{
	Forward,
	Backward,
	Left,
	Right,
	Up,
	Down,
};

enum class ProjectionMode
{
	Default = 0,
	Perspective = 1,
	Orthographic = 2,
};

class Camera : public Component,public EnableSharedFromThis<Camera>
{
	REGISTER_OBJECT_CLASS(Camera, Component)
public:
	/**
	* @brief : Construct
	*/
	explicit Camera(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);
	/**
	* @brief : To register for transorm
	*/
	void Init()override;
	/**
	* @brief : Override Start
	*/
	void Start()override;
	/**
	* @brief : Accumulation pitch and yaw,Expects the offset value in both the x and y direction.
	*/
	void AccPitchYaw(float pitchOffset, float yawOffset, bool constrainPitch = true);
	/**
	* @brief : Set pitch and yaw,Expects the offset value in both the x and y direction.
	*/
	void SetPitchYaw(float yaw, float pitch, bool constrainPitch = true);
	/**
	* @brief : Accumulation pitch ,Expects the offset value in both the x and y direction.
	*/
	void AccYaw(float yawOffset);
	/**
	* @brief : Set yaw ,Expects the offset value in both the x and y direction.
	*/
	void SetYaw(float yaw);
	/**
	* @brief : Accumulation yaw ,Expects the offset value in both the x and y direction.
	*/
	void AccPitch(float pitchOffset, bool constrainPitch = true);
	/**
	* @brief : Set pitch ,Expects the offset value in both the x and y direction.
	*/
	void SetPitch(float pitch, bool constrainPitch = true);
	/**
	* @brief : Accumulation zoom,only requires input on the vertical wheel - axis
	*/
	void AccZoom(float offset);
	/**
	* @brief : Set zoom (0.000001 ~ 180)
	*/
	void SetZoom(float zoom);
	/**
	* @brief : Use off screen in this camera
	*/
	void AllowOffScreen(bool enable);
	/**
	* @brief : Use MSAA in this camera
	* @param : The number of samples is pointNum,clamp(1~8) default 4
	*/
	void AllowMSAA(bool enable,int pointNum = 4);
	/**
	* @brief : Use post processing in this camera
	*/
	void AllowLateEffect(bool enable);
	/**
	* @brief : Use HDR in this camera
	*/
	void AllowHDR(bool enable);
	/**
	* @brief : Default Post Processing effect 
	*/
	void SetLateEffect(PostProcessingMode mode);
	/**
	* @brief : Set custom Post Processing effect 
	*/
	void SetLateEffect(SharedPtr<ShaderVariation> shader);
	/**
	* @brief : If allow Off screen get OffScreen ,else get nullptr
	*/
	SharedPtr<OffScreen> GetOffScreen();
	/**
	* @brief : According to projection Mode ,get view matrix
	*/
	glm::mat4 GetProjectionMatrix();
	/**
	* @brief : Get camera front vector3
	*/
	Vector3 GetFront() { return _front; }
	/**
	* @brief : Get camera up vector3
	*/
	Vector3 GetUp() { return _up; }
	/**
	* @brief : Get camera right vector3
	*/
	Vector3 GetRight(){ return _right; }
	/**
	* @brief : Get view matrix
	*/
	glm::mat4 GetViewMatrix();
	/**
	* @brief : Get depth
	*/
	float GetDepth() const { return _depth; }
	/**
	* @brief : Get color in this camera
	*/
	Color GetBackgroundColor() const { return _backGroundColor; }
	/**
	* @brief : Get projection mode
	*/
	ProjectionMode GetSortMode() const { return _projectionMode; }
	/**
	* @brief : Get view rect
	*/
	Rectf GetViewRect() const { return _viewRect; }
	/**
	* @brief : Get camera near
	*/
	float GetNear() const { return _near; }
	/**
	* @brief : Get camera far
	*/
	float GetFar() const { return _far; }
	/**
	* @brief : Get camera zoom
	*/
	float GetZoom() const { return _zoom; }
	/**
	* @brief : Get camera position
	*/
	const Vector3 GetPosition() const { return _transform->GetPosition(); }
	/**
	* @brief : Set camera position
	*/
	void SetPosition(const Vector3& vec) { _transform->SetPosition(vec); }
	/**
	* @brief : Cumulative camera position
	*/
	void AccPosition(const Vector3& vec) { SetPosition(GetPosition() + vec); }
	/**
	* @brief : Get allow msaa
	*/
	bool GetAllowMSAA();
	/**
	* @brief : Get allow late effect
	*/
	bool GetAllowLateEffect();
	/**
	* @brief : Get allow offscreen
	*/
	bool GetAllowOffScreen() { return _isAllowOffScreen; }
	/**
	* @brief : Set depth
	*/
	void SetDepth(float depth) { _depth = depth; }
	/**
	* @brief : Set backgound color
	*/
	void SetBackgroundColor(const Color& color) { _backGroundColor = color; }
	/**
	* @brief : Set camera projection mode
	*/
	void SetType(ProjectionMode type) { _projectionMode = type; }
	/**
	* @brief : Set camera view rect
	*/
	void SetViewRect(float x, float y, float w, float h) { _viewRect = Rectf(x, y, w, h); }
	/**
	* @brief : Set camera view rect
	*/
	void SetViewRect(const Rectf& rectf) { _viewRect = rectf; }
	/**
	* @brief : Set camera near
	*/
	void SetNear(float snear) { _near = snear; }
	/**
	* @brief : Set camera fat
	*/
	void SetFar(float sfar) { _far = sfar; }
private:
	/**
	* @brief : Calculates the front vector from the Camera's (updated) Eular Angles
	*/
	void updateCameraVectors();
protected:
	/// Camera transform
	SharedPtr<Transform> _transform;
	/// Camera zoom
	float _zoom;
	/// Camera view rect
	Rectf _viewRect;
	/// Camera near
	float _near;
	/// Camera far
	float _far;
	/// Camera fornt Vector3
	Vector3 _front;
	/// Camera up Vector3
	Vector3 _up;
	/// Camera right Vector3
	Vector3 _right;
	/// Camera world up Vector3
	Vector3 _worldUp;
	/// Camera yaw
	float _yaw;
	/// Camera pitch
	float _pitch;
	/// Camera view matrix
	glm::mat4 _viewMatrix;
	/// Camera projection matrix
	glm::mat4 _projectionMatrix;
	/// Camera depth
	float _depth;
	/// Camera back ground color
	Color _backGroundColor;
	/// Projection mode
	ProjectionMode _projectionMode;
	/// Is or not rendering
	bool _isRendering;
	/// Is or not first mouse
	bool _isFirstMouse;
	/// OffScreen
	SharedPtr<OffScreen> _offScreen;
	/// Is allow OffScreen
	bool _isAllowOffScreen{};
};
}
