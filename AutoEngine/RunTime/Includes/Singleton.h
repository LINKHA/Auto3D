#ifndef SINGLETON_H_
#define SINGLETON_H_
#include "Auto.h"
#include "LogAssert.h"
#define SINGLETON_INSTANCE(x) template<> x* Singleton<x>::m_instance = nullptr
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
	static T* m_instance;
	
public:
	Singleton()
	{
		Assert(!m_instance);
		m_instance = static_cast<T*>(this);
	}
	~Singleton()
	{
		Assert(m_instance);  
		m_instance = nullptr;
	}
	/**
	* @brief : get instance
	* @return : return Singleton m_instance
	*/
	static T& Instance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new T();
		}
		return *m_instance;
	}
	/**
	* @brief : get point* instance
	* @return : return Singleton m_instance
	*/
	static T* InstancePtr()
	{
		if (m_instance == nullptr)
		{
			m_instance = new T();
		}
		return m_instance;
	}

	typedef T classType;
};

AUTO_END
#endif //!SINGLETON_H_