#include "Physics3DSpace.h"
#include "Application.h"
#include "../FreeCamera.h"
#include "Mesh.h"
#include "Sprite.h"
#include "Input.h"
#include "Time.h"
#include "Level_0.h"
#include "tchar.h"

Physics3DSpace::Physics3DSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
Physics3DSpace::~Physics3DSpace()
{
	RemoveScene(0);
}
void Physics3DSpace::Awake()
{
	RegisterScene(new Level_0(_ambient, 0));
}
void Physics3DSpace::Start()
{
}
void Physics3DSpace::Update()
{
}
int Physics3DSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(Physics3DSpace)