#include "StencilTestSpace.h"
#include "Application.h"
StencilTestSpace::StencilTestSpace()
{
}
StencilTestSpace::~StencilTestSpace()
{
}

void StencilTestSpace::Awake()
{

}
void StencilTestSpace::Start()
{

}
void StencilTestSpace::Update()
{

}
int StencilTestSpace::Launch()
{
	return INSTANCE(Application).Run();
}
