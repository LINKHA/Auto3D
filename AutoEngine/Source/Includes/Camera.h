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
	explicit Camera(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);
	/**
	* @brief :
	*/
	virtual void Reset();
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
	//void SetPostProcess(BuffersMode mode);
	void SetLateEffect(PostProcessingMode mode);
	/**
	* @brief : Set custom Post Processing effect 
	*/
	void SetLateEffect(ShaderVariation* shader);
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

	///////////////////////////////////////////////////////////////////////////
	// @brief : Get member
	glm::mat4 GetViewMatrix();
	float GetDepth() const { return _depth; }
	Color& GetBackgroundColor() { return _backGroundColor; }
	ProjectionMode GetSortMode() const { return _projectionMode; }
	bool GetEnable()const { return _isEnable; }
	Rectf& GetViewRect() { return _viewRect; }
	float GetNear() { return _near; }
	float GetFar() { return _far; }
	float GetZoom() { return _zoom; }
	const Vector3 GetPosition() const { return _transform->GetPosition(); }

	void SetPosition(const Vector3& vec) { _transform->SetPosition(vec); }

	void AccPosition(const Vector3& vec) { SetPosition(GetPosition() + vec); }

	bool GetAllowMSAA();
	bool GetAllowLateEffect() ;
	bool GetAllowOffScreen() { return _isAllowOffScreen; }
	//////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// @brief : Set menber
	void SetDepth(float depth) { _depth = depth; }
	void SetBackgroundColor(const Color& color) { _backGroundColor = color; }
	void SetType(ProjectionMode type) { _projectionMode = type; }
	void SetEnable(bool e) { _isEnable = e; }
	void SetViewRect(float x, float y, float w, float h) { _viewRect = Rectf(x, y, w, h); }
	void SetViewRect(const Rectf& rectf) { _viewRect = rectf; }
	void SetNear(float snear) { _near = snear; }
	void SetFar(float sfar) { _far = sfar; }
	//////////////////////////////////////////////////////////////////////////
	/**
	* @brief : To register for transorm
	*/
	void Init()override;
	///Temp !!! start not normally used,in script component not use,but int other component normal
	void Start()override;
private:
	/**
	* @brief : Calculates the front vector from the Camera's (updated) Eular Angles
	*/
	void updateCameraVectors();
private:
	SharedPtr<Transform> _transform;

	float _zoom;

	Rectf _viewRect;

	float _near;
	float _far;

	Vector3 _front;
	Vector3 _up;
	Vector3 _right;

	Vector3 _worldUp;

	float _yaw;
	float _pitch;

	glm::mat4 _viewMatrix;
	glm::mat4 _projectionMatrix;
protected:
	float _depth;
	Color _backGroundColor;
	ProjectionMode _projectionMode;
	bool _isEnable;
	bool _isRendering;
	bool _isFirstMouse;
	SharedPtr<OffScreen> _offScreen;
	bool _isAllowOffScreen{};
};
}
