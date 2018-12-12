#pragma once
#include "Collider2D.h"


namespace Auto3D {

class ColliderBox2D : public Collider2D
{
	REGISTER_OBJECT_CLASS(ColliderBox2D, Collider2D)
public:
	explicit ColliderBox2D(Ambient* ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(Ambient* ambient);
	
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
	void recreateFixture();
private:

	b2PolygonShape _boxShape;
	Vector2 _size;
	Vector2 _center;
	float _angle;
};

}