#pragma once
#include "RefCounted.h"
#include "Object.h"
#include "stl_use.h"
namespace Auto3D {
class Ambient : public RefCounted
{
	friend class Object;
public:
	Ambient();
	~Ambient()override;
	/**
	* @brief :
	*/
	void RegisterFactory(ObjectFactory* factory); 
	/**
	* @brief : Register sub system need extend object
	*/
	void RegisterSubSystem(Object* object);
	/**
	* @brief :
	*/
	void RemoveSubSystem(_String objectType);
	/**
	* @brief : Get sub system according to the parameters
	*/
	Object* GetSubSystem(_String type) const;
	/**
	* @brief : Get sub system by type
	*/
	template<typename T> T* GetSubSystem() const;
	/**
	* @brief : Get sub systems
	* @return :AUTO_HASH_MAP(_String, SharedPtr<Object>)
	*/
	const AUTO_HASH_MAP(_String, SharedPtr<Object>)& GetSubSystems() const { return _subSystems; }
private:
	AUTO_HASH_MAP(_String, SharedPtr<Object>) _subSystems;
};

template <typename T> 
T* Ambient::GetSubSystem() const 
{ 
	return static_cast<T*>(Ambient::GetSubSystem(T::GetClassStringStatic()));
}


}