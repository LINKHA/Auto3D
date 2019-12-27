#include "Actor.h"

namespace Auto3D
{

REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(AActor)
	.constructor<>()
	;
}
void AActor::BeginPlay()
{

}

void AActor::Tick(float DeltaSeconds)
{

}

void AActor::AddComponent(AComponent* component)
{
	if (component)
	{
		_ownedComponents.Insert(component);
	}
	else
	{
		ErrorString("Failed to create component, perhaps the input parameter component");
		return;
	}
	AddChild(component);
}

AComponent* AActor::CreateComponent(FStringHash childType)
{
	AComponent* component = dynamic_cast<AComponent*>(CreateChild(childType));

	if (component)
	{
		_ownedComponents.Insert(component);
	}
	else
	{
		ErrorString("Failed to create component, perhaps the input parameter component");
		return nullptr;
	}
		
	return component;
}

AComponent* AActor::CreateComponent(FStringHash childType, const FString& childName)
{
	AComponent* component = dynamic_cast<AComponent*>(CreateChild(childType, childName));

	if (component)
	{
		_ownedComponents.Insert(component);
	}
	else
	{
		ErrorString("Failed to create component, perhaps the input parameter component");
		return nullptr;
	}

	return component;
}

AComponent* AActor::CreateComponent(FStringHash childType, const char* childName)
{
	AComponent* component = dynamic_cast<AComponent*>(CreateChild(childType,childName));

	if (component)
	{
		_ownedComponents.Insert(component);
	}
	else
	{
		ErrorString("Failed to create component, perhaps the input parameter component");
		return nullptr;
	}

	return component;
}

}