#include "VertexExplodeSpace.h"
#include "ResourceSystem.h"
#include "Level_0.h"

VertexExplodeSpace::VertexExplodeSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}

VertexExplodeSpace::~VertexExplodeSpace()
{
	RemoveScene(0);
}

void VertexExplodeSpace::Awake()
{
	GetSubSystem<ResourceSystem>()->AddResourceDir("../Resource/");
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(VertexExplodeSpace)