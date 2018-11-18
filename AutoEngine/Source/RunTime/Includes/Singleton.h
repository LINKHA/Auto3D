#pragma once
#include "Auto.h"
#include "LogAssert.h"
#define SINGLETON_INSTANCE(x) template<> x* Singleton<x>::_instance = nullptr
#define INSTANCE(x)	x::Instance()

namespace Auto3D {
/*
* Template class for creating single-instance global classes.
*/
template<typename _Ty>
class Singleton
{
public:
	Singleton()
	{
		Assert(!_instance);
		_instance = static_cast<_Ty*>(this);
	}
	/*
	* @brief Explicit private copy constructor. This is a forbidden operation.
	*/
	Singleton(const Singleton<_Ty> &) = delete;
	/*
	* @brief Private operator= . This is a forbidden operation.
	*/
	Singleton& operator = (const Singleton<_Ty> &) = delete;
	
	virtual ~Singleton()
	{
		Assert(_instance);  
		_instance = nullptr;
	}
	/**
	* @brief : get instance
	* @return : return Singleton m_instance
	*/
	static _Ty& Instance()
	{
		if (_instance == nullptr)
		{
			_instance = new _Ty();
		}
		return *_instance;
	}
	/**
	* @brief : get point* instance
	* @return : return Singleton m_instance
	*/
	static _Ty* InstancePtr()
	{
		if (_instance == nullptr)
		{
			_instance = new _Ty();
		}
		return _instance;
	}

	using ClassType = _Ty;
protected:
	static _Ty* _instance;
};

}
