#pragma once
// Every non-abstract class that is derived from object has to place this inside the class Declaration
#define	REGISTER_DERIVED_CLASS(x,y) \
public: \
	using This = x;\
	using Super = y;\
	~x (); \
	virtual int GetClassID() { return ClassID (x); }\
	virtual bool IsAbstract() { return GetRTTIStatic()->GetIsAbstract();}\
	virtual char* GetClassCstrName() { return #x; }\
	virtual _String GetClassString() { return #x; } \
	virtual Auto3D::RTTI* GetRTTI() { return GetRTTIStatic(); }\
	static int GetClassIDStatic() { return ClassID(x); }\
	static bool IsAbstractStatic () { return GetRTTIStatic()->GetIsAbstract();}\
	static char* GetClassCstrNameStatic () { return #x; }\
	static _String GetClassStringStatic() { return #x; } \
	static Auto3D::RTTI* GetRTTIStatic() { static Auto3D::RTTI RTTIStatic(#x, y::GetRTTIStatic(), ClassID(x), false);return &RTTIStatic;}\

// Every abstract class that is derived from object has to place this inside the class Declaration
#define	REGISTER_DERIVED_ABSTRACT_CLASS(x, y) \
public: \
	using This = x; \
	using Super = y; \
	virtual ~x (); \
	virtual int GetClassID() { return ClassID (x); }\
	virtual bool IsAbstract() { return GetRTTIStatic()->GetIsAbstract();}\
	virtual char* GetClassCstrName() { return #x; }\
	virtual _String GetClassString() { return #x; } \
	virtual Auto3D::RTTI* GetRTTI() { return GetRTTIStatic(); }\
	static int GetClassIDStatic() { return ClassID(x); }\
	static bool IsAbstractStatic () { return GetRTTIStatic()->GetIsAbstract();}\
	static char* GetClassCstrNameStatic () { return #x; }\
	static _String GetClassStringStatic() { return #x; } \
	static Auto3D::RTTI* GetRTTIStatic() { static Auto3D::RTTI RTTIStatic(#x, y::GetRTTIStatic(), ClassID(x), true);return &RTTIStatic;}\

// Should be placed in every serializable object derived class
#define DECLARE_OBJECT_SERIALIZE(x)\
public: \


