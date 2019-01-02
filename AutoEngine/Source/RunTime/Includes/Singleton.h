#pragma once
#include <memory>

namespace Auto3D {

// T must be: no-throw default constructible and no-throw destructible
template <typename _Ty>
struct Singleton
{
private:
	struct ObjectCreator
	{
		// This constructor does nothing more than ensure that instance()
		//  is called before main() begins, thus creating the static
		//  T object before multithreading race issues can come up.
		ObjectCreator() { Singleton<_Ty>::Instance(); }
		inline void DoNothing() const { }
	};
	static ObjectCreator createObject;

	Singleton();

public:
	typedef _Ty ObjectType;

	// If, at any point (in user code), Singleton<T>::instance()
	//  is called, then the following function is instantiated.
	static ObjectType& Instance()
	{
		// This is the object that we return a reference to.
		// It is guaranteed to be created before main() begins because of
		//  the next line.
		static ObjectType obj;

		// The following line does nothing else than force the instantiation
		//  of Singleton<T>::createObject, whose constructor is
		//  called before main() begins.
		createObject.DoNothing();

		return obj;
	}
};
template <typename _Ty>
typename Singleton<_Ty>::ObjectCreator
Singleton<_Ty>::createObject;

}
