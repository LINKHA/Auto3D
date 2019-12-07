#pragma once
#include "AutoConfig.h"

namespace Auto3D
{
namespace RTTI
{

class FRegistrationManager;
class FTypeData;

class AUTO_API FTypeRegister
{
public:
	/// No copy.
	FTypeRegister(const FTypeRegister&) = delete;
	/// No assign.
	FTypeRegister& operator=(const FTypeRegister&) = delete;

	static void RegisterRegManager(FRegistrationManager* manager);
	static void UnregisterRegManager(FRegistrationManager* manager);

	static FTypeData* RegisterType(FTypeData* info)noexcept;
	static void UnregisterType(FTypeData* info) noexcept;
};

}
}