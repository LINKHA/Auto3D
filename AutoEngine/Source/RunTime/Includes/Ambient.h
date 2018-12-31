#pragma once
#include "Object.h"

namespace Auto3D {

class Ambient
{
	friend class Object;
	using SubSystems = HASH_MAP<STRING, SharedPtr<Object>>;
	using Factories = HASH_MAP<STRING, SharedPtr<ObjectFactory>>;
	using ObjectAttachs = HASH_MAP<STRING, VECTOR<STRING>>;
public:
	Ambient();
	~Ambient();
	/**
	* @brief : Create an object by type. Return pointer to it or null if no factory found.
	*/
	template <typename _Ty> inline SharedPtr<_Ty> CreateObject(){ return StaticCast<_Ty>(CreateObject(T::GetClassStringStatic())); }
	/**
	* @brief : Create object by string type , Return pointer to it or null if no factory found.
	*/
	SharedPtr<Object> CreateObject(STRING objectType);
	/**
	* @brief : Register sub system need extend object
	*/
	void RegisterSubSystem(Object* object);
	/**
	* @brief : Template version of registering subsystem.
	*/
	template <typename _Ty> _Ty* RegisterSubsystem();
	/**
	* @brief : Register a factory for an object type.
	*/
	void RegisterFactory(ObjectFactory* factory);
	/**
	* @brief : Register a factory for an object type and specify the object category.
	*/
	void RegisterFactory(ObjectFactory* factory, const char* category);
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
	const SubSystems& GetSubSystems() const { return _subSystems; }
	/**
	* @brief : Get sub system by type
	*/
	template<typename _Ty> SharedPtr<_Ty> GetSubSystem() const;
	/**
	* @brief : Return all subsystems.
	*/
	const SubSystems& GetSubsystems() const { return _subSystems; }
	/**
	* @brief : Return all object factories.
	*/
	const Factories& GetObjectFactories() const { return _factories; }
	/**
	* @brief : Return all object attach
	*/
	const ObjectAttachs& GetObjectAttachs() const { return _objectAttachs; }
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
	SubSystems _subSystems;
	///object factories hash map
	Factories _factories;
	///object attach hash map
	ObjectAttachs _objectAttachs;
};

template <typename _Ty> inline _Ty * Ambient::RegisterSubsystem()
{
	auto* subsystem = new _Ty(this);
	RegisterSubsystem(subsystem);
	return subsystem;
}

template<typename _Ty> void Ambient::RemoveSubSystem(){ RemoveSubSystem(_Ty::GetClassStringStatic()); }

template<typename _Ty> SharedPtr<_Ty> Ambient::GetSubSystem() const { return StaticCast<_Ty>(Ambient::GetSubSystem(_Ty::GetClassStringStatic())); }

template<typename _Ty> void Ambient::RegisterFactory() { RegisterFactory(new ObjectFactoryImpl<_Ty>(SharedPtr<Ambient>(this))); }

template<typename _Ty> void Ambient::RegisterFactory(const char* category) { RegisterFactory(new ObjectFactoryImpl<_Ty>(SharedPtr<Ambient>(this)), category); }

}