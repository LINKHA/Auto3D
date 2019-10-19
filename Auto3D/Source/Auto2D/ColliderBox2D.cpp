#include "ColliderBox2D.h"

namespace Auto3D {

ColliderBox2D::ColliderBox2D()
{

}

ColliderBox2D::~ColliderBox2D()
{

}

void ColliderBox2D::RegisterObject()
{
	RegisterFactory<ColliderBox2D>();
}

}