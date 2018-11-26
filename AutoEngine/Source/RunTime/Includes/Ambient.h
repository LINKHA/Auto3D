#pragma once
#include "RefCounted.h"
#include "Object.h"
#include "ObjectFactory.h"
namespace Auto3D {

class Ambient : public RefCounted
{
	friend class Object;
	using SubSystems = HASH_MAP<STRING, sharedPtr<Object>>;
	using Factories = HASH_MAP<STRING, sharedPtr<ObjectFactory>>;
	using ObjectAttachs = HASH_MAP<STRING, VECTOR<STRING>>;
public:
	Ambient();
	~Ambient()override;
	/**
	* @brief : Create an object by type. Return pointer to it or null if no factory found.
	*/

#if SharedPtrDebug
	template <typename T> inline sharedPtr<T> CreateObject(){ return StaticCast<T>(CreateObject(T::GetClassStringStatic())); }
#else
	template <typename T> inline T* CreateObject() { return static_cast<T*>(CreateObject(T::GetClassStringStatic())); }
#endif

	/**
	* @brief : Create object by string type , Return pointer to it or null if no factory found.
	*/
#if SharedPtrDebug
	sharedPtr<Object> CreateObject(STRING objectType);
#else
	Object* CreateObject(STRING objectType);
#endif
	/**
	* @brief : Register sub system need extend object
	*/
	void RegisterSubSystem(Object* object);
	/**
	* @brief : Template version of registering subsystem.
	*/
	template <typename T> T* RegisterSubsystem();
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
	template <typename T> void RegisterFactory();
	/**
	* @brief : Template version of registering an object factory with category.
	*/
	template <typename T> void RegisterFactory(const char* category);
	/**
	* @brief : Get sub system according to the parameters
	*/
	Object* GetSubSystem(STRING type) const;
	/**
	* @brief : Get sub systems
	* @return :HASH_MAP(STRING, sharedPtr<Object>)
	*/
	const SubSystems& GetSubSystems() const { return _subSystems; }
	/**
	* @brief : Get sub system by type
	*/
	template<typename T> T* GetSubSystem() const;
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
	template <typename T> void RemoveSubSystem();
private:
	///sub systems hash map
	SubSystems _subSystems;
	///object factories hash map
	Factories _factories;
	///object attach hash map
	ObjectAttachs _objectAttachs;
};

template <typename T> inline T * Ambient::RegisterSubsystem()
{
	auto* subsystem = new T(this);
	RegisterSubsystem(subsystem);
	return subsystem;
}

template<typename T> void Ambient::RemoveSubSystem(){ RemoveSubSystem(T::GetClassStringStatic()); }

template<typename T> T* Ambient::GetSubSystem() const { return static_cast<T*>(Ambient::GetSubSystem(T::GetClassStringStatic())); }

template<typename T> void Ambient::RegisterFactory() { RegisterFactory(new ObjectFactoryImpl<T>(this)); }

template<typename T> void Ambient::RegisterFactory(const char * category) { RegisterFactory(new ObjectFactoryImpl<T>(this), category); }

}