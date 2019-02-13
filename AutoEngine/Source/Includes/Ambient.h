#pragma once
#include "Object.h"

namespace Auto3D {

/**
* All Auto class ambient ,Responsible for registering objects such as factories
*/
class Ambient : public EnableSharedFromThis<Ambient>
{
	friend class Object;
public:
	Ambient();
	~Ambient();
	/**
	* @brief : Create an object by type. Return pointer to it or null if no factory found.
	*/
	template <typename _Ty> inline SharedPtr<_Ty> CreateObject(){ return StaticCast<_Ty>(CreateObject(_Ty::GetClassStringStatic())); }
	/**
	* @brief : Create object by string type , Return pointer to it or null if no factory found.
	*/
	SharedPtr<Object> CreateObject(STRING objectType);
	/**
	* @brief : Register sub system need extend object
	*/
	void RegisterSubSystem(SharedPtr<Object> object);
	/**
	* @brief : Template version of registering subsystem.
	*/
	template <typename _Ty> SharedPtr<_Ty> RegisterSubSystem();
	/**
	* @brief : Register a factory for an object type.
	*/
	void RegisterFactory(SharedPtr<ObjectFactory> factory);
	/**
	* @brief : Register a factory for an object type and specify the object category.
	*/
	void RegisterFactory(SharedPtr<ObjectFactory> factory, const char* category);
	/**
	* @brief : Template version of registering an object factory.
	*/
	template <typename _Ty> void RegisterFactory();
	/**
	* @brief : Template version of registering an object factory with category.
	*/
	template <typename _Ty> void RegisterFactory(const char* category);
	/**
	* @brief : Get sub system according to the parameters
	*/
	SharedPtr<Object> GetSubSystem(STRING type) const;
	/**
	* @brief : Get sub systems
	* @return :HASH_MAP(STRING, sharedPtr<Object>)
	*/
	const HASH_MAP<STRING, SharedPtr<Object>>& GetSubSystems() const { return _subSystems; }
	/**
	* @brief : Get sub system by type
	*/
	template<typename _Ty> SharedPtr<_Ty> GetSubSystem() const;
	/**
	* @brief : Return all subsystems.
	*/
	const HASH_MAP<STRING, SharedPtr<Object>>& GetSubsystems() const { return _subSystems; }
	/**
	* @brief : Return all object factories.
	*/
	const HASH_MAP<STRING, SharedPtr<ObjectFactory>>& GetObjectFactories() const { return _factories; }
	/**
	* @brief : Return all object attach
	*/
	const HASH_MAP<STRING, VECTOR<STRING>>& GetObjectAttachs() const { return _objectAttachs; }
	/**
	* @brief : Remove a sub system from _subSystems
	*/
	void RemoveSubSystem(STRING objectType);
	/**
	* @brief : Template version of removing a subsystem.
	*/
	template <typename _Ty> void RemoveSubSystem();
private:
	///sub systems hash map
	HASH_MAP<STRING, SharedPtr<Object>> _subSystems;
	///object factories hash map
	HASH_MAP<STRING, SharedPtr<ObjectFactory>> _factories;
	///object attach hash map
	HASH_MAP<STRING, VECTOR<STRING>> _objectAttachs;
};

template <typename _Ty> inline SharedPtr<_Ty> Ambient::RegisterSubSystem()
{
	auto subsystem = MakeShared<_Ty>(SharedFromBase());
	RegisterSubSystem(subsystem);
	return subsystem;
}

template<typename _Ty> void Ambient::RemoveSubSystem(){ RemoveSubSystem(_Ty::GetClassStringStatic()); }

template<typename _Ty> SharedPtr<_Ty> Ambient::GetSubSystem() const { return StaticCast<_Ty>(Ambient::GetSubSystem(_Ty::GetClassStringStatic())); }

template<typename _Ty> void Ambient::RegisterFactory() { RegisterFactory(MakeShared<ObjectFactoryImpl<_Ty> >(SharedFromBase())); }

template<typename _Ty> void Ambient::RegisterFactory(const char* category) { RegisterFactory(MakeShared<ObjectFactoryImpl<_Ty> >(SharedFromBase()), category); }

}