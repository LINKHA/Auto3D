#ifndef KH_STL_TYPE_SINGLETON_H_
#define KH_STL_TYPE_SINGLETON_H_

#include <memory>

namespace KhSTL {
// T must be: no-throw default constructible and no-throw destructible
template <typename _Ty>
struct tSingleton
{
private:
	struct ObjectCreator
	{
		/**
		* @brief : This constructor does nothing more than ensure that instance()
		*  is called before main() begins, thus creating the static
		*  T object before multithreading race issues can come up
		*/
		ObjectCreator() { tSingleton<_Ty>::Instance(); }
		inline void DoNothing() const { }
	};
	static ObjectCreator createObject;

	tSingleton();

public:
	typedef _Ty ObjectType;
	/**
	* @brief : If, at any point (in user code), tSingleton<T>::instance()
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
		*  of tSingleton<T>::createObject, whose constructor is
		*  called before main() begins
		*/
		createObject.DoNothing();

		return obj;
	}
};
template <typename _Ty> 
typename tSingleton<_Ty>::ObjectCreator 
tSingleton<_Ty>::createObject;
}
#endif //!KH_STL_TYPE_SINGLETON_H_