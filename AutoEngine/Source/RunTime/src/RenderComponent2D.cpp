#include "RenderComponent2D.h"
#include "Ambient.h"
#include "NewDef.h"

namespace Auto3D {

RenderComponent2D::RenderComponent2D(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
}


RenderComponent2D::~RenderComponent2D()
{
}

}