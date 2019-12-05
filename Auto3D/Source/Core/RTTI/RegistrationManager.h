#pragma once
#include "AutoConfig.h"
#include "Container/Ptr.h"
#include "Core/RTTI/TypeData.h"
#include ""
namespace Auto3D
{

class AUTO_API FRegistrationManager
{

public:
	FRegistrationManager()
	{
		FTypeRegister::RegisterRegManager(this);
	}
	~FRegistrationManager()
	{
		
	}

	FTypeData* AddTtem(TUniquePtr<FTypeData> typeData)
	{
		auto regType = FTypeRegister::RegisterType(typeData.Get());
		const auto wasTypeStored = (regType == typeData.Get());
		if (wasTypeStored)
			_typeDataList.Push(std::move(typeData)); // so we have to unregister it later

		return regType;
	}
private:
	TVector<TUniquePtr<FTypeData> >  _typeDataList;
};

inline FRegistrationManager& GetRegistrationManager() noexcept
{
	static FRegistrationManager obj;
	return obj;
}

}