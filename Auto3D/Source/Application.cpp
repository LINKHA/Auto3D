#include "Core/RTTI/registration.h"
#include <iostream>


#include "Application.h"
#include "Thread/Thread.h"
#include "Core/ProcessUtils.h"
#include "Engine/Engine.h"

#include "Debug/DebugNew.h"

using namespace rttr;

namespace Auto3D
{
	struct MyStruct { MyStruct() {}; void func(double s) { std::cout <<s << std::endl; }; int data; };

	static void rttr_auto_register_reflection_function_();
	namespace
	{
		struct rttr__auto__register__
		{
			rttr__auto__register__()
			{
				rttr_auto_register_reflection_function_();
			}
		};
	}
	static const rttr__auto__register__ RTTR_CAT(auto_register__, __LINE__);
	static void rttr_auto_register_reflection_function_()
	{
		registration::class_<MyStruct>("MyStruct")
			.constructor<>()
			.property("data", &MyStruct::data)
			.method("func", &MyStruct::func);
	}

AApplication::AApplication() :
	_exitCode(EXIT_SUCCESS)
{
	if (!_engine)
		_engine = MakeUnique<AEngine>();
}

AApplication::~AApplication()
{
}

int AApplication::Run()
{

	//{
	//	type t = type::get<MyStruct>();
	//	for (auto& prop : t.get_properties())
	//		std::cout << "name: " << prop.get_name();

	//	for (auto& meth : t.get_methods())
	//		std::cout << "name: " << meth.get_name();
	//}
	//std::cout << std::endl;

	//{
	//	type t = type::get_by_name("MyStruct");
	//	variant var = t.create();    // will invoke the previously registered ctor

	//	constructor ctor = t.get_constructor();  // 2nd way with the constructor class
	//	var = ctor.invoke();
	//	std::cout << var.get_type().get_name(); // prints 'MyStruct'


	//}

	//std::cout << std::endl;
	//{
	//	MyStruct obj;

	//	property prop = type::get(obj).get_property("data");
	//	prop.set_value(obj, 23);

	//	variant var_prop = prop.get_value(obj);
	//	std::cout << var_prop.to_int(); // prints '23'


	//}

	//std::cout << std::endl;
	//{
	//	MyStruct obj;

	//	method meth = type::get(obj).get_method("func");
	//	meth.invoke(obj, 42.0);

	//	variant var = type::get(obj).create();
	//	meth.invoke(var, 42.0);

	//}

	// Make sure the engine is created properly
	if (_engine.Null())
		_engine = MakeUnique<AEngine>();
	
		
#if !defined(__GNUC__) || __EXCEPTIONS
	try
	{
#endif
		Init();
		if (!_engine->Init())
		{
			ErrorExit();
			return _exitCode;
		}

		Start();
		if (_exitCode)
			return _exitCode;

		while (!_engine->IsExiting())
		{
			if (_engine->Update())
			{
				Update();
				_engine->Render();
				UIDraw();
				_engine->FrameFinish();
			}
			else
				// If winodw is minimized reduce CPU load keep FPS to a minimum
				FThread::Sleep(1000 / MIN_FRAMES_PER_SECOND);
		}
		Stop();
		_engine->Exit();

		return _exitCode;
#if !defined(__GNUC__) || __EXCEPTIONS
	}
	catch (std::bad_alloc&)
	{
		ErrorDialog(GetTypeName(), "An application that has an out-of-memory condition will exit immediately.");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
#endif
}

void AApplication::ErrorExit(const FString& message)
{
	// Close the rendering window
	_engine->Exit(); 
	_exitCode = EXIT_FAILURE;

	if (!message.Length())
	{
		ErrorDialog(GetTypeName(), _startupErrors.Length() ? _startupErrors :
			"Application has been terminated due to unexpected error.");
	}
	else
		ErrorDialog(GetTypeName(), message);
}

}
