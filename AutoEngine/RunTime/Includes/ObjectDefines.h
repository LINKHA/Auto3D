#pragma once
// Every non-abstract class that is derived from object has to place this inside the class Declaration
#define	REGISTER_DERIVED_CLASS(x,y) \
public: \
	using This = x;\
	using Super = y;\
	~x (); \
	virtual int GetClassID()							{ return ClassID (x); }\
	virtual bool IsAbstract()							{ return GetRTTIStatic()->GetIsAbstract();}\
	virtual const char* GetClassName()					{ return #x; }\
	virtual const _String GetClassString()				{ return #x; } \
	virtual const Auto3D::RTTI* GetRTTI()				{ return GetRTTIStatic(); }\
	static int GetClassIDStatic()						{ return ClassID(x); }\
	static bool IsAbstractStatic ()						{ return GetRTTIStatic()->GetIsAbstract();}\
	static const char* GetClassNameStatic ()			{ return #x; }\
	static const _String GetClassStringStatic()			{ return #x; } \
	static const Auto3D::RTTI* GetRTTIStatic()			{ const static Auto3D::RTTI RTTIStatic(#x, y::GetRTTIStatic(), ClassID(x), false);return &RTTIStatic;}\

// Every abstract class that is derived from object has to place this inside the class Declaration
#define	REGISTER_DERIVED_ABSTRACT_CLASS(x, y) \
public: \
	using This = x; \
	using Super = y; \
	virtual ~x (); \
	virtual int GetClassID()							{ return ClassID (x); }\
	virtual bool IsAbstract()							{ return GetRTTIStatic()->GetIsAbstract();}\
	virtual const char* GetClassName()					{ return #x; }\
	virtual const _String GetClassString()				{ return #x; } \
	virtual const Auto3D::RTTI* GetRTTI()				{ return GetRTTIStatic(); }\
	static int GetClassIDStatic()						{ return ClassID(x); }\
	static bool IsAbstractStatic ()						{ return GetRTTIStatic()->GetIsAbstract();}\
	static const char* GetClassNameStatic ()			{ return #x; }\
	static const _String GetClassStringStatic()			{ return #x; } \
	static const Auto3D::RTTI* GetRTTIStatic()			{ const static Auto3D::RTTI RTTIStatic(#x, y::GetRTTIStatic(), ClassID(x), true);return &RTTIStatic;}\

// Should be placed in every serializable object derived class
#define DECLARE_OBJECT_SERIALIZE(x)\
public: \


