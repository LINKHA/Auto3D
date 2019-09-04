#include "ColliderBox.h"
#include "PhysicsUtils.h"
#include "PhysicsWorld.h"


namespace Auto3D 
{


ColliderBox::ColliderBox(): 
	_size(Vector3F::ONE)
{
	_shapeType = ShapeType::BOX;
	_shape = new btBoxShape(ToBtVector3(_size));
}

ColliderBox::~ColliderBox()
{
	SafeDelete(_shape);
}

void ColliderBox::RegisterObject()
{
	RegisterFactory<ColliderBox>();
}

void ColliderBox::SetSize(const Vector3F& vec)
{
	_size = vec;
	Resize(_size);
}

void ColliderBox::SetSize(float x, float y, float z)
{
	_size = Vector3F(x, y, z);
	Resize(_size);
}

void ColliderBox::SetSize(float scale)
{
	_size = Vector3F(scale, scale, scale);
	Resize(_size);
}


void ColliderBox::Resize(const Vector3F& vec)
{
	if (!_shape)
		return;
	btBoxShape* shape = dynamic_cast<btBoxShape*>(_shape);

	float margin = _shape->getMargin();

	//Resize implicit shape dimensions
	shape->setImplicitShapeDimensions((ToBtVector3(vec) * shape->getLocalScaling()) - btVector3(margin, margin, margin));
	//Resize the edge
	shape->setSafeMargin(ToBtVector3(vec));

}

}