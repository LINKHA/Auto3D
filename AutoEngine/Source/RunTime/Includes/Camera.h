#pragma once
#include "Auto.h"
#include "Math/Rect.h"
#include "Math/Matrix4x4.h"
#include "Math/Color.h"
#include "HDRSkyBox.h"
#include "OffScreen.h"
namespace Auto3D {

enum class CameraMovement
{
	kForward,
	kBackward,
	kLeft,
	kRight
};

enum class ProjectionMode
{
	kDefault = 0,
	kPerspective = 1,
	kOrthographic = 2,
};

class Camera : public Component
{
	REGISTER_DERIVED_CLASS(Camera, Component);
	DECLARE_OBJECT_SERIALIZE(Camera);
public:
	explicit Camera(Ambient* ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(Ambient* ambient);
	/**
	* @brief :
	*/
	virtual void Reset();
	/**
	* @brief : Processes input received from a key board
	*			Expect to move in space
	*/
	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	/**
	* @brief : Processes input received from a mouse input system. 
	*			Expects the offset value in both the x and y direction.
	*/
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	/**
	* @brief : Processes input received from a mouse scroll - wheel event.
	*			Only requires input on the vertical wheel - axis
	*/
	void ProcessMouseScroll(float yoffset);
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
	void SetLateEffect(const Shader& shader);
	/**
	* @brief : If allow Off screen get OffScreen ,else get nullptr
	*/
	OffScreen* GetOffScreen();
	/**
	* @brief : According to projection Mode ,get view matrix
	*/
	glm::mat4& GetProjectionMatrix();
	
	///////////////////////////////////////////////////////////////////////////
	// @brief : Get member
	glm::mat4& GetViewMatrix();
	float GetDepth() const { return _depth; }
	Color& GetBackgroundColor() { return _backGroundColor; }
	ProjectionMode GetSortMode() const { return _projectionMode; }
	bool GetEnable()const { return _isEnable; }
	Rectf& GetViewRect() { return _viewRect; }
	float GetNear() { return _near; }
	float GetFar() { return _far; }
	float GetZoom() { return _zoom; }
	glm::vec3& GetPosition() { return _position; }
	bool GetAllowMSAA();
	bool GetAllowLateEffect() ;
	bool GetAllowOffScreen() { return _isAllowOffScreen; }
	//////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// @brief : Set menber
	void SetDepth(float depth) { _depth = depth; }
	void SetBackgroundColor(const Color& color) { _backGroundColor = color; }
	void SetSortMode(ProjectionMode m) { _projectionMode = m; }
	void SetEnable(bool e) { _isEnable = e; }
	void SetViewRect(float x, float y, float w, float h) { _viewRect = Rectf(x, y, w, h); }
	void SetViewRect(const Rectf& rectf) { _viewRect = rectf; }
	void SetNear(float snear) { _near = snear; }
	void SetFar(float sfar) { _far = sfar; }
	void SetZoom(float zoom) { _zoom = zoom; }
	void SetSpeed(float speed) { _movementSpeed = speed; }
	void SetSensitivity(float sen) { _mouseSensitivity = sen; }
	//////////////////////////////////////////////////////////////////////////

	///Temp !!! start not normally used,in script component not use,but int other component normal
	void Start()override;
private:
	/**
	* @brief : Calculates the front vector from the Camera's (updated) Eular Angles
	*/
	void updateCameraVectors();
private:
	glm::vec3 _position;
	float _zoom;
	Rectf _viewRect;
	float _near;
	float _far;
	float _movementSpeed;
	float _mouseSensitivity;
	glm::vec3 _front;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _worldUp;
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
	sharedPtr<OffScreen> _offScreen;
	bool _isAllowOffScreen;
};
}
