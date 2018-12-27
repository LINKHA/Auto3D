#include "BaseSpace.h"
#include "MotionSpace.h"
#include "Scene.h"
#include "NewDef.h"

namespace Auto3D {

BaseSpace::BaseSpace(Ambient* ambient)
	:Super(ambient)
{
}


BaseSpace::~BaseSpace()
{
}

void BaseSpace::Init()
{
	SpaceHandle::Instance().Init();
}

void BaseSpace::Destruct()
{
	SpaceHandle::Instance().Destruct();
}

}