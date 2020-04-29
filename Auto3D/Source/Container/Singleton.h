#pragma once

#include <memory>

namespace Auto3D {
#define REGISTER_SINGLETON(_This) \
public:\
	friend struct TSingleton<_This>;\
	static _This& Get();\

#define IMPLEMENT_SINGLETON(_This) \
	_This& _This::Get(){ return TSingleton<_This>::Instance(); }

// T must be: no-throw default constructible and no-throw destructible
template <typename _Ty>
struct TSingleton
{
private:
	struct ObjectCreator
	{
		/**
		* @brief : This constructor does nothing more than ensure that instance()
		*  is called before main() begins, thus creating the static
		*  T object before multithreading race issues can come up
		*/
		ObjectCreator() { TSingleton<_Ty>::Instance(); }
		inline void DoNothing() const { }
	};
	static ObjectCreator createObject;

	TSingleton();

public:
	typedef _Ty ObjectType;
	/**
	* @brief : If, at any point (in user code), TSingleton<T>::instance()
	*		  is called, then the following function is instantiated
	*/
	static ObjectType& Instance()
	{
		/**
		* @brief :This is the object that we return a reference to.
		*		It is guaranteed to be created before main() begins because of
		*		the next line
		*/
		static ObjectType obj;
		/**
		* @brief :The following line does nothing else than force the instantiation
		*  of TSingleton<T>::createObject, whose constructor is
		*  called before main() begins
		*/
		createObject.DoNothing();

		return obj;
	}
};
template <typename _Ty>
typename TSingleton<_Ty>::ObjectCreator
	TSingleton<_Ty>::createObject;
}