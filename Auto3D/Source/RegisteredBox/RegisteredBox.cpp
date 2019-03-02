#include "RegisteredBox.h"


namespace Auto3D
{

RegisteredBox::RegisteredBox()
{
	RegisterSubsystem(this);
}
RegisteredBox::~RegisteredBox()
{
	RemoveSubsystem(this);
}


}