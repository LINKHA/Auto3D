#pragma once
#include "Collider.h"

namespace Auto3D {

class ColliderBox : public Collider
{
	REGISTER_OBJECT_CLASS(ColliderBox, Collider)
public:
	/**
	* @brief : Construct
	*/
	explicit ColliderBox(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);
	/**
	* @brief : Override Start
	*/
	void Start()override;
	/**
	* @brief : Override Update
	*/
	void Update()override;
	/** 
	* @brief : Set size form Vector3
	*/
	void SetSize(const Vector3& vec);
	/**
	* @brief : Set size form float x y z
	*/
	void SetSize(float x, float y, float z);
	/**
	* @brief : Set size form float
	*/
	void SetSize(float scale);
private:
	/**
	* @brief : Resize form Vector3
	*/
	void resize(const Vector3& vec);
private:
	/// Box shape size
	Vector3 _size;
};

}