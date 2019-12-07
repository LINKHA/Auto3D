#include "Core/RTTI/TypeRegister.h"
#include "Core/RTTI/RegistrationManager.h"
#include "core/RTTI/TypeRegisterP.h"
#include "Core/RTTI/TypeData.h"

namespace Auto3D
{
namespace RTTI
{

void FTypeRegister::RegisterRegManager(FRegistrationManager* manager)
{
	GTypeRegisterPrivate::GetInstance().RegisterRegManager(manager);
}

void FTypeRegister::UnregisterRegManager(FRegistrationManager* manager)
{
	GTypeRegisterPrivate::GetInstance().UnregisterRegManager(manager);
}



FTypeData* FTypeRegister::RegisterType(FTypeData* info) noexcept
{
	return GTypeRegisterPrivate::GetInstance().RegisterType(info);
}

void FTypeRegister::UnregisterType(FTypeData* info) noexcept
{
	GTypeRegisterPrivate::GetInstance().UnregisterType(info);
}
}
}