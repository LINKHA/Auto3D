#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_
#include "Auto.h"


AUTO_BEGIN
class BaseObject;

template<class T>
class PPtr
{
	AInt32 m_InstanceCD;
	static const char* GetTypeString();
protected:
	inline void AssignObject(const BaseObject* o);




};









class BaseObject 
{
	BaseObject();
	virtual ~BaseObject();

};
AUTO_END
#endif // !BASE_OBJECT_H_
