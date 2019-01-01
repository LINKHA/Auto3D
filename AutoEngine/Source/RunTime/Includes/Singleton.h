#pragma once
#include <memory>

namespace Auto3D {
/*
* Template class for creating single-instance global classes.
*/
template<typename _Ty> class Singleton
{
public:
	using ClassType = _Ty;
public:
	Singleton()
	{}

	virtual ~Singleton()
	{}
	/*
	* @brief Explicit private copy constructor. This is a forbidden operation.
	*/
	Singleton(const Singleton<_Ty> &) = delete;
	/*
	* @brief Private operator= . This is a forbidden operation.
	*/
	Singleton& operator = (const Singleton<_Ty> &) = delete;
	
	/**
	* @brief : get point* instance
	* @return : return Singleton m_instance
	*/
	static _Ty* Instance()
	{
		if (_instance == nullptr)
		{
			_instance = new _Ty();
		}
		return _instance;
	}

protected:
	static _Ty* _instance;
};

template<typename _Ty> _Ty* Singleton<_Ty>::_instance = nullptr;

}
