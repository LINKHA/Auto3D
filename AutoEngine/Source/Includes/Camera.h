#pragma once
#include "Auto.h"
#include "Component.h"

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

class Camera : public Component
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
	
};
}
