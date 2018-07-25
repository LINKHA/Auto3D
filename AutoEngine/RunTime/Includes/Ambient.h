#pragma once
#include "RefCounted.h"
#include "Object.h"
#include "stl_use.h"
#include "Ptr.h"
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
	Object* GetSubSystem(_String type) const;
	template<typename T> T* GetSubSystem() const;
	const AUTO_HASH_MAP(_String, SharedPtr<Object>)& GetSubSystems() const { return _subSystems; }
private:
	AUTO_HASH_MAP(_String, SharedPtr<Object>) _subSystems;
};
template <typename T> T* Ambient::GetSubSystem() const { return static_cast<T*>(Ambient::GetSubSystem(T::GetClassStringStatic()));}
AUTO_END