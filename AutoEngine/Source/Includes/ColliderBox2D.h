#pragma once
#include "Collider2D.h"


namespace Auto3D {

class ColliderBox2D : public Collider2D
{
	REGISTER_OBJECT_CLASS(ColliderBox2D, Collider2D)
public:
	/**
	* @brief : Construct
	*/
	explicit ColliderBox2D(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	void Start()override;
	/**
	* @brief : Set size
	*/
	void SetSize(const Vector2& size);
	/**
	* @brief : Set size
	*/
	void SetSize(float width, float height);
	/**
	* @brief : Set center
	*/
	void SetCenter(const Vector2& center);
	/**
	* @brief : Set center
	*/
	void SetCenter(float x, float y);
	/**
	* @brief : Set angle
	*/
	void SetAngle(float angle);
	/**
	* @brief : Return size
	*/
	const Vector2& GetSize() const { return _size; }
	/**
	* @brief : Return center
	*/
	const Vector2& GetCenter() const { return _center; }
	/**
	* @brief : Return angle
	*/
	float GetAngle() const { return _angle; }
private:
	/**
	* @brief : Recreate fixture
	*/
	void recreateFixture();
private:
	/// Box shape
	b2PolygonShape _boxShape;
	/// Collider size
	Vector2 _size;
	/// Collider center
	Vector2 _center;
	/// Collider angle
	float _angle;
};

}