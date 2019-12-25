#include "Sprite2D.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

// Font cpp fail,Initializes the global constructor so apply here
// Apply in GeometryNode2D CPP
//REGISTER_CLASS
//{
//	REGISTER_CALSS_FACTORY_IMP(ASprite2D)
//	.constructor<>()
//	;
//}

ASprite2D::ASprite2D() = default;

ASprite2D::~ASprite2D() = default;

void ASprite2D::DoNothing()
{

}

}