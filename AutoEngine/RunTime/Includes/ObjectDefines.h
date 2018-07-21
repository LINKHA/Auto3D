#pragma once
// Every non-abstract class that is derived from object has to place this inside the class Declaration
#define	REGISTER_DERIVED_CLASS(x,y) \
public: \
	using This = x;\
	using Super = y;\
	~x (); \
	static int GetClassIDStatic ()						{ return ClassID (x); }\
	static bool IsAbstractStatic ()						{ return false; }\
	static const char* GetClassStringStatic ()			{ return #x; }\
	static const char* GetSharedPtrTypeStringStatic ()	{ return "SharedPtr<"#x">"; }\

// Every abstract class that is derived from object has to place this inside the class Declaration
#define	REGISTER_DERIVED_ABSTRACT_CLASS(x, y) \
public: \
	using This = x; \
	using Super = y; \
	virtual ~x (); \
	static int GetClassIDStatic ()						{ return ClassID (x); } \
	static bool IsAbstractStatic ()						{ return true; }\
	static const char* GetClassStringStatic ()			{ return #x; }\
	static const char* GetSharedPtrTypeStringStatic ()	{ return "SharedPtr<"#x">"; }\

// Should be placed in every serializable object derived class
#define DECLARE_OBJECT_SERIALIZE(x)\
public: \
	static const char* GetTypeString ()					{ return GetClassStringStatic(); }\
	virtual int GetClassIDVirtual() const				{ return ClassID (x); }\
	virtual const char* GetClassStringVirtual()			{ return #x; }\
	virtual const char* GetSharedPtrTypeStringVirtual()	{ return "SharedPtr<"#x">"; }

#define Ptr(x,y)\
struct _##y##Message{\
	x * ptr;\
	_##y##Message()\
		:ptr(nullptr){}\
	~_##y##Message(){/*SAFE_DELETE(ptr);*/}\
};\
_##y##Message y;

#define GET_SET(TYPE,PROP_NAME,VAR_NAME)	void Set##PROP_NAME (TYPE val) { VAR_NAME = val; }	const TYPE Get##PROP_NAME () const {return (const TYPE)VAR_NAME; }

