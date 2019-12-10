#pragma once
#include "GeometryNode2D.h"

namespace Auto3D 
{

class AUTO_API ASprite2D : public AGeometryNode2D
{
	DECLARE_CLASS(ASprite2D, AGeometryNode2D)

public:
	/// Constructor
	ASprite2D();
	/// Destructor
	~ASprite2D();
	/// Register factory and attributes.
	static void RegisterObject();
};

}