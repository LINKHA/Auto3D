#pragma once
#include "Auto.h"
#include "LogAssert.h"
#define SINGLETON_INSTANCE(x) template<> x* Singleton<x>::_instance = nullptr
#define INSTANCE(x)	x::Instance()

namespace Auto3D {
/*
* Template class for creating single-instance global classes.
*/
template<typename T>
class Singleton
{
public:
	Singleton()
	{
		Assert(!_instance);
		_instance = static_cast<T*>(this);
	}
	/*
	* @brief Explicit private copy constructor. This is a forbidden operation.
	*/
	Singleton(const Singleton<T> &) = delete;
	/*
	* @brief Private operator= . This is a forbidden operation.
	*/
	Singleton& operator = (const Singleton<T> &) = delete;
	
	~Singleton()
	{
		Assert(_instance);  
		_instance = nullptr;
	}
	/**
	* @brief : get instance
	* @return : return Singleton m_instance
	*/
	static T& Instance()
	{
		if (_instance == nullptr)
		{
			_instance = new T();
		}
		return *_instance;
	}
	/**
	* @brief : get point* instance
	* @return : return Singleton m_instance
	*/
	static T* InstancePtr()
	{
		if (_instance == nullptr)
		{
			_instance = new T();
		}
		return _instance;
	}

	using ClassType = T;
protected:
	static T* _instance;
};

}
