#pragma once
#include "Collider.h"

namespace Auto3D {

class ColliderSphere : public Collider
{
	REGISTER_OBJECT_CLASS(ColliderSphere, Collider)
public:
	/**
	* @brief : Construct
	*/
	explicit ColliderSphere(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);
	/**
	* @brief : Set size
	*/
	void SetSize(float size);
	/**
	* @brief : Override Start
	*/
	void Start()override;
	/**
	* @brief : Override Update
	*/
	void Update()override;
private:
	/** 
	* @brief : Resize 
	*/
	void resize(float size);
private:
	/// Sphere shape size
	float _size;
};

}