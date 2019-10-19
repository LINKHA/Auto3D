#include "Collider2D.h"

namespace Auto3D {

Collider2D::Collider2D()
{

}

Collider2D::~Collider2D()
{

}

void Collider2D::RegisterObject()
{
	RegisterFactory<Collider2D>();
}

}