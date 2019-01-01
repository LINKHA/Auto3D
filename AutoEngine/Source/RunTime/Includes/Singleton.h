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
	static std::shared_ptr Instance()
	{
		if (_instance == nullptr)
		{
			_instance = std::make_shared<_Ty>();
		}
		return _instance;
	}

protected:
	friend class std::shared_ptr<Singleton<_Ty> >;
	static std::shared_ptr<Singleton<_Ty> > _instance;
	//static _Ty* _instance;
private:
	// Garbage collection class
	class GarbageCollection
	{
	public:
		GarbageCollection(){}
		~GarbageCollection()
		{
			// We can destory all the resouce here, eg:db connector, file handle and so on
			if (_instance)
			{
				delete _instance;
				_instance = nullptr;
			}
		}
	};
	static GarbageCollection _garbageCollection;
};

template<typename _Ty> _Ty* Singleton<_Ty>::_instance = nullptr;
//template<typename _Ty> Singleton<_Ty>::GarbageCollection Singleton<_Ty>::_garbageCollection;

}
