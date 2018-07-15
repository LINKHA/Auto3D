#pragma once
// Every non-abstract class that is derived from object has to place this inside the class Declaration
#define	REGISTER_DERIVED_CLASS(x,y) \
public: \
	static int GetClassIDStatic ()				{ return ClassID (x); }\
	static bool IsAbstract ()					{ return false; }\
	static const char* GetClassStringStatic ()	{ return #x; }\
	static const char* GetPPtrTypeString ()		{ return "PPtr<"#x">"; }\
	virtual int GetClassID() const				{ return ClassID (x); }\
	typedef y Super;\
	using ClassName = x;\
	using SuperName = y;\
	virtual ~x (); \
protected: \
	void ClassInit()							{ SetClassID(ClassID(x)); }\
public:
// Every abstract class that is derived from object has to place this inside the class Declaration
#define	REGISTER_DERIVED_ABSTRACT_CLASS(x, y) \
public: \
	static int GetClassIDStatic ()				{ return ClassID (x); } \
	static bool IsAbstract ()					{ return true; }\
	static const char* GetClassStringStatic ()	{ return #x; }\
	static const char* GetPPtrTypeString ()		{ return "PPtr<"#x">"; }\
	virtual int GetClassID() const				{ return ClassID (x); }\
	typedef y Super; \
	using ClassName = x;\
	using SuperName = y;\
	virtual ~x (); \
protected:\
	void ClassInit()							{ SetClassID(ClassID(x)); }\
public:


// Should be placed in every serializable object derived class
#define DECLARE_OBJECT_SERIALIZE(x)\
	static const char* GetTypeString ()	 { return GetClassStringStatic(); }\
	template<class TransferFunction> void Transfer (TransferFunction& transfer);


#define Ptr(x,y)\
struct _##y##Message{\
	x * ptr;\
	_##y##Message()\
		:ptr(nullptr){}\
	~_##y##Message(){/*SAFE_DELETE(ptr);*/}\
};\
_##y##Message y;

#define GET_SET(TYPE,PROP_NAME,VAR_NAME)	void Set##PROP_NAME (TYPE val) { VAR_NAME = val; }	const TYPE Get##PROP_NAME () const {return (const TYPE)VAR_NAME; }

