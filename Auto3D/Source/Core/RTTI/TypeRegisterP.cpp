#include "TypeRegisterP.h"
#include "Core/RTTI/TypeData.h"

namespace Auto3D
{
void GTypeRegisterPrivate::RegisterRegManager(FRegistrationManager* manager)
{
	_registrationManagerList.Insert(manager);
}
void GTypeRegisterPrivate::UnregisterRegManager(FRegistrationManager* manager)
{
	_registrationManagerList.Erase(manager);
}

FTypeData* GTypeRegisterPrivate::RegisterType(FTypeData* info) noexcept
{
	
}

void GTypeRegisterPrivate::UnregisterType(FTypeData* info) noexcept
{

}
GTypeRegisterPrivate& GTypeRegisterPrivate::GetInstance()
{
	static GTypeRegisterPrivate obj;
	return obj;
}

}