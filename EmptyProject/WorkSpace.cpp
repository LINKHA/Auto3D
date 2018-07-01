#include "WorkSpace.h"
#include "Application.h"

WorkSpace::WorkSpace()
{}
WorkSpace::~WorkSpace()
{}

void WorkSpace::Start()
{
}

void WorkSpace::Update()
{
}

int WorkSpace::Launch()
{
	return INSTANCE(Application).Run();
}
