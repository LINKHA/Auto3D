#pragma once
#include "Auto.h"
#include "LogAssert.h"
#define SINGLETON_INSTANCE(x) template<> x* Singleton<x>::_instance = nullptr
#define INSTANCE(x)	x::Instance()

AUTO_BEGIN
/*
* Template class for creating single-instance global classes.
*/
template<typename T>
class Singleton
{
private:
	/*
	* @brief Explicit private copy constructor. This is a forbidden operation.
	*/
	Singleton(const Singleton<T> &);
	/*
	* @brief Private operator= . This is a forbidden operation. 
	*/
	Singleton& operator=(const Singleton<T> &);
	
protected:
	static T* _instance;
	
public:
	Singleton()
	{
		assert(!_instance);
		_instance = static_cast<T*>(this);
	}
	~Singleton()
	{
		assert(_instance);  
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
};

AUTO_END
