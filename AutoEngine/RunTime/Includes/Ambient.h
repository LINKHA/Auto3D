#pragma once
#include "RefCounted.h"
#include "Object.h"
AUTO_BEGIN
class Ambient : public RefCounted
{
	friend class Object;
public:
	Ambient();
	~Ambient()override;
	void RegisterFactory(ObjectFactory* factory);  
	void RegisterSubSystem(Object* object);
	void RemoveSubSystem(_String objectType);

	Object* GetSubSystem()const;
};

AUTO_END