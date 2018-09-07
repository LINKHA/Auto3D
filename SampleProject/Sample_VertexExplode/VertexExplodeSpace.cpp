#include "VertexExplodeSpace.h"
#include "Application.h"
#include "Level_0.h"


VertexExplodeSpace::VertexExplodeSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}

VertexExplodeSpace::~VertexExplodeSpace()
{
	RemoveLevel(0);
}

void VertexExplodeSpace::Awake()
{
	RegisterLevel(new Level_0(_ambient, 0));
}

int VertexExplodeSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(VertexExplodeSpace)