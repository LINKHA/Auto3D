#ifndef OBJECT_DEFINES_H_
#define OBJECT_DEFINES_H_

// Every non-abstract class that is derived from object has to place this inside the class Declaration
#define	REGISTER_DERIVED_CLASS(x,y) \
public: \
	static int GetClassIDStatic ()				{ return ClassID (x); }\
	static bool IsAbstract ()					{ return false; }\
	static const char* GetClassStringStatic ()	{ return #x; }\
	static const char* GetPPtrTypeString ()		{ return "PPtr<"#x">"; }\
	typedef y Super;\
protected: \
	virtual ~x (); \
public:
// Every abstract class that is derived from object has to place this inside the class Declaration
#define	REGISTER_DERIVED_ABSTRACT_CLASS(x, y) \
public: \
	static int GetClassIDStatic ()				{ return ClassID (x); } \
	static bool IsAbstract ()					{ return true; }\
	static const char* GetClassStringStatic ()	{ return #x; }\
	static const char* GetPPtrTypeString ()		{ return "PPtr<"#x">"; }\
	typedef y Super; \
protected: \
	virtual ~x (); \
public:


// Should be placed in every serializable object derived class
#define DECLARE_OBJECT_SERIALIZE(x)\
	static const char* GetTypeString ()	 { return GetClassStringStatic(); }\
	template<class TransferFunction> void Transfer (TransferFunction& transfer);




#endif // OBJECT_DEFINES_H_
