#pragma once
#include "Auto.h"
namespace Auto3D {

/**
* type info
*/
class RTTI
{
public:
	/**
	* @brief : Construct
	*/
	RTTI(const char* className, const RTTI* baseRTTI, int classID, bool isAbstract)
		: _className(className)
		, _base(baseRTTI)
		, _classID(classID)
		, _isAbstract(isAbstract)
	{}
	/**
	* @brief : Destructor
	*/
	~RTTI() = default;
	/**
	* @brief : Return class name for string
	*/
	const STRING GetClassString() { return _className; }
	/**
	* @brief : Return class name for c str
	*/
	const char* GetClassCstrName() { return (_className).CStr(); }
	/**
	* @brief : Return is abstract
	*/
	const bool GetIsAbstract() { return _isAbstract; }
	/**
	* @brief : Return base RTTI
	*/
	const RTTI* GetBaseRTTI() { return _base; }
	/**
	* @brief : Return class id
	*/
	const int GetClassID() { return _classID; }
private:
	/// class name for string
	STRING _className;
	/// super rtti class
	const RTTI* _base;
	/// class Id
	int _classID;
	/// is the class Abstract
	bool _isAbstract;
};

}