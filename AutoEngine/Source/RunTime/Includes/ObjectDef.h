#pragma once

/**
* : Every non-abstract class that is derived from object has to place this inside the class Declaration
*/
#define	REGISTER_OBJECT_CLASS(_This,_Base) \
public: \
	using This = _This;\
	using Super = _Base;\
	~_This (); \
	virtual int GetClassID() { return ClassID (_This); }\
	virtual bool IsAbstract() { return GetRTTIStatic()->GetIsAbstract();}\
	virtual char* GetClassCstrName() { return #_This; }\
	virtual STRING GetClassString() { return #_This; } \
	virtual Auto3D::RTTI* GetRTTI() { return GetRTTIStatic(); }\
	static int GetClassIDStatic() { return ClassID(_This); }\
	static bool IsAbstractStatic () { return GetRTTIStatic()->GetIsAbstract();}\
	static char* GetClassCstrNameStatic () { return #_This; }\
	static STRING GetClassStringStatic() { return #_This; } \
	static Auto3D::RTTI* GetRTTIStatic() { static Auto3D::RTTI RTTIStatic(#_This, _Base::GetRTTIStatic(), ClassID(_This), false);return &RTTIStatic;}\

/**
* : Every abstract class that is derived from object has to place this inside the class Declaration
*/
#define	REGISTER_OBJECT_ABSTRACT_CLASS(_This, _Base) \
public: \
	using This = _This; \
	using Super = _Base; \
	virtual ~_This (); \
	virtual int GetClassID() { return ClassID (_This); }\
	virtual bool IsAbstract() { return GetRTTIStatic()->GetIsAbstract();}\
	virtual char* GetClassCstrName() { return #_This; }\
	virtual STRING GetClassString() { return #_This; } \
	virtual Auto3D::RTTI* GetRTTI() { return GetRTTIStatic(); }\
	static int GetClassIDStatic() { return ClassID(_This); }\
	static bool IsAbstractStatic () { return GetRTTIStatic()->GetIsAbstract();}\
	static char* GetClassCstrNameStatic () { return #_This; }\
	static STRING GetClassStringStatic() { return #_This; } \
	static Auto3D::RTTI* GetRTTIStatic() { static Auto3D::RTTI RTTIStatic(#_This, _Base::GetRTTIStatic(), ClassID(_This), true);return &RTTIStatic;}\

/**
* : This macro definition needs to be registered for each scenario
*/
#define REGISTER_LEVEL_CLASS(_This) \
public: \
	using This = _This; \
	using Super = LevelScene; \
	_This(SharedPtr<Ambient> ambient, int levelID)\
		:LevelScene(ambient, levelID){}\
	~_This() = default;\
	virtual char* GetClassCstrName() { return #_This; }\
	virtual STRING GetClassString() { return #_This; } \
	static char* GetClassCstrNameStatic () { return #_This; }\
	static STRING GetClassStringStatic() { return #_This; } \

/**
* @brief : Each script needs to register this macro definition
*/
#define REGISTER_SCRIPT_CLASS(_This) \
public: \
	using This = _This; \
	using Super = ScriptComponent; \
	_This(SharedPtr<Ambient> ambient)\
		:ScriptComponent(ambient){}\
	~_This() = default;\
	virtual char* GetClassCstrName() { return #_This; }\
	virtual STRING GetClassString() { return #_This; } \
	static char* GetClassCstrNameStatic () { return #_This; }\
	static STRING GetClassStringStatic() { return #_This; } \


/**
* @brief : Each space needs to register this macro definition
*/
#define REGISTER_SPACE_CLASS(_This) \
public: \
	using This = _This; \
	using Super = MotionSpace; \
	_This(SharedPtr<Ambient> ambient)\
		:MotionSpace(ambient){}\
	~_This() = default;\
	virtual char* GetClassCstrName() { return #_This; }\
	virtual STRING GetClassString() { return #_This; } \
	static char* GetClassCstrNameStatic () { return #_This; }\
	static STRING GetClassStringStatic() { return #_This; } \
