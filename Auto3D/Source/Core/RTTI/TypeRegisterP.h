#pragma once
#include "AutoConfig.h"
#include "Container/Vector.h"
#include "Container/HashSet.h"
namespace Auto3D
{

class FRegistrationManager;

class AUTO_API GTypeRegisterPrivate
{
public:

	void RegisterRegManager(FRegistrationManager* manager);
	void UnregisterRegManager(FRegistrationManager* manager);

	FTypeData* RegisterType(FTypeData* info) noexcept;
	void UnregisterType(FTypeData* info) noexcept;

	static GTypeRegisterPrivate& GetInstance();


	THashSet<FRegistrationManager*>  _registrationManagerList;
};

}